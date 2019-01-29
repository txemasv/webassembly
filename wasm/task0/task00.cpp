#include <emscripten/bind.h>
#include <emscripten/emscripten.h>

using namespace emscripten;

int getDoubleNumber(int number) {
  return number * 2;
}

int main() {
  return getDoubleNumber(4);
}

EMSCRIPTEN_BINDINGS(my_module)
{
    function("getDoubleNumber", &getDoubleNumber);
}
