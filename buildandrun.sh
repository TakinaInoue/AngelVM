./build.sh
ARCH=$(dpkg --print-architecture)
echo "Running build linux-${ARCH}"
./bin/linux-$ARCH/angelvm
