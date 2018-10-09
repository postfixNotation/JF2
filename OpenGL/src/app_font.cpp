#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <text.hpp>

const GLFWvidmode* mode;
GLuint shader_handle;
GLint win_width, win_height;
std::string kWindowTitle = "Prototype Application";
GLFWwindow *window = nullptr;
GLuint vs, fs;

std::string LoadFile(const std::string&);

int main() {
	if (!glfwInit()) {
		std::cerr << "ERROR: COULD NOT START GLFW3" << std::endl;
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	mode = glfwGetVideoMode(monitor);
	win_width = static_cast<float>(mode->width) * (2.0f / 3);
	win_height = static_cast<float>(mode->height) * (2.0f / 3);

	window = glfwCreateWindow(
		win_width,
		win_height,
		kWindowTitle.c_str(),
		nullptr,
		nullptr
	);
	if (!window) {
		std::cerr << "ERROR: COULD NOT OPEN WINDOW WITH GLFW3" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(
		window,
		[](GLFWwindow* win, int key, int scancode, int action, int mode) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(win, 1);
		}
		else if (key == GLFW_KEY_L && action == GLFW_PRESS) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (key == GLFW_KEY_K && action == GLFW_PRESS) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	});

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
		glViewport(0, 0, width, height);
		win_width = width;
		win_height = height;
	});

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cerr << "ERROR: COULD NOT INITIALIZE GLEW" << std::endl;
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, win_width, win_height);
	glEnable(GL_CULL_FACE);

	std::string vertex_shader_string = LoadFile("../shader/font_vert.glsl");
	std::string fragment_shader_string = LoadFile("../shader/font_frag.glsl");
	const GLchar* vertex_shader = vertex_shader_string.c_str();
	const GLchar* fragment_shader = fragment_shader_string.c_str();

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(
		vs,
		1,
		&vertex_shader,
		nullptr
	);
	glCompileShader(vs);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(
		fs,
		1,
		&fragment_shader,
		nullptr
	);
	glCompileShader(fs);

	shader_handle = glCreateProgram();
	glAttachShader(shader_handle, fs);
	glAttachShader(shader_handle, vs);
	glLinkProgram(shader_handle);

	glDeleteShader(vs);
	glDeleteShader(fs);

	Text text{ shader_handle, static_cast<size_t>(win_width), static_cast<size_t>(win_height) };
	text.SetFileName("../fonts/Wallpoet-Regular.ttf");

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0.8f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		text.RenderText(
			"Welcome to OpenGL ©",
			10.0f,
			30.0f,
			0.6f,
			glm::vec3(0.0, 0.0f, 0.0f)
		);

		glfwSwapBuffers(window);
	}

	glDeleteProgram(shader_handle);
	glfwTerminate();
	return 0;
}

std::string LoadFile(const std::string &file_name) {
	std::ifstream fs;
	std::stringstream ss;

	try {
		fs.open(file_name, std::ifstream::in);

		if (!fs.fail()) {
			ss << fs.rdbuf();
		}

		fs.close();
	}
	catch (std::exception ex) {
		std::cout << "EXCEPTION: LOADING SHADER SOURCE" << std::endl;
		std::cout << ex.what() << std::endl;
	}

	return ss.str();
}
