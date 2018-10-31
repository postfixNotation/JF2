#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_normalized_axis.hpp>

#include <SFML/Audio.hpp>
#include <Box2D/Box2D.h>

#include <jf2.hpp>

float near = 0.1f;
float far = 100.0f;

glm::mat4 projection{}, model_mat{}, view{};

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

int main(int argc, const char **argv) {
	FileSystem::SetResourceRootDir("Resources");
	FileSystem::InitSubDirs( {"fonts", "audio", "models", "shader", "textures"} );

	context::Config config;
	config.opengl_major = 3;
	config.opengl_minor = 3;
	config.number_of_samples = 4;
	config.debug_context = true;
	config.cusor_enabled = false;
	config.context_title = "JF2 - Rendering Engine";
	config.context_size = context::Size::DEBUG;

	Context::Instance().Create(config);
	Context::Instance().SetCursorPos(
		Context::Instance().GetWidth() / 2,
		Context::Instance().GetHeight() / 2);
	opengl::Init();
	opengl::SetDefaultSetting();
	opengl::SetViewport(
		0,
		0,
		Context::Instance().GetWidth(),
		Context::Instance().GetHeight());
	opengl::SetColor(1.0f, 0.9f, 0.8f, 1.0f);

	InputHandler input_handler;
	input_handler.Init();

	//GLint flags;
	//glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	//if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
	//	glEnable(GL_DEBUG_OUTPUT);
	//	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	//	glDebugMessageCallback(
	//		opengl::DebugMessageCallback,
	//		nullptr);

	//	glDebugMessageControl(
	//		GL_DONT_CARE,
	//		GL_DONT_CARE,
	//		GL_DONT_CARE,
	//		0,
	//		nullptr,
	//		GL_TRUE
	//	);
	//}

	SetCallbacks();

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

	//cubemap_shader->SetInt("skybox", 0);

	ResourceManager::LoadShader(
		FileSystem::GetPathString("shader")+"mesh_vert_shader.glsl",
		FileSystem::GetPathString("shader")+"mesh_frag_shader.glsl",
		"model");

	std::shared_ptr<MeshRenderer> model;
	model = std::make_shared<MeshRenderer>(ResourceManager::GetShader("model"));
	model->Load(FileSystem::GetPathString("models")+"cyborg.obj", false);

	std::shared_ptr<Texture2D> texture;
	texture = std::make_shared<Texture2D>(ResourceManager::GetShader("model"));
	texture->Load(FileSystem::GetPathString("textures")+"cyborg_diffuse.png");

	ResourceManager::GetShader("model")->SetMat4("model", model_mat);

	ResourceManager::LoadShader(
		FileSystem::GetPathString("shader")+"font_vert.glsl",
		FileSystem::GetPathString("shader")+"font_frag.glsl",
		"text");
	ResourceManager::LoadShader(
		FileSystem::GetPathString("shader")+"sprite_vert_shader.glsl",
		FileSystem::GetPathString("shader")+"sprite_frag_shader.glsl",
		"sprite");
	ResourceManager::LoadTextRenderer(
		ResourceManager::GetShader("text"),
		Context::Instance().GetWidth(),
		Context::Instance().GetHeight(),
		64,
		FileSystem::GetPathString("fonts")+"Nosifer-Regular.ttf",
		"Nosifier");
	ResourceManager::LoadTextRenderer(
		ResourceManager::GetShader("text"),
		Context::Instance().GetWidth(),
		Context::Instance().GetHeight(),
		32,
		FileSystem::GetPathString("fonts")+"PermanentMarker-Regular.ttf",
		"PermanentMarker");
	ResourceManager::LoadTextRenderer(
		ResourceManager::GetShader("text"),
		Context::Instance().GetWidth(),
		Context::Instance().GetHeight(),
		20,
		FileSystem::GetPathString("fonts")+"Wallpoet-Regular.ttf",
		"Wallpoet");
	ResourceManager::LoadTexture(
		ResourceManager::GetShader("sprite"),
		FileSystem::GetPathString("textures")+"tux.png",
		"tux");
	std::shared_ptr<SpriteRenderer> first_sprite = std::make_shared<SpriteRenderer>(
		ResourceManager::GetShader("sprite"),
		Context::Instance().GetWidth(),
		Context::Instance().GetHeight());

	double previous_time{ glfwGetTime() }, delta_time{};
	//camera::orbit_camera.SetLookAt(glm::vec3{ 0.0f,0.0f,0.0f });

	while (!Context::Instance()) {
		Update(glfwGetTime() - previous_time);
		previous_time = glfwGetTime();

		Renderer::Clear();

		//view = camera::orbit_camera.GetViewMatrix();
		view = camera::fps_camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera::fps_camera.GetFov()), Context::Instance().GetRatio(), near, far);

		ResourceManager::GetShader("model")->SetMat4("view", view);
		ResourceManager::GetShader("model")->SetMat4("projection", projection);

		texture->Bind("tex_sampler", 0);
		model->Draw();
		texture->Unbind(0);

		ResourceManager::GetTextRenderer("Wallpoet")->Draw(
			"Framerate: "+std::to_string(Context::Instance().GetFrameRate(2)),
			0.0f,
			0.0f,
			1.2f,
			glm::vec3{ .3f,.7f,.6f }
		);
		//first_sprite->Draw(
		//	ResourceManager::GetTexture("tux"),
		//	glm::vec2{ 0.0f, 0.0f },
		//	glm::vec2{ 160.0f, 160.0f }
		//);

		//glDepthFunc(GL_LEQUAL);

		//view = glm::mat4(glm::mat3(camera::fps_camera.GetViewMatrix()));

		//cubemap_shader->SetMat4("view", view);
		//cubemap_shader->SetMat4("projection", projection);

		//glFrontFace(GL_CW);

		//textures[1]->BindCubeTextureUnit("skybox", 0);
		//meshes[1]->Draw();
		//textures[1]->UnbindCubeTextureUnit(0);

		//glFrontFace(GL_CCW);
		//glDepthFunc(GL_LESS);

		input_handler.HandleInput();
		Context::Instance().PollEvents();
		Context::Instance().SwapBuffers();
	}

	Context::Instance().Terminate();
	return 0;
}

void Update(double elapsed_time) {
	double mouse_x, mouse_y;

	glfwGetCursorPos(Context::Instance().Get(), &mouse_x, &mouse_y);
	camera::fps_camera.Rotate(
		static_cast<float>(Context::Instance().GetWidth() / 2.0 - mouse_x) * camera::kMouseSensitivity,
		static_cast<float>(Context::Instance().GetHeight() / 2.0 - mouse_y) * camera::kMouseSensitivity
	);
	glfwSetCursorPos(
		Context::Instance().Get(),
		Context::Instance().GetWidth() / 2.0,
		Context::Instance().GetHeight() / 2.0
	);

	if (glfwGetKey(Context::Instance().Get(), GLFW_KEY_W) == GLFW_PRESS)
		camera::fps_camera.Move(
			camera::kMoveSpeed * static_cast<float>(elapsed_time) * camera::fps_camera.GetLook()
		);
	else if (glfwGetKey(Context::Instance().Get(), GLFW_KEY_S) == GLFW_PRESS)
		camera::fps_camera.Move(
			camera::kMoveSpeed * static_cast<float>(elapsed_time) * -camera::fps_camera.GetLook()
		);

	if (glfwGetKey(Context::Instance().Get(), GLFW_KEY_A) == GLFW_PRESS)
		camera::fps_camera.Move(
			camera::kMoveSpeed * static_cast<float>(elapsed_time) * -camera::fps_camera.GetRight()
		);
	else if (glfwGetKey(Context::Instance().Get(), GLFW_KEY_D) == GLFW_PRESS)
		camera::fps_camera.Move(
			camera::kMoveSpeed * static_cast<float>(elapsed_time) * camera::fps_camera.GetRight()
		);
}

void SetCallbacks() {
	glfwSetScrollCallback(Context::Instance().Get(), [](GLFWwindow* win, double xoffset, double yoffset) {
		//	// FPS camera
		if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
			double fov = camera::fps_camera.GetFov() + yoffset * camera::kZoomSensitivity;
			fov = glm::clamp(fov, 1.0, 120.0);
			camera::fps_camera.SetFov(static_cast<float>(fov));

			projection = glm::perspective(
				glm::radians(camera::fps_camera.GetFov()),
				Context::Instance().GetRatio(),
				near,
				far
			);
		}
		// Orbit camera
		else if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			camera::orbit_camera.SetRadius(static_cast<float>(yoffset) * camera::kZoomSensitivity);
		}
	});

	////// Orbit camera
	//glfwSetCursorPosCallback(Context::Instance(), [](GLFWwindow* win, double xpos, double ypos) {
	//	static glm::vec2 last_mouse_pos = glm::vec2{};

	//	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
	//		camera::yaw -= (static_cast<float>(xpos) - last_mouse_pos.x) * camera::kMouseSensitivity;
	//		camera::pitch += (static_cast<float>(ypos) - last_mouse_pos.y) * camera::kMouseSensitivity;
	//		camera::orbit_camera.Rotate(camera::yaw, camera::pitch);
	//	}

	//	last_mouse_pos = glm::vec2{ static_cast<float>(xpos), static_cast<float>(ypos) };
	//});

	glfwSetFramebufferSizeCallback(Context::Instance().Get(), [](GLFWwindow* win, int width, int height) {
		glViewport(0, 0, width, height);
		Context::Instance().UpdateDimensions();
			projection = glm::perspective(
				glm::radians(camera::fps_camera.GetFov()),
				Context::Instance().GetRatio(),
				near,
				far
			);

			ResourceManager::GetShader("model")->SetMat4("projection", projection);
	});
}
