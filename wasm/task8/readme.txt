# env(Linux)
source emsdk/./emsdk_env.sh -build=Release

# build
emcc --bind -o task8.js task8.cpp -s USE_WEBGL2=1 -s USE_GLFW=3 -s WASM=1

# server
emrun --no_browser --port 8080 .