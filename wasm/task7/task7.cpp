#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <iostream>

using namespace emscripten;

float r,g,b = 0.0f;

//Program Object represents fully processed executable code,
//in the OpenGL Shading Language, for one or more Shader stages
GLuint programObject;

// Create a shader object, load the shader source, and compile the shader.
GLuint loadShader(GLenum type, const char *shaderSrc)
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);
	if (shader == 0)
		return 0;

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {
			char* infoLog = (char*) malloc(sizeof(char) * infoLen);
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			printf("Error compiling shader\n");
			free(infoLog);
		}

		glDeleteShader(shader);

		return 0;
	}

	return shader;
}

//Window context to draw
int createWindow()
{
	if (!glfwInit()) {
		fputs("Failed to initialize GLFW", stderr);
		emscripten_force_exit (EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow * window = glfwCreateWindow(320, 240, "My Window", NULL, NULL);

	if (!window) {
		fputs("Failed to create GLFW window", stderr);
		glfwTerminate();
		emscripten_force_exit (EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	return 0;
}

// Init programObject with shaders
int init()
{
	const char* vertexShaderStr =
			"attribute vec4 position;                 \n"
			"void main()                              \n"
			"{                                        \n"
			"   gl_Position = position; \n"
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
	vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderStr);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderStr);

	// Create the program object
	programObject = glCreateProgram();
	if (programObject == 0)
		return 0;

	// Attach shaders
	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	// Bind position to attribute 0
	glBindAttribLocation(programObject, 0, "position");

	// Link the program
	glLinkProgram(programObject);

	// Check the link status
	GLint linked;
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint infoLen = 0;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {
			char* infoLog = (char*) malloc(sizeof(char) * infoLen);

			glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
			printf("Error linking program\n");

			free(infoLog);
		}
		glDeleteProgram(programObject);
		return GL_FALSE;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//code
	return GL_TRUE;
}

// Render the animation
void draw()
{
	float vVertices[] = {
			0.0f, 0.5f, // Vertex 1 (X, Y)
			0.5f, -0.5f, // Vertex 2 (X, Y)
			-0.5f, -0.5f  // Vertex 3 (X, Y)
			};

	// No clientside arrays, so do this in a webgl-friendly manner
	GLuint vertexPosObject;
	glGenBuffers(1, &vertexPosObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPosObject);
	glBufferData(GL_ARRAY_BUFFER, 9 * 4, vVertices, GL_STATIC_DRAW);

	// Set the viewport
	glViewport(0, 0, 320, 240);

	// Clear the color buffer
	glClear (GL_COLOR_BUFFER_BIT);

	// Use the program object
	glUseProgram(programObject);

	// Load the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexPosObject);
	// Second param: using 2 floats per vertex (x, y)
	glVertexAttribPointer(0 /* ? */, 2, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(0);

	//Background color drawing
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLint uniColor = glGetUniformLocation(programObject, "color");
	g = 1.0f;
	glUniform3f(uniColor, r, g, b);

	//Shapes drawing:
	//The first parameter specifies the kind of primitive (commonly point, line or triangle)
	//the second parameter specifies how many vertices to skip at the beginning
	//the last parameter specifies the number of vertices (not primitives!) to process.
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

//Binded function
void myFunction()
{
	r = (rand() % 100) * 0.01f;
	g = (rand() % 100) * 0.01f;
	b = (rand() % 100) * 0.01f;

	float v1 = (rand() % 100) * 0.01f;
	std::cout << "rgb (" << r << ", " << "g" << g << ", " << "b" << b << ")";
	std::cout << std::endl;
}

int main()
{
	//Create Window
	createWindow();

	//Init program object with shaders
	if (!init())
		return 0;

	//Draw
	emscripten_set_main_loop(draw, 0, 1);

	return 0;
}

EMSCRIPTEN_BINDINGS(my_module)
{
	function("myFunction", &myFunction);
}
