# env(Linux)
source emsdk/./emsdk_env.sh -build=Release

# build
emcc -o task9.js task9.cpp -s USE_WEBGL2=1 -s USE_GLFW=3 -s WASM=1 -s NO_EXIT_RUNTIME=1 -s EXTRA_EXPORTED_RUNTIME_METHODS=["ccall"]

# server
emrun --no_browser --port 8080 .