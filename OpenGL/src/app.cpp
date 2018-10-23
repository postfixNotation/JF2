#include <sstream>
#include <sstream>
#include <vector>

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
#include <context.hpp>
#include <camera.hpp>
#include <shader.hpp>
#include <gl.hpp>

float near = 0.1f;
float far = 100.0f;

glm::mat4 proj{}, model{}, view{};

void Update(double);
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

Context window;
int main() {
	context::Config config;
	config.opengl_major = 3;
	config.opengl_minor = 3;
	config.number_of_samples = 4;
	config.debug_context = true;
	config.cusor_enabled = false;
	config.context_title = "JF2 - Rendering Engine";
	config.context_size = context::Size::DEBUG;

	window.Create(config);
	window.SetCursorPos(window.GetWidth() / 2, window.GetHeight() / 2);
	opengl::Init();
	opengl::SetDefaultSetting();
	opengl::SetViewport(0, 0, window.GetWidth(), window.GetHeight());
	opengl::SetColor(0.7f, 0.9f, 0.8f, 1.0f);

	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(
			opengl::DebugMessageCallback,
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

	SetCallbacks();

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
		window.GetWidth(),
		window.GetHeight(),
		64,
		"../fonts/Nosifer-Regular.ttf",
		"Nosifier"
	);
	ResourceManager::LoadTextRenderer(
		ResourceManager::GetShader("text"),
		window.GetWidth(),
		window.GetHeight(),
		32,
		"../fonts/PermanentMarker-Regular.ttf",
		"PermanentMarker"
	);
	ResourceManager::LoadTextRenderer(
		ResourceManager::GetShader("text"),
		window.GetWidth(),
		window.GetHeight(),
		20,
		"../fonts/Wallpoet-Regular.ttf",
		"Wallpoet"
	);
	ResourceManager::LoadTexture(
		ResourceManager::GetShader("sprite"), "../textures/tux.png", "tux"
	);
	std::shared_ptr<SpriteRenderer> first_sprite = std::make_shared<SpriteRenderer>(
		ResourceManager::GetShader("sprite"),
		window.GetWidth(),
		window.GetHeight()
	);

	while (!window) {
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
			static_cast<GLfloat>(window.GetHeight()) / 2,
			1.2f,
			glm::vec3{ .3f,.7f,.6f }
		);
		ResourceManager::GetTextRenderer("Wallpoet")->Draw(
			std::to_string(window.GetFrameRate(2)),
			0.0f,
			static_cast<GLfloat>(window.GetHeight()) / 3.0f,
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

		window.PollEvents();
		window.SwapBuffers();
	}

	window.Terminate();
	return 0;
}

void Update(double elapsed_time) {
	//double mouse_x, mouse_y;

	//glfwGetCursorPos(window.Get(), &mouse_x, &mouse_y);
	//camera::fps_camera.Rotate(
	//	static_cast<float>(window.GetWidth() / 2.0 - mouse_x) * camera::kMouseSensitivity,
	//	static_cast<float>(window.GetHeight() / 2.0 - mouse_y) * camera::kMouseSensitivity
	//);
	//glfwSetCursorPos(
	//	window.Get(),
	//	window.GetWidth() / 2.0,
	//	window.GetHeight() / 2.0
	//);

	//if (glfwGetKey(window.Get(), GLFW_KEY_W) == GLFW_PRESS)
	//	camera::fps_camera.Move(
	//		camera::kMoveSpeed * static_cast<float>(elapsed_time) * camera::fps_camera.GetLook()
	//	);
	//else if (glfwGetKey(window.Get(), GLFW_KEY_S) == GLFW_PRESS)
	//	camera::fps_camera.Move(
	//		camera::kMoveSpeed * static_cast<float>(elapsed_time) * -camera::fps_camera.GetLook()
	//	);

	//if (glfwGetKey(window.Get(), GLFW_KEY_A) == GLFW_PRESS)
	//	camera::fps_camera.Move(
	//		camera::kMoveSpeed * static_cast<float>(elapsed_time) * -camera::fps_camera.GetRight()
	//	);
	//else if (glfwGetKey(window.Get(), GLFW_KEY_D) == GLFW_PRESS)
	//	camera::fps_camera.Move(
	//		camera::kMoveSpeed * static_cast<float>(elapsed_time) * camera::fps_camera.GetRight()
	//	);
}

void SetCallbacks() {
	glfwSetKeyCallback(
		window.Get(),
		[](GLFWwindow* win, int key, int scancode, int action, int mode) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			window.SetCloseFlag();
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

	glfwSetFramebufferSizeCallback(window.Get(), [](GLFWwindow* win, int width, int height) {
		glViewport(0, 0, width, height);
		window.UpdateDimensions();
	//	proj = glm::perspective(
	//		glm::radians(camera::fps_camera.GetFov()),
	//		ratio,
	//		near,
	//		far
	//	);

	//	model_shader->SetMat4("projection", proj);
	});
}
