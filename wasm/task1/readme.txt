# env(Linux)
source emsdk/./emsdk_env.sh -build=Release

# build
emcc task1.cpp -s WASM=1 -o index.html

# server
emrun --no_browser --port 8080 .