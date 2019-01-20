#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

using namespace emscripten;

/*
 * Adding comments in my personal branch
 * */

//Window context to draw
int createWindow()
{
    //TODO
	// false in case something is wrong
	return 0;
}

// Init programObject with Shaders
int init()
{
	//TODO
	// true in case everything is OK
	return 1;
}

// Render the animation
void draw()
{
	//TODO
}

//Binded function
std::string myFunction()
{
	return "hello comments to new branch!!";
}

int main()
{
    //Create Window
    createWindow();

    //Init program object with shaders
    if ( !init () )
      return 0;

    //Draw
    emscripten_set_main_loop(draw, 0, 1);

    return 0;
}

EMSCRIPTEN_BINDINGS(my_module)
{
    function("myFunction", &myFunction);
}
