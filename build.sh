DIR="./build"
if [ -d "$DIR" ]; then
    mkdir ./build
fi
gcc *.c -o ./build/minecraftnt -lGL -lGLU -lglut -lm