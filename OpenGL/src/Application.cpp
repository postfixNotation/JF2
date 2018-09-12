#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/rotate_normalized_axis.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <iostream>
#include <sstream>

#include <texture2d.hpp>
#include <camera.hpp>
#include <irrKlang/irrKlang.h>

glm::mat4 proj{}, model{}, view{};
float ratio;
float near = 0.1f;
float far = 100.0f;
glm::vec3 cube_pos{};
const GLFWvidmode* mode;
GLuint prog_handle;
GLint win_width, win_height;
std::string kWindowTitle = "Camera Prototype Application";
GLFWwindow *window = nullptr;

void Update(double);

namespace camera {
	FPSCamera fps_camera{ glm::vec3{0.0f, 0.0f, 5.0f} };
	float yaw{ 0.0f };
	float pitch{ 0.0f };
	float radius{ 10.0f };
	constexpr float kZoomSensitivity = -2.0f;
	constexpr float kMoveSpeed = 5.0f;
	constexpr float kMouseSensitivity = 0.1f;
}

int main() {
	const GLubyte *renderer;
	const GLubyte *version;
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	GLuint vs, fs;

	Texture2D crate_cube{}, floor{};

	struct Color {
		float r{ 255.0f / 255.0f };
		float g{ 255.0f / 255.0f };
		float b{ 153.0f / 255.0f };
		float a{ 1.0f };
	} rgb;

	GLfloat half_cube_length{ 1.0f };

	GLfloat points[] = {
		-half_cube_length, half_cube_length, half_cube_length, 0.0f, 1.0f,
		half_cube_length, half_cube_length, half_cube_length, 1.0f, 1.0f,
		half_cube_length, -half_cube_length, half_cube_length, 1.0f, 0.0f,
		-half_cube_length, -half_cube_length, half_cube_length, 0.0f, 0.0f,

		half_cube_length, half_cube_length, half_cube_length, 0.0f, 1.0f,
		half_cube_length, half_cube_length, -half_cube_length, 1.0f, 1.0f,
		half_cube_length, -half_cube_length, -half_cube_length, 1.0f, 0.0f,
		half_cube_length, -half_cube_length, half_cube_length, 0.0f, 0.0f,

		-half_cube_length, half_cube_length, half_cube_length, 0.0f, 1.0f,
		-half_cube_length, -half_cube_length, half_cube_length, 1.0f, 1.0f,
		-half_cube_length, -half_cube_length, -half_cube_length, 1.0f, 0.0f,
		-half_cube_length, half_cube_length, -half_cube_length, 0.0f, 0.0f,

		-half_cube_length, half_cube_length, -half_cube_length, 0.0f, 1.0f,
		half_cube_length, half_cube_length, -half_cube_length, 1.0f, 1.0f,
		half_cube_length, -half_cube_length, -half_cube_length, 1.0f, 0.0f,
		-half_cube_length, -half_cube_length, -half_cube_length, 0.0f, 0.0f,

		-half_cube_length, half_cube_length, half_cube_length, 0.0f, 1.0f,
		-half_cube_length, half_cube_length, -half_cube_length, 1.0f, 1.0f,
		half_cube_length, half_cube_length, -half_cube_length, 1.0f, 0.0f,
		half_cube_length, half_cube_length, half_cube_length, 0.0f, 0.0f,

		-half_cube_length, -half_cube_length, half_cube_length, 0.0f, 1.0f,
		-half_cube_length, -half_cube_length, -half_cube_length, 1.0f, 1.0f,
		half_cube_length, -half_cube_length, -half_cube_length, 1.0f, 0.0f,
		half_cube_length, -half_cube_length, half_cube_length, 0.0f, 0.0f,
	};

	glm::uvec3 indices[] = {
		glm::uvec3{0, 1, 2},
		glm::uvec3{0, 2, 3},

		glm::uvec3{4, 5, 6},
		glm::uvec3{4, 6, 7},

		glm::uvec3{8, 9, 10},
		glm::uvec3{8, 10, 11},

		glm::uvec3{12, 13, 14},
		glm::uvec3{12, 14, 15},

		glm::uvec3{16, 17, 18},
		glm::uvec3{16, 18, 19},

		glm::uvec3{20, 21, 22},
		glm::uvec3{20, 22, 23},
	};

	constexpr float kVertFactor = 6.0 / 4.0;
	size_t number_vertices = static_cast<size_t>(kVertFactor * sizeof(points) / sizeof(glm::vec3));

	const char *vertex_shader = "#version 450\n"
		"layout (location = 0) in vec3 vert_pos;"
		"layout (location = 1) in vec2 tex_pos;"
		"out vec2 tex_pos_frag;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"void main () {"
		"  gl_Position = projection * view * model * vec4(vert_pos.xyz, 1.0f);"
		"  tex_pos_frag = tex_pos;"
		"}";

	const char *fragment_shader = "#version 450\n"
		"in vec2 tex_pos_frag;"
		"out vec4 frag_color;"
		"layout (binding = 0) uniform sampler2D tex_sampler;"
		"void main () {"
		"  frag_color = texture(tex_sampler, tex_pos_frag);"
		"}";

	if (!glfwInit()) {
		std::cerr << "ERROR: COULD NOT START GLFW3" << std::endl;
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	mode = glfwGetVideoMode(monitor);

	win_width = mode->width;
	win_height = mode->height;
	ratio = (float)win_width / win_height;

	window = glfwCreateWindow(win_width, win_height, kWindowTitle.c_str(), NULL, NULL);
	if (!window) {
		std::cerr << "ERROR: COULD NOT OPEN WINDOW WITH GLFW3" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, win_width / 2.0, win_height / 2.0f);

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	irrklang::ISoundEngine *sound_engine = irrklang::createIrrKlangDevice();
	sound_engine->play2D("../audio/zelda.mp3", GL_TRUE);

	renderer = glGetString(GL_RENDERER);
	version = glGetString(GL_VERSION);
	std::cout << "RENDERER: " << renderer << std::endl;
	std::cout << "OPENGL VERSION SUPPORTED " << version << std::endl;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(points),
		points,
		GL_STATIC_DRAW
	);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		5 * sizeof(GLfloat),
		(const GLvoid*)0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		5 * sizeof(GLfloat),
		(const GLvoid*)(3 * sizeof(GLfloat))
	);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		(GLsizeiptr)sizeof(indices),
		(const GLvoid*)indices,
		GL_STATIC_DRAW
	);

	crate_cube.LoadTexture("../textures/crate.jpg");
	floor.LoadTexture("../textures/sand.jpg");

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	prog_handle = glCreateProgram();
	glAttachShader(prog_handle, fs);
	glAttachShader(prog_handle, vs);
	glLinkProgram(prog_handle);

	glDeleteShader(vs);
	glDeleteShader(fs);

	glUseProgram(prog_handle);

	model = glm::translate(glm::mat4{}, cube_pos);
	glUniformMatrix4fv(
		glGetUniformLocation(prog_handle, "model"),
		1,
		GL_FALSE,
		(const GLfloat*)glm::value_ptr(model)
	);

	// perspective / frustum
	// initialize projection matrix
	proj = glm::perspective(glm::radians(camera::fps_camera.GetFov()), ratio, near, far);

	glUniformMatrix4fv(
		glGetUniformLocation(prog_handle, "projection"),
		1,
		GL_FALSE,
		(const GLfloat*)glm::value_ptr(proj));

	// set key callback function
	glfwSetKeyCallback(
		window,
		[](GLFWwindow* win, int key, int scancode, int action, int mode) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(win, 1);
		}
	});

	// set scroll callback function
	glfwSetScrollCallback(window, [](GLFWwindow* win, double xoffset, double yoffset) {
		double fov = camera::fps_camera.GetFov() + yoffset * camera::kZoomSensitivity;
		fov = glm::clamp(fov, 1.0, 120.0);
		camera::fps_camera.SetFov(fov);

		proj = glm::perspective(
			glm::radians(camera::fps_camera.GetFov()),
			ratio,
			near,
			far
		);
	});

	// set framebuffer size callback function
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
		glViewport(0, 0, width, height);
		win_width = width;
		win_height = height;
		ratio = (float)win_width / win_height;

		proj = glm::perspective(
			glm::radians(camera::fps_camera.GetFov()),
			ratio,
			near,
			far
		);

		glUniformMatrix4fv(
			glGetUniformLocation(prog_handle, "projection"),
			1,
			GL_FALSE,
			(const GLfloat*)glm::value_ptr(proj));
	});

	// initialization
	glViewport(0, 0, win_width, win_height);
	glClearColor(rgb.r, rgb.g, rgb.b, rgb.a);
	double previous_time{ glfwGetTime() }, delta_time{};

	while (!glfwWindowShouldClose(window)) {
		delta_time = glfwGetTime() - previous_time;
		previous_time = glfwGetTime();
		Update(delta_time);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(prog_handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		crate_cube.BindTextureUnit();
		model = glm::translate(glm::mat4{}, cube_pos);
		view = camera::fps_camera.GetViewMatrix();

		glUniformMatrix4fv(
			glGetUniformLocation(prog_handle, "model"),
			1,
			GL_FALSE,
			(const GLfloat*)glm::value_ptr(model)
		);

		glUniformMatrix4fv(
			glGetUniformLocation(prog_handle, "view"),
			1,
			GL_FALSE,
			(const GLfloat*)glm::value_ptr(view)
		);

		glUniformMatrix4fv(
			glGetUniformLocation(prog_handle, "projection"),
			1,
			GL_FALSE,
			(const GLfloat*)glm::value_ptr(proj)
		);

		glBindVertexArray(vao);

		glDrawElements(
			GL_TRIANGLES,
			number_vertices,
			GL_UNSIGNED_INT,
			0
		);

		floor.BindTextureUnit();

		model = glm::translate(glm::mat4{}, glm::vec3{ 0.0f, -half_cube_length, 0.0f });
		model = glm::scale(model, glm::vec3{ 30.0f, 0.01f, 30.0f });
		glUniformMatrix4fv(
			glGetUniformLocation(prog_handle, "model"),
			1,
			GL_FALSE,
			(const GLfloat*)glm::value_ptr(model)
		);

		glDrawElements(
			GL_TRIANGLES,
			number_vertices,
			GL_UNSIGNED_INT,
			0
		);

		glBindVertexArray(0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);

	glfwTerminate();
	return 0;
}

void Update(double elapsed_time) {
	// Camera orientation
	double mouseX, mouseY;

	// Get the current mouse cursor position delta
	glfwGetCursorPos(window, &mouseX, &mouseY);

	// Rotate the camera the difference in mouse distance from the center screen.  Multiply this delta by a speed scaler
	camera::fps_camera.Rotate((float)(win_width / 2.0 - mouseX) * camera::kMouseSensitivity, (float)(win_height / 2.0 - mouseY) * camera::kMouseSensitivity);

	// Clamp mouse cursor to center of screen
	glfwSetCursorPos(window, win_width / 2.0, win_height / 2.0);

	// Camera FPS movement

	// Forward/backward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera::fps_camera.Move(camera::kMoveSpeed * (float)elapsed_time * camera::fps_camera.GetLook());
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera::fps_camera.Move(camera::kMoveSpeed * (float)elapsed_time * -camera::fps_camera.GetLook());

	// Strafe left/right
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera::fps_camera.Move(camera::kMoveSpeed * (float)elapsed_time * -camera::fps_camera.GetRight());
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera::fps_camera.Move(camera::kMoveSpeed * (float)elapsed_time * camera::fps_camera.GetRight());
}
