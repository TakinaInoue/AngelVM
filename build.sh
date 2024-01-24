ARCH=$(dpkg --print-architecture)
echo "Building for ${ARCH}"
clang -c src/main.c -o bin/main.o
clang bin/main.o -o bin/linux-$ARCH/angelvm
