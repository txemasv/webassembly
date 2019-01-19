#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#include <GL/glfw.h>

using namespace emscripten;

float red = 0.0f;
float green = 0.0f;
float blue = 0.0f;
float alpha = 1.0f;

bool started = false;

void draw() 
{
    red += 0.1f;
    if (red > 1.0f)  {
        red = 0.0f;
    }
    
    green += 0.1f;
    if (green > 1.0f)  {
        green = 0.0f;
    }
    
    blue += 0.1f;
    if (blue > 1.0f)  {
        blue = 0.0f;
    }
    
    glClearColor (red, green, blue, alpha) ;
    glClear ( GL_COLOR_BUFFER_BIT ) ;
    
    if(!started)
        emscripten_pause_main_loop();
}

void startDrawing() 
{
    started = true;
    emscripten_resume_main_loop();
    printf("startDrawing");
}

void stopDrawing() 
{
    emscripten_pause_main_loop();
    printf("stopDrawing");
}

int main() 
{
    /* Initialize the library */
    if (!glfwInit()) {
        printf("Could not initialize library\n");
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    if(glfwOpenWindow(320, 240, 0,0,0,0,16,0, GLFW_WINDOW) != GL_TRUE) {
        printf("Could not create OpenGL window\n");
        glfwTerminate();
        return -1;
    }

    emscripten_set_main_loop(draw, 0, 1);

    return 0;
}

EMSCRIPTEN_BINDINGS(my_module) 
{
    function("startDrawing", &startDrawing);
    function("stopDrawing", &stopDrawing);
}
