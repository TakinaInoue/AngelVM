module angeloscope.Lexer;

import std.utf;
import std.ascii;
import std.stdio;

enum TokenType {
    Invalid,

    Identifier,

    KeywordImport,
    KeywordPublic,
    KeywordPrivate,
    KeywordConst,
    KeywordVar,
    KeywordStructure,
    KeywordMethod,
    KeywordFunction,
    KeywordAs,

    LParen, RParen,
    LBrace, RBrace,
    LBracket, RBracket,

    Plus, Minus, Star, Slash, Percentage,
    Dot, Comma, Semicolon,

    IntLiteral,
    CharLiteral,
    FloatLiteral,
    StringLiteral,

    EndOfFile,
}

struct Token {
    TokenType type;
    string acc;
    uint line;
}

class Lexer {
    private static TokenType[dstring] symbols;
    private static TokenType[dstring] keywords;

    dstring source;
    dchar current;
    size_t position;
    uint line;
    bool isEOF;

    this(string source) {
        if (keywords.length == 0 || symbols.length == 0) {
            keywords["import"] = TokenType.KeywordImport;
            keywords["private"] = TokenType.KeywordPrivate;
            keywords["public"] = TokenType.KeywordPublic;
            keywords["const"] = TokenType.KeywordConst;
            keywords["var"] = TokenType.KeywordVar;
            keywords["structure"] = TokenType.KeywordStructure;
            keywords["method"] = TokenType.KeywordMethod;
            keywords["function"] = TokenType.KeywordFunction;
            keywords["as"] = TokenType.KeywordAs;

            symbols["("] = TokenType.LParen;
            symbols[")"] = TokenType.RParen;
            symbols["{"] = TokenType.LBrace;
            symbols["}"] = TokenType.RBrace;
            symbols["["] = TokenType.LBracket;
            symbols["]"] = TokenType.RBracket;

            symbols["+"] = TokenType.Plus;
            symbols["-"] = TokenType.Minus;
            symbols["*"] = TokenType.Star;
            symbols["/"] = TokenType.Slash;
            symbols["%"] = TokenType.Percentage;
            
            symbols["."] = TokenType.Dot;
            symbols[","] = TokenType.Comma;
            symbols[";"] = TokenType.Semicolon;
        }
        this.source = toUTF32(source);
        this.line = 1;
        this.position = 0;
        this.isEOF = false;
        this.NextChar();
    }

    Token NextToken() {
        while(!isEOF && current.isWhite) {
            NextChar();
        }

        if (isEOF) {
            return NewToken(TokenType.EndOfFile, "");
        }

        if (current.isDigit) {
            dstring num = "";
            TokenType t = TokenType.IntLiteral;
            while (!isEOF && (current.isDigit || current == '.')) {
                if (current == '.') t = TokenType.FloatLiteral;
                num ~= NextChar();
            }
            return NewToken(t, num);
        }
        if (isAlpha(current)) {
            dstring id = "";
            while (!isEOF && (current.isAlpha || current.isDigit || current == '_')) {
                id ~= NextChar();
            }
            return NewToken(GetIDType(id), id);
        }

        if (current == '\"') {
            dstring str = "";
            NextChar();
            while (!isEOF && current != '\"') {
                str ~= NextChar();
            }
            NextChar();
            return NewToken(TokenType.StringLiteral, str);
        }
        
        if (current == '\'') {
            dstring str = "";
            NextChar();
            while (!isEOF && current != '\'') {
                str ~= NextChar();
            }
            NextChar();
            return NewToken(TokenType.CharLiteral, str);
        }

        foreach (dstring sy ; symbols.byKey()) {
            dstring c = current ~ source[position .. position + sy.length - 1];
            if (c == sy) {
                for (int i = 0; i < sy.length; i++) NextChar();
                return NewToken(symbols[sy], "");
            }
        }

        return NewToken(TokenType.Invalid, [NextChar()]);
    }

    private Token NewToken(TokenType t, dstring n) {
        return Token(t, toUTF8(n), line);
    }

    private TokenType GetIDType(dstring id) {

        TokenType* t = id in keywords;
        return t is null ? TokenType.Identifier : *t;
    }

    private dchar NextChar() {
        dchar last = current;
        if (position >= source.length) {
            isEOF = true;
        } else {
            current = source[position++];
            if (current == '\n') this.line++;
        }
        return last;
    }
}