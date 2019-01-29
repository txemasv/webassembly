#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <iostream>

//Window context to draw
int createWindow()
{
    //TODO
	// false in case something is wrong
	return 0;
}

// Init programObject with shaders
int init()
{
	//TODO
	// true in case everithing is ok
	return 1;
}

// Render the animation
void draw()
{
	//TODO
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

#ifdef __cplusplus
extern "C" {
#endif

//Binded function
void EMSCRIPTEN_KEEPALIVE myFunction()
{
	std::cout << "hello" << std::endl;
}

#ifdef __cplusplus
}

#endif



