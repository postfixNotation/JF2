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
glm::vec3 cube_pos{};
const GLFWvidmode* mode;
GLuint prog_handle;

namespace camera {
	OrbitCamera orbit_camera{};
	float yaw{ 0.0f };
	float pitch{ 0.0f };
	float radius{ 10.0f };
	constexpr float kMouseSensitivity = 0.25f;
	constexpr float kRadiusSensitivity = 0.00001f;
}

int main() {
	GLFWwindow *window = NULL;
	const GLubyte *renderer;
	const GLubyte *version;
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	GLuint vs, fs;

	Texture2D texture2d{}, floor{};

	struct Color {
		float r{ 0.7f };
		float g{ 0.7f };
		float b{ 0.7f };
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

	window = glfwCreateWindow(mode->width, mode->height, "Hello Triangle", NULL, NULL);
	if (!window) {
		std::cerr << "ERROR: COULD NOT OPEN WINDOW WITH GLFW3" << std::endl;
		glfwTerminate();
		return 1;
	}

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

	texture2d.LoadTexture("../textures/crate.jpg");
	floor.LoadTexture("../textures/grass.jpg");

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

	// perspective / frustum
	// initialize projection matrix
	float ratio = (float)mode->width / mode->height;
	float near = .1f;
	float far = 100.f;
	float fov = glm::radians(67.f);
	proj = glm::perspective(fov, ratio, near, far);

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
		else if (key = GLFW_KEY_F1 && action == GLFW_PRESS) {
			static size_t selection{ 0 };
			switch (selection++) {
			case 0:
				glPointSize(5);
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				break;
			case 1:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;
			case 2:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				selection = 0;
				break;
			}
		}
	});

	// set cursor callback function
	glfwSetCursorPosCallback(
		window,
		[](GLFWwindow* win, double xpos, double ypos) {
		static glm::vec2 last_mouse_pos{ 0.0f, 0.0f };

		if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			camera::pitch += ((float)ypos - last_mouse_pos.y) * camera::kMouseSensitivity;
			camera::yaw -= ((float)xpos - last_mouse_pos.x) * camera::kMouseSensitivity;
		}

		last_mouse_pos = glm::vec2{ xpos, ypos };
	});

	// set scroll callback function
	glfwSetScrollCallback(window, [](GLFWwindow* win, double xoffset, double yoffset) {
		camera::radius += yoffset;
	});

	// set framebuffer size callback function
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
		glViewport(0, 0, width, height);

		float ratio = (float)width / height;
		float near = .1f;
		float far = 100.f;
		float fov = glm::radians(67.f);
		proj = glm::perspective(fov, ratio, near, far);

		glUniformMatrix4fv(
			glGetUniformLocation(prog_handle, "projection"),
			1,
			GL_FALSE,
			(const GLfloat*)glm::value_ptr(proj));
	});

	// initialization
	glViewport(0, 0, mode->width, mode->height);
	glClearColor(rgb.r, rgb.g, rgb.b, rgb.a);

	double previous_time{ glfwGetTime() }, delta_time{};
	double angle{};

	while (!glfwWindowShouldClose(window)) {
		delta_time = glfwGetTime() - previous_time;
		previous_time = glfwGetTime();

		angle = (angle > 360) ? 0 : angle + delta_time * 30.0f;
		cube_pos.x = 8.0f * sinf(glm::radians(angle));
		cube_pos.z = 8.0f * cosf(glm::radians(angle));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(prog_handle);
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		texture2d.BindTextureUnit();

		camera::orbit_camera.SetLookAt(cube_pos);
		camera::orbit_camera.SetRadius(camera::radius);
		camera::orbit_camera.Rotate(camera::yaw, camera::pitch);

		model = glm::translate(glm::mat4{}, cube_pos);
		view = camera::orbit_camera.GetViewMatrix();

		glUniformMatrix4fv(
			glGetUniformLocation(prog_handle, "view"),
			1,
			GL_FALSE,
			(const GLfloat*)glm::value_ptr(view)
		);

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

		floor.BindTextureUnit();
		model = glm::scale(glm::mat4{}, glm::vec3{ 10.0f, 0.01f, 10.0f }) * glm::translate(glm::mat4{}, glm::vec3{ 0.0f, -half_cube_length * 100.0f, 0.0f });

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
