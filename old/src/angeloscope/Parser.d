module angeloscope.Parser;

import std.stdio;
import std.format;
import std.algorithm;
import std.file : exists, isFile, readText;

import angel.arch;
import angel.memory;
import angel.bytecode;

import angeloscope.Lexer;       

struct Local {
    ushort offset;
    string name;
    int declLine;
    size_t scopeDepth;
    bool enforceType;
    ValueType enforcedType;
}

class CompilationFragment {
    Fragment frag;
    Local[string] locals;
    int currScopeDepth;
    int localMemSize;

    this(string name, string sourceFile) {
        frag = new Fragment(name, sourceFile);
        currScopeDepth = 0;
    }

    void Write(size_t line, ubyte[] inst...) {
        if (frag.lines.length > 0 && frag.lines[frag.lines.length - 1].line == line) {
            frag.lines[frag.lines.length -1].endOffset += inst.length;
        } else {
            frag.lines ~= Line(line, frag.instructions.length);
        }
        frag.instructions ~= inst;
    }

    void WriteU16(int line, ushort d) {
        ubyte a, b; Split(d, a, b);
        Write(line, a, b);
    }

    void WriteValue(int line, ubyte opcode, Value* v) {
        frag.values ~= Value(v.vtype, v.as);

        ubyte a, b; Split(cast(ushort)(frag.values.length - 1), a, b);
        Write(line, opcode, a, b);
    }
}

class Program {
    string programName;
    CompilationFragment[] fragments;

    // not ready to implement
    //Structure[] structures;

    this(string name) {
        this.programName = name;
    }
}

public class ParserModuleResolver {
    private this() {}
    public static bool[string] ModulesLoaded;
    public static string[] includePaths = [];
}

public class Parser {

    string filename; 
    Lexer lexer;
    Token current;
    Program program;
    Program[] dependencies;
    bool hadError;

    this(string filename, string content) {
        this.lexer = new Lexer(content);
        this.filename = filename;
        this.hadError = false;
        this.program = new Program(filename);
        Next();    
    }

    public Program Parse() {
        while (current.type != TokenType.EndOfFile) {
            if (Consume(TokenType.KeywordImport)) {
                Token pth = Match("Expected a valid import path!", TokenType.StringLiteral);
                 
                bool Resolve(string p) {
                    if ((p in ParserModuleResolver.ModulesLoaded) !is null)
                        return true;
                    if (exists(p) && isFile(p)){
                        Parser parser = new Parser(p, readText(p));
                        dependencies ~= parser.Parse();
                        if (parser.hadError)
                            return false;
                        return true;
                    }
                    return false;
                }

                if (!Resolve(pth.acc)) {
                    bool found = false;
                    foreach (path ; ParserModuleResolver.includePaths) {
                        if (Resolve(path ~ pth.acc)){
                            found = true;
                            break;
                        }
                    }
                    if (found) continue; 
                } else continue;
                Error(format("Could not import module '%s' in include paths: %s",
                    pth.acc, ParserModuleResolver.includePaths)); 
                return null;
            } else if (Consume(TokenType.KeywordFunction)) {
                ParseMethod();
            }
            Error("Unexpected token: '" ~ current.acc ~"'");
        }
        return program;
    }

    private CompilationFragment ParseMethod() {
        CompilationFragment fragment = new CompilationFragment(
            Match("Expected a valid name for this method/function!", TokenType.Identifier).acc,
            filename
        );
        Match("Expected a left parenthesis to open parameter block", TokenType.LParen);
        
        if (current.type != TokenType.RParen) {
            do {
                ParseLocalDefinition(fragment);
            } while(Consume(TokenType.Comma)); 
        }
        Match("Expected a right parenthesis to close parameter block", TokenType.RParen);
        Match("Expected a left brace to close parameter block", TokenType.LBrace);

        StartScope(fragment);
        while (current.type != TokenType.RBrace && current.type != TokenType.EndOfFile) {
            Next(); // for now
        }
        EndScope(fragment);
        Match("Expected a right brace to close parameter block", TokenType.RBrace);

        return fragment;
    }

    private void StartScope(CompilationFragment fragment) {
        fragment.currScopeDepth++;
    }

    private void EndScope(CompilationFragment fragment) {
        fragment.currScopeDepth--;
        foreach (string key ; fragment.locals.byKey) {
            Local* local = &fragment.locals[key];
            if (local.scopeDepth > fragment.currScopeDepth)
                fragment.locals.remove(key);
        }
    }

    private void ParseLocalDefinition(CompilationFragment fragment) {
        Token name = Match("Expected a valid local name.", TokenType.Identifier);
        if (Consume(TokenType.KeywordAs)) {
            fragment.locals[name.acc] = Local(
                cast(ushort) fragment.locals.length,
                name.acc, name.line, fragment.currScopeDepth, true,
                IdentifyType(Match("Expected a valid typename", TokenType.Identifier))
            );
        } else {
            fragment.locals[name.acc] = Local(
                cast(ushort) fragment.locals.length,
                name.acc, name.line, fragment.currScopeDepth, false
            );
        }
        fragment.localMemSize++;
    }

    private ValueType IdentifyType(Token t) {
        switch(t.acc) {
            case "byte":
                return ValueType.Byte;
            case "short":
                return ValueType.Short;
            case "int":
                return ValueType.Int;
            case "long":
                return ValueType.Long;
            case "float":
                return ValueType.Float;
            case "double":
                return ValueType.Double;
            default:
                Error("Unknown typename: " ~ t.acc);
                return ValueType.Float;
        }
    }

    private bool Consume(TokenType T) {
        if (current.type == T) {
            Next();
            return true;
        }
        return false;
    }

    private void Error(string msg) {
        writeln("Error in file ", filename, " at line ", current.line, " > ", msg);
        hadError = true;
    }

    private Token Match(string errMsg, TokenType[] types...) {
        foreach (TokenType t; types) {
            if (current.type == t)
                return Next();
        }
        Error(errMsg);
        Next();
        return Token();
    }

    private Token Next() {
        Token a = current;
        current = lexer.NextToken();
        return a;
    }
}