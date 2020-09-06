#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <stdio.h>

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 640;
int g_gl_height = 480;
float aspect = (float) g_gl_width / (float) g_gl_height;
float rad45 = glm::radians(45.0f);
float fovy = glm::radians(50.0f);
float rad81 = glm::radians(81.0f);

// Resize callback
void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
	g_gl_width = width;
	g_gl_height = height;
	aspect = (float) g_gl_width / (float) g_gl_height;
}

int main() {
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "My spinning cube", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwSetWindowSizeCallback(window, glfw_window_size_callback);
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* glversion = glGetString(GL_VERSION); // version as a string
	const GLubyte* glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", glversion);
	printf("GLSL version supported %s\n", glslversion);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	// Vertex Shader
	const char* vertex_shader =
		"#version 130                                                       \n"
		"                                                                   \n"
		"in vec4 position;                                                  \n"
		"in vec3 color;                                                     \n"
		"                                                                   \n"
		"out vec4 vs_out;                                                   \n"
		"                                                                   \n"
		"uniform mat4 mv_matrix;                                            \n"
		"uniform mat4 proj_matrix;                                          \n"
		"                                                                   \n"
		"void main(void)                                                    \n"
		"{                                                                  \n"
		"    gl_Position = proj_matrix * mv_matrix * position;              \n"
		"    vs_out = vec4(color, 1.0);                                     \n"
		"}";

	// Fragment Shader
	const char* fragment_shader =
		"#version 130                                                       \n"
		"                                                                   \n"
		"out vec4 color;                                                    \n"
		"                                                                   \n"
		"in vec4 vs_out;                                                    \n"
		"                                                                   \n"
		"void main(void)                                                    \n"
		"{                                                                  \n"
		"    color = vs_out;                                                \n"
		"}";

	// Shaders compilation
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	// Create program, attach shaders to it and link it
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, vs);
	glAttachShader(shader_programme, fs);
	glLinkProgram(shader_programme);

	// Vertex Array Object
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Cube to be rendered
	const GLfloat vertex_positions[] = {
		-0.25f,  0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		 0.25f, -0.25f, -0.25f,

		 0.25f, -0.25f, -0.25f,
		 0.25f,  0.25f, -0.25f,
		-0.25f,  0.25f, -0.25f,

		 0.25f, -0.25f, -0.25f,
		 0.25f, -0.25f,  0.25f,
		 0.25f,  0.25f, -0.25f,

		 0.25f, -0.25f,  0.25f,
		 0.25f,  0.25f,  0.25f,
		 0.25f,  0.25f, -0.25f,

		 0.25f, -0.25f,  0.25f,
		-0.25f, -0.25f,  0.25f,
		 0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		-0.25f,  0.25f,  0.25f,
		 0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f, -0.25f,
		-0.25f,  0.25f, -0.25f,
		-0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		 0.25f, -0.25f,  0.25f,
		 0.25f, -0.25f, -0.25f,

		 0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f,  0.25f,

		-0.25f,  0.25f, -0.25f,
		 0.25f,  0.25f, -0.25f,
		 0.25f,  0.25f,  0.25f,

		 0.25f,  0.25f,  0.25f,
		-0.25f,  0.25f,  0.25f,
		-0.25f,  0.25f, -0.25f
	};

	const GLfloat vertex_colors[] = {
		0.0f, 0.0f, 0.0f, // Negro
		0.0f, 0.0f, 0.0f, // Negro
		0.0f, 0.0f, 0.0f, // Negro

		1.0f, 0.0f, 0.0f, // Rojo
		1.0f, 0.0f, 0.0f, // Rojo
		1.0f, 0.0f, 0.0f, // Rojo

		1.0f, 0.5f, 0.0f, // Naranja
		1.0f, 0.5f, 0.0f, // Naranja
		1.0f, 0.5f, 0.0f, // Naranja

		1.0f, 1.0f, 0.0f, // Amarillo
		1.0f, 1.0f, 0.0f, // Amarillo
		1.0f, 1.0f, 0.0f, // Amarillo

		0.5f, 1.0f, 0.0f, // Pistacho
		0.5f, 1.0f, 0.0f, // Pistacho
		0.5f, 1.0f, 0.0f, // Pistacho

		0.0f, 1.0f, 0.0f, // Verde
		0.0f, 1.0f, 0.0f, // Verde
		0.0f, 1.0f, 0.0f, // Verde

		0.0f, 1.0f, 1.0f, // Cyan
		0.0f, 1.0f, 1.0f, // Cyan
		0.0f, 1.0f, 1.0f, // Cyan

		0.0f, 0.5f, 1.0f, // Turquesa
		0.0f, 0.5f, 1.0f, // Turquesa
		0.0f, 0.5f, 1.0f, // Turquesa

		0.0f, 0.0f, 1.0f, // Azul
		0.0f, 0.0f, 1.0f, // Azul
		0.0f, 0.0f, 1.0f, // Azul

		0.5f, 0.0f, 1.0f, // Violeta
		0.5f, 0.0f, 1.0f, // Violeta
		0.5f, 0.0f, 1.0f, // Violeta

		1.0f, 0.0f, 1.0f, // Magenta
		1.0f, 0.0f, 1.0f, // Magenta
		1.0f, 0.0f, 1.0f, // Magenta

		1.0f, 1.0f, 1.0f, // Blanco
		1.0f, 1.0f, 1.0f, // Blanco
		1.0f, 1.0f, 1.0f, // Blanco
	};

	// Vertex Buffer Object(s)
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);

	GLuint vbo_color = 1;
	glGenBuffers(1, &vbo_color);

	// Vertex attributes
	// 0: vertex position
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
				 sizeof(vertex_positions),
				 vertex_positions,
				 GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// 1: vertex color
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	glBufferData(GL_ARRAY_BUFFER,
				sizeof(vertex_colors),
				vertex_colors,
				GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	// Uniforms
	// - Model-View matrix
	// - Projection matrix
	GLint mv_location, proj_location;
	mv_location = glGetUniformLocation(shader_programme, "mv_matrix");
	proj_location = glGetUniformLocation(shader_programme, "proj_matrix");

	// Render loop
	while(!glfwWindowShouldClose(window)) {

		double currentTime = glfwGetTime();
		float f = (float)currentTime * 0.3f;

		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.25f, 0.0f, 1.0f);
		glViewport(0, 0, g_gl_width, g_gl_height);
		glUseProgram(shader_programme);
		glBindVertexArray(vao);

		// To manage the model-view and projection transformations:
		// - use routines and structures from GLM
		glm::mat4 mv_matrix, proj_matrix;
		mv_matrix = glm::translate(glm::vec3(0.0f, 0.0f, -4.0f)) *
					glm::translate(glm::vec3(sinf(2.1f * f) * 0.5f,
											cosf(1.7f * f) * 0.5f,
											sinf(1.3f * f) * cosf(1.5f * f) * 2.0f)) *
					glm::rotate((float)currentTime * rad45, glm::vec3(0.0f, 1.0f, 0.0f)) *
					glm::rotate((float)currentTime * rad81, glm::vec3(1.0f, 0.0f, 0.0f));
					// Esto se ha cambiado a radianes porque

		// transfer mv_matrix uniform to shaders:
		glUniformMatrix4fv(mv_location, 1, GL_FALSE, glm::value_ptr(mv_matrix));

		proj_matrix = glm::perspective(fovy, aspect, 0.1f, 1000.0f);

		// transfer proj_matrix uniform to shaders:
		glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(proj_matrix));

		// Draw cube
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// update other events like input handling
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();

	return 0;
}
