set echo on

#mkdir bin

CFiles=$(find . -type f -name "*.c")
echo "Files found to be compiled:" $CFiles

echo "Building game initialized"
assembly="game"
compilerflags="-g -fPIC"
includeflags="-Idev/mk2 -I./glad/include"
linkerflags="-g -shared -lSDL2 -lSDL2_mixer -msse2 -lm -ldl -no-pie"
echo "Compilation is using the Compiler flags: " $compilerflags
echo "Compilation is using the Include  flags: " $includeflags
echo "Compilation is using the Linker   flags: " $linkerflags

echo "Building $assembly..."

echo gcc $CFiles -o $assembly $compilerflags $includeflags $linkerflags
gcc $CFiles $compilerflags $includeflags $linkerflags -o $assembly
