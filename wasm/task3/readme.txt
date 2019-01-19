# env(linux)
source emsdk/./emsdk_env.sh -build=Release

# build
emcc --bind -o task3.js task3.cpp -s WASM=1

# server
emrun --no_browser --port 8080 .