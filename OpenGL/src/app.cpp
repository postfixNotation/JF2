#include <iostream>
#include <sstream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_normalized_axis.hpp>

#include <SFML/Audio.hpp>

#include <Box2D/Box2D.h>

#include <resource_manager.hpp>
#include <sprite_renderer.hpp>
#include <mesh_renderer.hpp>
#include <text_renderer.hpp>
#include <texture2d.hpp>
#include <camera.hpp>
#include <shader.hpp>

float ratio;
float near = 0.1f;
float far = 100.0f;

glm::mat4 proj{}, model{}, view{};
const GLFWvidmode* mode;

GLint win_width, win_height;
std::string kWindowTitle = "Prototype Application";
GLFWwindow *window = nullptr;

//const GLubyte *renderer;
//const GLubyte *version;

Color rgb = { 0.0f / 255, 117.0f / 255, 153.0f / 255, 1.0f };

void Update(double);
void APIENTRY DebugMessageCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);
void SetCallbacks();

namespace camera {
	FPSCamera fps_camera{ glm::vec3{0.0f, 0.0f, 20.0f} };
	OrbitCamera orbit_camera{};
	float yaw{ 0.0f };
	float pitch{ 0.0f };
	float radius{ 10.0f };
	constexpr float kZoomSensitivity = -2.0f;
	constexpr float kMoveSpeed = 5.0f;
	constexpr float kMouseSensitivity = 0.1f;
}

int main() {
	if (!glfwInit()) {
		std::cerr << "ERROR: COULD NOT START GLFW3" << std::endl;
		return 1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	mode = glfwGetVideoMode(monitor);

	win_width = static_cast<GLint>(mode->width * (2.0f / 3));
	win_height = static_cast<GLint>(mode->height * (2.0f / 3));
	ratio = static_cast<float>(win_width) / win_height;

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, win_width / 2.0, win_height / 2.0f);

	glfwMakeContextCurrent(window);
	Shader::Init();

	//model_shader = std::make_shared<Shader>(
	//	"../shader/mesh_vert_shader.glsl",
	//	"../shader/mesh_frag_shader.glsl"
	//);
	//cubemap_shader = std::make_shared<Shader>(
	//	"../shader/cubemap_vert_shader.glsl",
	//	"../shader/cubemap_frag_shader.glsl"
	//);

	//std::string filename{};
	//GLFWimage images[2];
	//int width{}, height{}, components{};
	//const size_t kBytesPerValue{ 4 };
	//unsigned char *image_data{};

	//filename = "../textures/android_1.png";
	//image_data = stbi_load(
	//	filename.c_str(),
	//	&width,
	//	&height,
	//	&components,
	//	0
	//);
	//if (image_data == NULL) return 1;

	//images[0].width = width;
	//images[0].height = height;
	//images[0].pixels = image_data;

	//filename = "../textures/android_2.png";
	//image_data = stbi_load(
	//	filename.c_str(),
	//	&width,
	//	&height,
	//	&components,
	//	0
	//);
	//if (image_data == NULL) return 1;

	//images[1].width = width;
	//images[1].height = height;
	//images[1].pixels = image_data;

	//glfwSetWindowIcon(window, 2, images);

	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(
			DebugMessageCallback,
			nullptr);
		glDebugMessageControl(
			GL_DONT_CARE,
			GL_DONT_CARE,
			GL_DONT_CARE,
			0,
			nullptr,
			GL_TRUE
		);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	//renderer = glGetString(GL_RENDERER);
	//version = glGetString(GL_VERSION);

	//std::cout << "RENDERER: " << renderer << std::endl;
	//std::cout << "OPENGL VERSION SUPPORTED " << version << std::endl;

	//sf::Music music;
	//if (!music.openFromFile("../audio/throne.ogg"))
	//	return -1;
	//music.play();

	//std::vector<std::shared_ptr<MeshRenderer>> meshes(2);
	//std::vector<std::shared_ptr<Texture2D>> textures(3);

	//meshes[0] = std::make_shared<MeshRenderer>(model_shader);
	//meshes[0]->LoadObj("../models/robot.obj", ObjLoadingType::TRIANGLES);
	//textures[0] = std::make_shared<Texture2D>(model_shader);
	//textures[0]->LoadTexture("../textures/robot.jpg");

	//std::vector<std::string> faces{
	//	"../textures/skybox/right.jpg",
	//	"../textures/skybox/left.jpg",
	//	"../textures/skybox/bottom.jpg",
	//	"../textures/skybox/top.jpg",
	//	"../textures/skybox/front.jpg",
	//	"../textures/skybox/back.jpg"
	//};
	//meshes[1] = std::make_shared<MeshRenderer>(cubemap_shader);
	//meshes[1]->LoadObj("../models/cube.obj", ObjLoadingType::QUADS);
	//textures[1] = std::make_shared<Texture2D>(cubemap_shader);
	//textures[1]->LoadCubemap(faces);

	//model_shader->SetMat4("model", model);
	//cubemap_shader->SetInt("skybox", 0);

	SetCallbacks();

	glViewport(0, 0, win_width, win_height);
	glClearColor(rgb.r, rgb.g, rgb.b, rgb.a);
	//double previous_time{ glfwGetTime() }, delta_time{};

	//camera::orbit_camera.SetLookAt(glm::vec3{ 0.0f,0.0f,0.0f });

	ResourceManager::LoadShader(
		"../shader/font_vert.glsl",
		"../shader/font_frag.glsl",
		"text"
	);
	ResourceManager::LoadShader(
		"../shader/sprite_vert_shader.glsl",
		"../shader/sprite_frag_shader.glsl",
		"sprite"
	);
	ResourceManager::LoadTextRenderer(
		ResourceManager::GetShader("text"),
		static_cast<size_t>(win_width),
		static_cast<size_t>(win_height),
		64,
		"../fonts/Nosifer-Regular.ttf",
		"Nosifier"
	);
	ResourceManager::LoadTextRenderer(
		ResourceManager::GetShader("text"),
		static_cast<size_t>(win_width),
		static_cast<size_t>(win_height),
		32,
		"../fonts/PermanentMarker-Regular.ttf",
		"PermanentMarker"
	);
	ResourceManager::LoadTexture(ResourceManager::GetShader("sprite"), "../textures/tux.png", "tux");
	
	std::shared_ptr<Texture2D> texture = ResourceManager::GetTexture("tux");
	std::shared_ptr<SpriteRenderer> first_sprite = std::make_shared<SpriteRenderer>(
		ResourceManager::GetShader("sprite"),
		win_width,
		win_height
	);

	while (!glfwWindowShouldClose(window)) {
		//Update(glfwGetTime() - previous_time);
		//previous_time = glfwGetTime();

		Renderer::Clear();

		//view = camera::orbit_camera.GetViewMatrix();
		//view = camera::fps_camera.GetViewMatrix();
		//proj = glm::perspective(glm::radians(camera::fps_camera.GetFov()), ratio, near, far);

		//model_shader->SetMat4("view", view);
		//model_shader->SetMat4("projection", proj);

		//textures[0]->BindTextureUnit("tex_sampler", 0);
		//meshes[0]->Draw();
		//textures[0]->UnbindTextureUnit(0);

		first_sprite->Draw(
			ResourceManager::GetTexture("tux"),
			glm::vec2{0.0f, 0.0f},
			glm::vec2{160.0f, 160.0f}
		);
		ResourceManager::GetTextRenderer("Nosifier")->Draw(
			"Welcome to OpenGL ©",
			0.0f,
			static_cast<GLfloat>(win_height) / 2.0f,
			1.2f,
			glm::vec3{ .3f,.7f,.6f }
		);
		ResourceManager::GetTextRenderer("PermanentMarker")->Draw(
			"This is awesome!",
			0.0f,
			static_cast<GLfloat>(win_height) / 3.0f,
			1.2f,
			glm::vec3{ .6f,.8f,.9f }
		);

		//glDepthFunc(GL_LEQUAL);

		//view = glm::mat4(glm::mat3(camera::fps_camera.GetViewMatrix()));

		//cubemap_shader->SetMat4("view", view);
		//cubemap_shader->SetMat4("projection", proj);

		//glFrontFace(GL_CW);

		//textures[1]->BindCubeTextureUnit("skybox", 0);
		//meshes[1]->Draw();
		//textures[1]->UnbindCubeTextureUnit(0);

		//glFrontFace(GL_CCW);
		//glDepthFunc(GL_LESS);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void APIENTRY DebugMessageCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam) {

	// IGNORE NON-SIGNIFICANT ERROR/WARNING CODES
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << std::endl;
	std::cout << "DEBUG MESSAGE (" << id << "): " << message << std::endl;

	switch (source) {
	case GL_DEBUG_SOURCE_API:					std::cout << "SOURCE: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		std::cout << "SOURCE: WINDOW SYSTEM"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	std::cout << "SOURCE: SHADER COMPILER"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:		std::cout << "SOURCE: THIRD PARTY"; break;
	case GL_DEBUG_SOURCE_APPLICATION:		std::cout << "SOURCE: APPLICATION"; break;
	case GL_DEBUG_SOURCE_OTHER:				std::cout << "SOURCE: OTHER"; break;
	} std::cout << std::endl;

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:					std::cout << "TYPE: ERROR"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:std::cout << "TYPE: DEPRECATED BEHAVIOUR"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	std::cout << "TYPE: UNDEFINED BEHAVIOUR"; break;
	case GL_DEBUG_TYPE_PORTABILITY:			std::cout << "TYPE: PORTABILITY"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:			std::cout << "TYPE: PERFORMANCE"; break;
	case GL_DEBUG_TYPE_MARKER:					std::cout << "TYPE: MARKER"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:			std::cout << "TYPE: PUSH GROUP"; break;
	case GL_DEBUG_TYPE_POP_GROUP:				std::cout << "TYPE: POP GROUP"; break;
	case GL_DEBUG_TYPE_OTHER:					std::cout << "TYPE: OTHER"; break;
	} std::cout << std::endl;

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:				std::cout << "SEVERITY: HIGH"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:			std::cout << "SEVERITY: MEDIUM"; break;
	case GL_DEBUG_SEVERITY_LOW:				std::cout << "SEVERITY: LOW"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	std::cout << "SEVERITY: NOTIFICATION"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

void Update(double elapsed_time) {
	double mouse_x, mouse_y;

	glfwGetCursorPos(window, &mouse_x, &mouse_y);
	camera::fps_camera.Rotate(
		static_cast<float>(win_width / 2.0 - mouse_x) * camera::kMouseSensitivity,
		static_cast<float>(win_height / 2.0 - mouse_y) * camera::kMouseSensitivity
	);
	glfwSetCursorPos(
		window,
		win_width / 2.0,
		win_height / 2.0
	);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera::fps_camera.Move(
			camera::kMoveSpeed * static_cast<float>(elapsed_time) * camera::fps_camera.GetLook()
		);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera::fps_camera.Move(
			camera::kMoveSpeed * static_cast<float>(elapsed_time) * -camera::fps_camera.GetLook()
		);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera::fps_camera.Move(
			camera::kMoveSpeed * static_cast<float>(elapsed_time) * -camera::fps_camera.GetRight()
		);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera::fps_camera.Move(
			camera::kMoveSpeed * static_cast<float>(elapsed_time) * camera::fps_camera.GetRight()
		);
}

void SetCallbacks() {
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

	//glfwSetScrollCallback(window, [](GLFWwindow* win, double xoffset, double yoffset) {
	//	//	// FPS camera
	//	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
	//		double fov = camera::fps_camera.GetFov() + yoffset * camera::kZoomSensitivity;
	//		fov = glm::clamp(fov, 1.0, 120.0);
	//		camera::fps_camera.SetFov(static_cast<float>(fov));

	//		proj = glm::perspective(
	//			glm::radians(camera::fps_camera.GetFov()),
	//			ratio,
	//			near,
	//			far
	//		);
	//	}
	//	// Orbit camera
	//	else if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
	//		camera::orbit_camera.SetRadius(static_cast<float>(yoffset) * camera::kZoomSensitivity);
	//	}
	//});

	////// Orbit camera
	//glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
	//	static glm::vec2 last_mouse_pos = glm::vec2{};

	//	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
	//		camera::yaw -= (static_cast<float>(xpos) - last_mouse_pos.x) * camera::kMouseSensitivity;
	//		camera::pitch += (static_cast<float>(ypos) - last_mouse_pos.y) * camera::kMouseSensitivity;
	//		camera::orbit_camera.Rotate(camera::yaw, camera::pitch);
	//	}

	//	last_mouse_pos = glm::vec2{ static_cast<float>(xpos), static_cast<float>(ypos) };
	//});

	//glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
	//	glViewport(0, 0, width, height);
	//	win_width = width;
	//	win_height = height;
	//	ratio = (float)win_width / win_height;

	//	proj = glm::perspective(
	//		glm::radians(camera::fps_camera.GetFov()),
	//		ratio,
	//		near,
	//		far
	//	);

	//	model_shader->SetMat4("projection", proj);
	//});
}
