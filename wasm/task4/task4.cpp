#include <emscripten/bind.h>
#include <emscripten/emscripten.h>

//window and context management and input management
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#include <cmath>

using namespace emscripten;

bool startedLoop = false;

//Program Object represents fully processed executable code, 
//in the OpenGL Shading Language, for one or more Shader stages
GLuint programObject;

typedef struct {float x; float y;} POINT;

POINT rotatePoint(float angle, POINT &p) 
{
  float s = sin(angle);
  float c = cos(angle);

  // rotate point
  float xnew = p.x * c - p.y * s;
  float ynew = p.x * s + p.y * c;

  // translate point back
  p.x = xnew;
  p.y = ynew;
  return p;
}

POINT pCenter {0.0f, 0.25f}; //rectangle center

//Window context to draw (using glfw3)
int createWindow()
{
    if (!glfwInit()) {
        fputs("Failed to initialize GLFW", stderr);
        emscripten_force_exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow * window = glfwCreateWindow(320, 240, "My Window", NULL, NULL);

    if (!window) {
        fputs("Failed to create GLFW window", stderr);
        glfwTerminate();
        emscripten_force_exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    return 0;
}

// Create a shader object, load the shader source, and compile the shader.
GLuint loadShader ( GLenum type, const char *shaderSrc )
{
    GLuint shader;
    GLint compiled;

    // Create the shader object
   shader = glCreateShader ( type );
   if ( shader == 0 )
    return 0;

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );

   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled ) {
      GLint infoLen = 0;
      
      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );  

      if ( infoLen > 1 ) {
        char* infoLog = (char*) malloc (sizeof(char) * infoLen );
        glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );  
        printf ( "Error compiling shader\n");      
        free ( infoLog );
      }

      glDeleteShader ( shader );
      
      return 0;
   }

    return shader;
}

// Init programObject with shaders
int init() 
{
    const char* vertexShaderStr =  
      "attribute vec4 position;                 \n"
      "uniform vec4 translation;                \n"
      "void main()                              \n"
      "{                                        \n"
      "   gl_Position = position + translation; \n"
      "}                                        \n";

    const char* fragmentShaderStr =  
      "precision mediump float;             \n"
      "uniform vec3 color;                  \n"
      "void main()                          \n"
      "{                                    \n"
      "  gl_FragColor = vec4 ( color, 1.0 );\n"
      "}                                    \n";
      
    GLuint vertexShader;
    GLuint fragmentShader;

    // Load the vertex/fragment shaders
    vertexShader = loadShader ( GL_VERTEX_SHADER, vertexShaderStr );
    fragmentShader = loadShader ( GL_FRAGMENT_SHADER, fragmentShaderStr );

    // Create the program object
    programObject = glCreateProgram ( );
    if ( programObject == 0 )
        return 0;
    
    // Attach shaders
    glAttachShader ( programObject, vertexShader );
    glAttachShader ( programObject, fragmentShader );
    
    // Bind position to attribute 0   
    glBindAttribLocation ( programObject, 0, "position" );

    // Link the program
    glLinkProgram ( programObject );

    // Check the link status
    GLint linked;
    glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );  
    if ( !linked ) {
         GLint infoLen = 0;
         glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

        if ( infoLen > 1 ) {
            char* infoLog = (char*) malloc (sizeof(char) * infoLen );

            glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
            printf ( "Error linking program\n"); 

            free ( infoLog );
        }
        glDeleteProgram ( programObject );
        return GL_FALSE;
    }
    
    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
      
    //code
    return GL_TRUE;
}

void startDrawing() 
{
  startedLoop = true;
  emscripten_resume_main_loop();
}

void stopDrawing() 
{
  emscripten_pause_main_loop();
}

// Render the animation
void draw() 
{
  // Rotate the center of the rectangle (pCenter) around 0,0
  rotatePoint(-0.1f, pCenter);

  float rx = 0.2f; // radio x rectangle
  float ry = 0.1f; // radio y rectacgle

  //Vertices
  POINT p1, p2, p3, p4;
  p1.x = pCenter.x-rx;
  p1.y = pCenter.y-ry;
  p2.x = pCenter.x-rx;
  p2.y = pCenter.y+ry;
  p3.x = pCenter.x+rx;
  p3.y = pCenter.y+ry;
  p4.x = pCenter.x+rx;
  p4.y = pCenter.y-ry;

  GLfloat vVertices[] = {
     p1.x, p1.y,
     p2.x, p2.y,
     p3.x, p3.y,
     p4.x, p4.y,
  };

  // No clientside arrays, so do this in a webgl-friendly manner
  GLuint vertexPosObject;
  glGenBuffers(1, &vertexPosObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexPosObject);
  glBufferData(GL_ARRAY_BUFFER, 9*4, vVertices, GL_STATIC_DRAW);

  // Set the viewport
  glViewport ( 0, 0, 320, 240 );

  // Clear the color buffer
  glClear ( GL_COLOR_BUFFER_BIT );

  // Use the program object
  glUseProgram ( programObject );

  // Load the vertex data
  glBindBuffer(GL_ARRAY_BUFFER, vertexPosObject);
  // Second param: using 2 floats per vertex (x, y)
  glVertexAttribPointer(0 /* ? */, 2, GL_FLOAT, 0, 0, 0);
  glEnableVertexAttribArray(0);

  //Modify rectangle color
  GLint uniColor = glGetUniformLocation(programObject, "color");
  glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);

  //Modify rectangle position
  GLint uniTrans = glGetUniformLocation(programObject, "translation");
  glUniform4f(uniTrans, pCenter.x, pCenter.y, 0.0f, 0.0f);

  //Background color drawing
  glClearColor (0.9f, 0.9f, 0.9f, 1.0f) ;
  glClear ( GL_COLOR_BUFFER_BIT ) ;

  //Shapes drawing:
  //The first parameter specifies the kind of primitive (commonly point, line or triangle)
  //the second parameter specifies how many vertices to skip at the beginning
  //the last parameter specifies the number of vertices (not primitives!) to process.
  glDrawArrays ( GL_LINE_LOOP, 0, 4 );  

  if(!startedLoop)
      emscripten_pause_main_loop();
}

int main() 
{
    //Create Window
    createWindow();

    //Init
    if ( !init () )
      return 0;

    //Render
    emscripten_set_main_loop(draw, 0, 1);

    return 0;
}

EMSCRIPTEN_BINDINGS(my_module) 
{
  function("startDrawing", &startDrawing);
  function("stopDrawing", &stopDrawing);
}
