# env(Linux)
source emsdk/./emsdk_env.sh -build=Release

# build
emcc --bind -o task2.js task2.cpp -s WASM=1

# server
emrun --no_browser --port 8080 .