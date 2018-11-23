#include "sandbox_app.hpp"

#define FPS 1
#define PHONG 1
#define STARS 0

glm::mat4 Sandbox::projection;
std::unique_ptr<jf2::Camera> Sandbox::camera;

Sandbox::Sandbox() {}

Sandbox::~Sandbox() {
	jf2::Context::Instance().Terminate();
}

void Sandbox::Init() {
	camera =
#if FPS == 1
		std::make_unique<jf2::FPSCamera>(glm::fvec3{ 0.0f, 0.0f, 20.0f });
#else
		std::make_unique<jf2::OrbitCamera>();
#endif

	jf2::FileSystem::Instance().SetResourceRootDir("Resources");
	jf2::FileSystem::Instance().InitSubDirs({ "fonts", "audio", "models", "shader", "textures" });

	audio_list_.push_back(std::make_unique<jf2::Music>());
	audio_list_.push_back(std::make_unique<jf2::Sound>());

	jf2::context::Config config;
	config.opengl_major = 3;
	config.opengl_minor = 3;
	config.number_of_samples = 4;
	config.debug_context = true;
	config.cusor_enabled = false;
	config.context_title = "JF2 - Rendering Engine";
	config.context_size = jf2::context::Size::DEBUG;

	jf2::Context::Instance().Create(config);
	jf2::Context::Instance().SetCursorPos(
		static_cast<float>(jf2::Context::Instance().GetWidth() / 2),
		static_cast<float>(jf2::Context::Instance().GetHeight() / 2));
	jf2::Context::Instance().SetIcon(
		jf2::FileSystem::Instance().GetPathString("textures") + "tux.png");

	jf2::opengl::Init();
	jf2::opengl::SetDefaultSetting();
	jf2::opengl::SetViewport(
		0, 0,
		static_cast<GLsizei>(jf2::Context::Instance().GetWidth()),
		static_cast<GLsizei>(jf2::Context::Instance().GetHeight()));
	jf2::opengl::SetColor(
		179.0f / 255,
		255.0f / 255,
		179.0f / 255,
		1.0f);

	jf2::opengl::SetDebugMessageCallback(jf2::opengl::DebugMessageCallback);

#if STARS == 0
	std::vector<std::string> cubemap_textures{
		jf2::FileSystem::Instance().GetPathString("textures") + "skybox/right.jpg",
		jf2::FileSystem::Instance().GetPathString("textures") + "skybox/left.jpg",
		jf2::FileSystem::Instance().GetPathString("textures") + "skybox/top.jpg",
		jf2::FileSystem::Instance().GetPathString("textures") + "skybox/bottom.jpg",
		jf2::FileSystem::Instance().GetPathString("textures") + "skybox/front.jpg",
		jf2::FileSystem::Instance().GetPathString("textures") + "skybox/back.jpg"
	};
#else
	std::vector<std::string> cubemap_textures{
		jf2::FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		jf2::FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		jf2::FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		jf2::FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		jf2::FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		jf2::FileSystem::Instance().GetPathString("textures") + "stars.jpg" };
#endif

	jf2::ResourceManager::LoadShader(
		jf2::FileSystem::Instance().GetPathString("shader") + "cubemap_vert_shader.glsl",
		jf2::FileSystem::Instance().GetPathString("shader") + "cubemap_frag_shader.glsl",
		"cubemap");

	jf2::ResourceManager::LoadTexture(
		jf2::ResourceManager::GetShader("cubemap"),
		cubemap_textures,
		"faces");

#if PHONG == 0
	jf2::ResourceManager::LoadShader(
		jf2::FileSystem::Instance().GetPathString("shader") + "mesh_vert_shader.glsl",
		jf2::FileSystem::Instance().GetPathString("shader") + "mesh_frag_shader.glsl",
		"model");
#else
	jf2::ResourceManager::LoadShader(
		jf2::FileSystem::Instance().GetPathString("shader") + "mesh_vert_phong.glsl",
		jf2::FileSystem::Instance().GetPathString("shader") + "mesh_frag_phong.glsl",
		"model");
#endif
	projection = camera->GetProjectionMatrix(jf2::Context::Instance().GetRatio());

	jf2::ResourceManager::GetShader("model")->SetFloat("xoffset[0]", -10.0f);
	jf2::ResourceManager::GetShader("model")->SetFloat("xoffset[1]", -5.0f);
	jf2::ResourceManager::GetShader("model")->SetFloat("xoffset[2]", 0.0f);
	jf2::ResourceManager::GetShader("model")->SetFloat("xoffset[3]", 5.0f);
	jf2::ResourceManager::GetShader("model")->SetFloat("xoffset[4]", 10.0f);

	jf2::ResourceManager::GetShader("model")->SetMat4("u_model", model_);
	jf2::ResourceManager::GetShader("model")->SetMat4("u_projection", projection);

	jf2::ResourceManager::GetShader("model")->SetVec3("u_light_color", glm::fvec3{ 1.0f, 1.0f, 1.0f });
	jf2::ResourceManager::GetShader("model")->SetVec3("u_view_pos", camera->GetPosition());

	jf2::ResourceManager::GetShader("cubemap")->SetInt("skybox", 0);
	jf2::ResourceManager::GetShader("cubemap")->SetMat4("projection", projection);

	jf2::ResourceManager::LoadTexture(
		jf2::ResourceManager::GetShader("model"),
		jf2::FileSystem::Instance().GetPathString("textures") + "cyborg_diffuse.png",
		"cyborg");
	jf2::ResourceManager::LoadTexture(
		jf2::ResourceManager::GetShader("model"),
		jf2::FileSystem::Instance().GetPathString("textures") + "floor.jpg",
		"floor");

	jf2::ResourceManager::LoadShader(
		jf2::FileSystem::Instance().GetPathString("shader") + "font_vert.glsl",
		jf2::FileSystem::Instance().GetPathString("shader") + "font_frag.glsl",
		"text");

	jf2::ResourceManager::LoadShader(
		jf2::FileSystem::Instance().GetPathString("shader") + "sprite_vert_shader.glsl",
		jf2::FileSystem::Instance().GetPathString("shader") + "sprite_frag_shader.glsl",
		"sprite");

	jf2::ResourceManager::LoadTextRenderer(
		jf2::ResourceManager::GetShader("text"),
		jf2::Context::Instance().GetWidth(),
		jf2::Context::Instance().GetHeight(),
		20,
		jf2::FileSystem::Instance().GetPathString("fonts") + "Wallpoet-Regular.ttf",
		"Wallpoet");

	jf2::ResourceManager::LoadTexture(
		jf2::ResourceManager::GetShader("sprite"),
		jf2::FileSystem::Instance().GetPathString("textures") + "donut_icon.png",
		"donut");

	meshes_.push_back(std::make_unique<jf2::MeshRenderer>(jf2::ResourceManager::GetShader("model")));
	meshes_[0]->Load(
		jf2::FileSystem::Instance().GetPathString("models") + "cyborg.obj",
		false);
	meshes_.push_back(std::make_unique<jf2::MeshRenderer>(jf2::ResourceManager::GetShader("model")));
	meshes_[1]->Load(
		jf2::FileSystem::Instance().GetPathString("models") + "floor.obj",
		false);
	meshes_.push_back(std::make_unique<jf2::MeshRenderer>(jf2::ResourceManager::GetShader("cubemap")));
	meshes_[2]->Load(
		jf2::FileSystem::Instance().GetPathString("models") + "cube.obj");

	sprites_.push_back(std::make_unique<jf2::SpriteRenderer>(
		jf2::ResourceManager::GetShader("sprite"),
		jf2::Context::Instance().GetWidth(),
		jf2::Context::Instance().GetHeight()));

	audio_list_[0]->Open(jf2::FileSystem::Instance().GetPathString("audio") + "throne.ogg");
	audio_list_[0]->Play(true);
	audio_list_[0]->Volume(10.0f);
	audio_list_[0]->Pitch(1.2f);

	audio_list_[1]->Open(jf2::FileSystem::Instance().GetPathString("audio") + "powerup1.ogg");
	audio_list_[1]->Play();
}

void Sandbox::ProcessInput(float dt) {
	if (jf2::Context::Instance().KeyDown(jf2::KeyNum::KEY_ESCAPE))
		jf2::Context::Instance().SetCloseFlag();

	if (jf2::Context::Instance().KeyDown(jf2::KeyNum::KEY_E))
		jf2::opengl::PolygonMode();
	if (jf2::Context::Instance().KeyDown(jf2::KeyNum::KEY_Q))
		jf2::opengl::FillMode();

	if (jf2::Context::Instance().KeyDown(jf2::KeyNum::KEY_W))
		camera->HandleKeyboard(jf2::CameraMovement::FORWARD, dt);
	if (jf2::Context::Instance().KeyDown(jf2::KeyNum::KEY_S))
		camera->HandleKeyboard(jf2::CameraMovement::BACKWARD, dt);
	if (jf2::Context::Instance().KeyDown(jf2::KeyNum::KEY_A))
		camera->HandleKeyboard(jf2::CameraMovement::LEFT, dt);
	if (jf2::Context::Instance().KeyDown(jf2::KeyNum::KEY_D))
		camera->HandleKeyboard(jf2::CameraMovement::RIGHT, dt);

	if (jf2::Context::Instance().KeyDown(jf2::MouseButton::MOUSE_LEFT))
		audio_list_[1]->Play();
	if (jf2::Context::Instance().KeyDown(jf2::KeyNum::KEY_F))
		audio_list_[1]->Play();
	if (jf2::Context::Instance().KeyDown(jf2::KeyNum::KEY_R))
		audio_list_[1]->Open(jf2::FileSystem::Instance().GetPathString("audio") + "powerup1.ogg");
	if (jf2::Context::Instance().KeyDown(jf2::KeyNum::KEY_V))
		audio_list_[1]->Open(jf2::FileSystem::Instance().GetPathString("audio") + "powerup2.ogg");
}

void Sandbox::Render() {
	jf2::Renderer::Clear();

	light_position_.x = static_cast<float>(4 * sinf(jf2::Context::Instance().GetTime() * 3));
	light_position_.z = static_cast<float>(4 * cosf(jf2::Context::Instance().GetTime() * 3));

	view_ = camera->GetViewMatrix();

	jf2::ResourceManager::GetShader("model")->SetMat4("u_view", view_);
	jf2::ResourceManager::GetShader("model")->SetVec3("u_view_pos", camera->GetPosition());
	jf2::ResourceManager::GetShader("model")->SetVec3("u_light_pos", light_position_);

	jf2::ResourceManager::GetTexture("floor")->Bind("u_tex_sampler", 0);
	meshes_[1]->Draw(5);
	jf2::ResourceManager::GetTexture("floor")->Unbind(0);

	jf2::ResourceManager::GetTexture("cyborg")->Bind("u_tex_sampler", 0);
	meshes_[0]->Draw(5);
	jf2::ResourceManager::GetTexture("cyborg")->Unbind(0);

	view_ = camera->GetViewMatrix(false);
	jf2::ResourceManager::GetShader("cubemap")->SetMat4("view", view_);

	jf2::ResourceManager::GetTexture("faces")->Bind("skybox", 0);
	jf2::opengl::SetCubeMapMode();
	meshes_[2]->Draw();
	jf2::opengl::ResetCubeMapMode();
	jf2::ResourceManager::GetTexture("faces")->Unbind(0);

	jf2::ResourceManager::GetTextRenderer("Wallpoet")->Draw(
		"Framerate: " + std::to_string(jf2::Context::Instance().GetFrameRate(2)).substr(0, 5),
		0.0f,
		0.0f,
		1.2f,
		glm::fvec3{ 0.5f, 0.5f, 0.5f });

	sprites_[0]->Draw(
		jf2::ResourceManager::GetTexture("donut"),
		glm::fvec2{ jf2::Context::Instance().GetWidth() - 100.0f, jf2::Context::Instance().GetHeight() - 100.0f },
		{ glm::fvec2{ -100.0f, 0.0f }, glm::fvec2{ -200.0f, 0.0f } },
		glm::fvec2{ 100.0f, 100.0f });

	jf2::Context::Instance().PollEvents();
	jf2::Context::Instance().SwapBuffers();
}

void Sandbox::SetCallbacks() {
	glfwSetScrollCallback(jf2::Context::Instance().Get(), [](GLFWwindow* win, double xoffset, double yoffset) {
		camera->HandleScroll(static_cast<float>(yoffset));
#if FPS == 1
		projection = camera->GetProjectionMatrix(jf2::Context::Instance().GetRatio());
		jf2::ResourceManager::GetShader("model")->SetMat4("u_projection", projection);
		jf2::ResourceManager::GetShader("cubemap")->SetMat4("projection", projection);
#endif
	});

	glfwSetCursorPosCallback(jf2::Context::Instance().Get(), [](GLFWwindow* win, double xpos, double ypos) {
		camera->HandleMouseCursor(static_cast<float>(xpos), static_cast<float>(ypos));
	});

	glfwSetFramebufferSizeCallback(
		jf2::Context::Instance().Get(), [](GLFWwindow*, int width, int height) {
		jf2::opengl::SetViewport(0, 0, width, height);
	});
}

void Sandbox::Run() {
	while (!jf2::Context::Instance()) {
		ProcessInput(jf2::Context::Instance().GetTimePerFrame());
		Render();
	}
}

jf2::Application* jf2::CreateApplication() {
	return new Sandbox();
}
