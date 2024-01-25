ARCH=$(dpkg --print-architecture)
echo "Running build linux-${ARCH}"
./build.sh && ./bin/linux-$ARCH/angelvm
