#include <emscripten/bind.h>
#include <cctype>

using namespace emscripten;

std::string capitalize(std::string str)
{
    for(auto& x: str)
        x = toupper(x);
    return str;
}

EMSCRIPTEN_BINDINGS(my_module)
{
    function("capitalize", &capitalize);
}
