#include "sandbox_app.hpp"

#define FPS 1
#define PHONG 1
#define STARS 0
#define SKYBOX 1
#define DEV 0

glm::mat4 Sandbox::projection;
std::unique_ptr<nxt::Camera> Sandbox::camera;

Sandbox::Sandbox() {}

Sandbox::~Sandbox() {
	nxt::Context::Instance().Terminate();
}

void Sandbox::Init() {
	camera =
#if FPS == 1
		std::make_unique<nxt::FPSCamera>(glm::fvec3{ 0.0f, 0.0f, 20.0f });
#else
		std::make_unique<nxt::OrbitCamera>();
#endif

	nxt::FileSystem::Instance().SetResourceRootDir("Resources");
	nxt::FileSystem::Instance().InitSubDirs({ "fonts", "audio", "models", "shader", "textures" });

	audio_list_.push_back(std::make_unique<nxt::Music>());
	audio_list_.push_back(std::make_unique<nxt::Sound>());

	nxt::context::Config config;
	config.opengl_major = 3;
	config.opengl_minor = 3;
	config.number_of_samples = 4;
	config.debug_context = true;
	config.cusor_enabled = false;
	config.context_title = "nxt - Rendering Engine";
	config.context_size = nxt::context::Size::DEBUG;

	nxt::Context::Instance().Create(config);
	nxt::Context::Instance().SetCursorPos(
		static_cast<float>(nxt::Context::Instance().GetWidth() / 2),
		static_cast<float>(nxt::Context::Instance().GetHeight() / 2));
	nxt::Context::Instance().SetIcon(
		nxt::FileSystem::Instance().GetPathString("textures") + "tux.png");

	nxt::opengl::Init();
	nxt::opengl::SetDefaultSetting();
	nxt::opengl::SetViewport(
		0, 0,
		static_cast<GLsizei>(nxt::Context::Instance().GetWidth()),
		static_cast<GLsizei>(nxt::Context::Instance().GetHeight()));
	nxt::opengl::SetColor(
		179.0f / 255,
		255.0f / 255,
		179.0f / 255,
		1.0f);

	nxt::opengl::SetDebugMessageCallback(nxt::opengl::DebugMessageCallback);

#if (SKYBOX == 1) && (STARS == 1)
#error CHOOSE ONLY ONE CUBEMAP TEXTURE SET!
#endif

#if SKYBOX == 1
	std::vector<std::string> cubemap_textures{
		nxt::FileSystem::Instance().GetPathString("textures") + "skybox/right.jpg",
		nxt::FileSystem::Instance().GetPathString("textures") + "skybox/left.jpg",
		nxt::FileSystem::Instance().GetPathString("textures") + "skybox/top.jpg",
		nxt::FileSystem::Instance().GetPathString("textures") + "skybox/bottom.jpg",
		nxt::FileSystem::Instance().GetPathString("textures") + "skybox/front.jpg",
		nxt::FileSystem::Instance().GetPathString("textures") + "skybox/back.jpg" };

#elif STARS == 1
	std::vector<std::string> cubemap_textures{
		nxt::FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		nxt::FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		nxt::FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		nxt::FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		nxt::FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		nxt::FileSystem::Instance().GetPathString("textures") + "stars.jpg" };

#else
	std::vector<std::string> cubemap_textures{
	nxt::FileSystem::Instance().GetPathString("textures") + "bricks_3k.jpg",
	nxt::FileSystem::Instance().GetPathString("textures") + "bricks_3k.jpg",
	nxt::FileSystem::Instance().GetPathString("textures") + "bricks_3k.jpg",
	nxt::FileSystem::Instance().GetPathString("textures") + "bricks_3k.jpg",
	nxt::FileSystem::Instance().GetPathString("textures") + "bricks_3k.jpg",
	nxt::FileSystem::Instance().GetPathString("textures") + "bricks_3k.jpg" };
#endif

	nxt::ResourceManager::LoadShader(
		nxt::FileSystem::Instance().GetPathString("shader") + "cubemap_vert_shader.glsl",
		nxt::FileSystem::Instance().GetPathString("shader") + "cubemap_frag_shader.glsl",
		"cubemap");
	nxt::ResourceManager::LoadTexture(
		nxt::ResourceManager::GetShader("cubemap"),
		cubemap_textures,
		"faces");

#if PHONG == 0
	nxt::ResourceManager::LoadShader(
		nxt::FileSystem::Instance().GetPathString("shader") + "mesh_vert_shader.glsl",
		nxt::FileSystem::Instance().GetPathString("shader") + "mesh_frag_shader.glsl",
		"model");
#else
	nxt::ResourceManager::LoadShader(
		nxt::FileSystem::Instance().GetPathString("shader") + "mesh_vert_phong.glsl",
		nxt::FileSystem::Instance().GetPathString("shader") + "mesh_frag_phong.glsl",
		"model");
#endif
	projection = camera->GetProjectionMatrix(nxt::Context::Instance().GetRatio());

	nxt::ResourceManager::GetShader("model")->SetFloat("xoffset[0]", -10.0f);
	nxt::ResourceManager::GetShader("model")->SetFloat("xoffset[1]", -5.0f);
	nxt::ResourceManager::GetShader("model")->SetFloat("xoffset[2]", 0.0f);
	nxt::ResourceManager::GetShader("model")->SetFloat("xoffset[3]", 5.0f);
	nxt::ResourceManager::GetShader("model")->SetFloat("xoffset[4]", 10.0f);
	nxt::ResourceManager::GetShader("model")->SetMat4("u_model", model_);
	nxt::ResourceManager::GetShader("model")->SetMat4("u_projection", projection);

#if PHONG == 1
	nxt::ResourceManager::GetShader("model")->SetVec3("u_light_color", glm::fvec3{ 1.0f, 1.0f, 1.0f });
	nxt::ResourceManager::GetShader("model")->SetVec3("u_view_pos", camera->GetPosition());
#endif

	nxt::ResourceManager::GetShader("cubemap")->SetInt("skybox", 0);
	nxt::ResourceManager::GetShader("cubemap")->SetMat4("projection", projection);

	nxt::ResourceManager::LoadTexture(
		nxt::ResourceManager::GetShader("model"),
		nxt::FileSystem::Instance().GetPathString("textures") + "cyborg_diffuse.png",
		"cyborg");
	nxt::ResourceManager::LoadTexture(
		nxt::ResourceManager::GetShader("model"),
		nxt::FileSystem::Instance().GetPathString("textures") + "bricks.jpg",
		"floor");
#if DEV == 1
	nxt::ResourceManager::LoadTexture(
		nxt::ResourceManager::GetShader("model"),
		nxt::FileSystem::Instance().GetPathString("textures") + "table.jpg",
		"table");
#endif

	nxt::ResourceManager::LoadShader(
		nxt::FileSystem::Instance().GetPathString("shader") + "font_vert.glsl",
		nxt::FileSystem::Instance().GetPathString("shader") + "font_frag.glsl",
		"text");

	nxt::ResourceManager::LoadShader(
		nxt::FileSystem::Instance().GetPathString("shader") + "sprite_vert_shader.glsl",
		nxt::FileSystem::Instance().GetPathString("shader") + "sprite_frag_shader.glsl",
		"sprite");

	nxt::ResourceManager::LoadTextRenderer(
		nxt::ResourceManager::GetShader("text"),
		nxt::Context::Instance().GetWidth(),
		nxt::Context::Instance().GetHeight(),
		20,
		nxt::FileSystem::Instance().GetPathString("fonts") + "Wallpoet-Regular.ttf",
		"Wallpoet");

	nxt::ResourceManager::LoadTexture(
		nxt::ResourceManager::GetShader("sprite"),
		nxt::FileSystem::Instance().GetPathString("textures") + "donut_icon.png",
		"donut");

	meshes_.push_back(std::make_unique<nxt::MeshRenderer>(nxt::ResourceManager::GetShader("model")));
	meshes_[0]->Load(
		nxt::FileSystem::Instance().GetPathString("models") + "cyborg.obj",
		false);

	meshes_.push_back(std::make_unique<nxt::MeshRenderer>(nxt::ResourceManager::GetShader("model")));
	meshes_[1]->Load(
		nxt::FileSystem::Instance().GetPathString("models") + "floor.obj",
		false);

	meshes_.push_back(std::make_unique<nxt::MeshRenderer>(nxt::ResourceManager::GetShader("cubemap")));
	meshes_[2]->Load(
		nxt::FileSystem::Instance().GetPathString("models") + "cube.obj");

#if DEV == 1
	meshes_.push_back(std::make_unique<nxt::MeshRenderer>(nxt::ResourceManager::GetShader("model")));
	meshes_[3]->Load(
		nxt::FileSystem::Instance().GetPathString("models") + "table.obj",
		false);
#endif

	sprites_.push_back(std::make_unique<nxt::SpriteRenderer>(
		nxt::ResourceManager::GetShader("sprite"),
		nxt::Context::Instance().GetWidth(),
		nxt::Context::Instance().GetHeight()));

	audio_list_[0]->Open(nxt::FileSystem::Instance().GetPathString("audio") + "throne.ogg");
	audio_list_[0]->Play(true);
	audio_list_[0]->Volume(10.0f);
	audio_list_[0]->Pitch(1.2f);

	audio_list_[1]->Open(nxt::FileSystem::Instance().GetPathString("audio") + "powerup1.ogg");
	audio_list_[1]->Play();
}

void Sandbox::ProcessInput(float dt) {
	if (nxt::Context::Instance().KeyDown(nxt::KeyNum::KEY_ESCAPE))
		nxt::Context::Instance().SetCloseFlag();

	if (nxt::Context::Instance().KeyDown(nxt::KeyNum::KEY_E))
		nxt::opengl::PolygonMode();
	if (nxt::Context::Instance().KeyDown(nxt::KeyNum::KEY_Q))
		nxt::opengl::FillMode();

	if (nxt::Context::Instance().KeyDown(nxt::KeyNum::KEY_W))
		camera->HandleKeyboard(nxt::CameraMovement::FORWARD, dt);
	if (nxt::Context::Instance().KeyDown(nxt::KeyNum::KEY_S))
		camera->HandleKeyboard(nxt::CameraMovement::BACKWARD, dt);
	if (nxt::Context::Instance().KeyDown(nxt::KeyNum::KEY_A))
		camera->HandleKeyboard(nxt::CameraMovement::LEFT, dt);
	if (nxt::Context::Instance().KeyDown(nxt::KeyNum::KEY_D))
		camera->HandleKeyboard(nxt::CameraMovement::RIGHT, dt);

	if (nxt::Context::Instance().KeyDown(nxt::MouseButton::MOUSE_LEFT))
		audio_list_[1]->Play();
	if (nxt::Context::Instance().KeyDown(nxt::KeyNum::KEY_F))
		audio_list_[1]->Play();
	if (nxt::Context::Instance().KeyDown(nxt::KeyNum::KEY_R))
		audio_list_[1]->Open(nxt::FileSystem::Instance().GetPathString("audio") + "powerup1.ogg");
	if (nxt::Context::Instance().KeyDown(nxt::KeyNum::KEY_V))
		audio_list_[1]->Open(nxt::FileSystem::Instance().GetPathString("audio") + "powerup2.ogg");
}

void Sandbox::Render() {
	nxt::Renderer::Clear();

#if PHONG == 1
	light_position_.x = static_cast<float>(4 * sinf(nxt::Context::Instance().GetTime() * 3));
	light_position_.z = static_cast<float>(4 * cosf(nxt::Context::Instance().GetTime() * 3));
#endif

	view_ = camera->GetViewMatrix();
	nxt::ResourceManager::GetShader("model")->SetMat4("u_view", view_);

#if PHONG == 1
	nxt::ResourceManager::GetShader("model")->SetVec3("u_view_pos", camera->GetPosition());
	nxt::ResourceManager::GetShader("model")->SetVec3("u_light_pos", light_position_);
#endif

	nxt::ResourceManager::GetTexture("floor")->Bind("u_tex_sampler", 0);
	meshes_[1]->Draw(5);
	nxt::ResourceManager::GetTexture("floor")->Unbind(0);

#if DEV == 1
	nxt::ResourceManager::GetTexture("table")->Bind("u_tex_sampler", 0);
	meshes_[3]->Draw(5);
	nxt::ResourceManager::GetTexture("table")->Unbind(0);
#else
	nxt::ResourceManager::GetTexture("cyborg")->Bind("u_tex_sampler", 0);
	meshes_[0]->Draw(5);
	nxt::ResourceManager::GetTexture("cyborg")->Unbind(0);
#endif

	view_ = camera->GetViewMatrix(false);
	nxt::ResourceManager::GetShader("cubemap")->SetMat4("view", view_);

	nxt::ResourceManager::GetTexture("faces")->Bind("skybox", 0);
	nxt::opengl::SetCubeMapMode();
	meshes_[2]->Draw();
	nxt::opengl::ResetCubeMapMode();
	nxt::ResourceManager::GetTexture("faces")->Unbind(0);

	nxt::ResourceManager::GetTextRenderer("Wallpoet")->Draw(
		"Framerate: " + std::to_string(nxt::Context::Instance().GetFrameRate(2)).substr(0, 5),
		0.0f,
		0.0f,
		1.2f,
		glm::fvec3{ 0.5f, 0.5f, 0.5f });

	sprites_[0]->Draw(
		nxt::ResourceManager::GetTexture("donut"),
		glm::fvec2{ nxt::Context::Instance().GetWidth() - 100.0f, nxt::Context::Instance().GetHeight() - 100.0f },
		{ glm::fvec2{ -100.0f, 0.0f }, glm::fvec2{ -200.0f, 0.0f } },
		glm::fvec2{ 100.0f, 100.0f });

	nxt::Context::Instance().PollEvents();
	nxt::Context::Instance().SwapBuffers();
}

void Sandbox::SetCallbacks() {
	glfwSetScrollCallback(nxt::Context::Instance().Get(), [](GLFWwindow* win, double xoffset, double yoffset) {
		camera->HandleScroll(static_cast<float>(yoffset));
#if FPS == 1
		projection = camera->GetProjectionMatrix(nxt::Context::Instance().GetRatio());
		nxt::ResourceManager::GetShader("model")->SetMat4("u_projection", projection);
		nxt::ResourceManager::GetShader("cubemap")->SetMat4("projection", projection);
#endif
	});

	glfwSetCursorPosCallback(nxt::Context::Instance().Get(), [](GLFWwindow* win, double xpos, double ypos) {
		camera->HandleMouseCursor(static_cast<float>(xpos), static_cast<float>(ypos));
	});

	glfwSetFramebufferSizeCallback(
		nxt::Context::Instance().Get(), [](GLFWwindow*, int width, int height) {
		nxt::opengl::SetViewport(0, 0, width, height);
	});
}

void Sandbox::Run() {
	while (!nxt::Context::Instance()) {
		ProcessInput(nxt::Context::Instance().GetTimePerFrame());
		Render();
	}
}

nxt::Application* nxt::CreateApplication() {
	return new Sandbox();
}
