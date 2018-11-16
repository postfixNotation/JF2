#include <sandbox.hpp>

#define FPS 1
#define PHONG 1
#define STARS 0

glm::mat4 Sandbox::projection_;
std::unique_ptr<Camera> Sandbox::camera_;

Application* CreateApplication() {
	return new Sandbox();
}

Sandbox::Sandbox() {}

Sandbox::~Sandbox() {
	Context::Instance().Terminate();
}

void Sandbox::Init() {
	camera_ =
#if FPS == 1
		std::make_unique<FPSCamera>(glm::vec3{ 0.0f, 0.0f, 20.0f });
#else
		std::make_unique<OrbitCamera>();
#endif

	FileSystem::Instance().SetResourceRootDir("Resources");
	FileSystem::Instance().InitSubDirs({ "fonts", "audio", "models", "shader", "textures" });

	audio_list_.push_back(std::make_unique<Music>());
	audio_list_.push_back(std::make_unique<Sound>());

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
		static_cast<double>(Context::Instance().GetWidth() / 2),
		static_cast<double>(Context::Instance().GetHeight() / 2));
	Context::Instance().SetIcon(
		FileSystem::Instance().GetPathString("textures") + "tux.png");

	opengl::Init();
	opengl::SetDefaultSetting();
	opengl::SetViewport(
		0, 0,
		static_cast<GLsizei>(Context::Instance().GetWidth()),
		static_cast<GLsizei>(Context::Instance().GetHeight()));
	opengl::SetColor(
		179 / 255,
		255 / 255,
		179 / 255,
		1.0f);

	opengl::SetDebugMessageCallback(opengl::DebugMessageCallback);


#if STARS == 0
	std::vector<std::string> cubemap_textures{
		FileSystem::Instance().GetPathString("textures") + "skybox/right.jpg",
		FileSystem::Instance().GetPathString("textures") + "skybox/left.jpg",
		FileSystem::Instance().GetPathString("textures") + "skybox/top.jpg",
		FileSystem::Instance().GetPathString("textures") + "skybox/bottom.jpg",
		FileSystem::Instance().GetPathString("textures") + "skybox/front.jpg",
		FileSystem::Instance().GetPathString("textures") + "skybox/back.jpg"
	};
#else
	std::vector<std::string> cubemap_textures{
		FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		FileSystem::Instance().GetPathString("textures") + "stars.jpg",
		FileSystem::Instance().GetPathString("textures") + "stars.jpg" };
#endif

	ResourceManager::LoadShader(
		FileSystem::Instance().GetPathString("shader") + "cubemap_vert_shader.glsl",
		FileSystem::Instance().GetPathString("shader") + "cubemap_frag_shader.glsl",
		"cubemap");

	ResourceManager::LoadTexture(
		ResourceManager::GetShader("cubemap"),
		cubemap_textures,
		"faces");

#if PHONG == 0
	ResourceManager::LoadShader(
		FileSystem::Instance().GetPathString("shader") + "mesh_vert_shader.glsl",
		FileSystem::Instance().GetPathString("shader") + "mesh_frag_shader.glsl",
		"model");
#else
	ResourceManager::LoadShader(
		FileSystem::Instance().GetPathString("shader") + "mesh_vert_phong.glsl",
		FileSystem::Instance().GetPathString("shader") + "mesh_frag_phong.glsl",
		"model");
#endif
	projection_ = camera_->GetProjectionMatrix(Context::Instance().GetRatio());

	ResourceManager::GetShader("model")->SetFloat("xoffset[0]", -10.0f);
	ResourceManager::GetShader("model")->SetFloat("xoffset[1]", -5.0f);
	ResourceManager::GetShader("model")->SetFloat("xoffset[2]", 0.0f);
	ResourceManager::GetShader("model")->SetFloat("xoffset[3]", 5.0f);
	ResourceManager::GetShader("model")->SetFloat("xoffset[4]", 10.0f);
	ResourceManager::GetShader("model")->SetMat4("u_model", model_);
	ResourceManager::GetShader("model")->SetMat4("u_projection", projection_);
	ResourceManager::GetShader("model")->SetVec3("u_light_color", glm::vec3{ 1.0f, 1.0f, 1.0f });
	ResourceManager::GetShader("model")->SetVec3("u_view_pos", camera_->GetPosition());

	ResourceManager::GetShader("cubemap")->SetInt("skybox", 0);
	ResourceManager::GetShader("cubemap")->SetMat4("projection", projection_);

	ResourceManager::LoadTexture(
		ResourceManager::GetShader("model"),
		FileSystem::Instance().GetPathString("textures") + "cyborg_diffuse.png",
		"cyborg");
	ResourceManager::LoadTexture(
		ResourceManager::GetShader("model"),
		FileSystem::Instance().GetPathString("textures") + "floor.jpg",
		"floor");

	ResourceManager::LoadShader(
		FileSystem::Instance().GetPathString("shader") + "font_vert.glsl",
		FileSystem::Instance().GetPathString("shader") + "font_frag.glsl",
		"text");

	ResourceManager::LoadShader(
		FileSystem::Instance().GetPathString("shader") + "sprite_vert_shader.glsl",
		FileSystem::Instance().GetPathString("shader") + "sprite_frag_shader.glsl",
		"sprite");

	ResourceManager::LoadTextRenderer(
		ResourceManager::GetShader("text"),
		Context::Instance().GetWidth(),
		Context::Instance().GetHeight(),
		20,
		FileSystem::Instance().GetPathString("fonts") + "Wallpoet-Regular.ttf",
		"Wallpoet");

	ResourceManager::LoadTexture(
		ResourceManager::GetShader("sprite"),
		FileSystem::Instance().GetPathString("textures") + "donut_icon.png",
		"donut");

	meshes_.push_back(std::make_unique<MeshRenderer>(ResourceManager::GetShader("model")));
	meshes_[0]->Load(
		FileSystem::Instance().GetPathString("models") + "cyborg.obj",
		false);
	meshes_.push_back(std::make_unique<MeshRenderer>(ResourceManager::GetShader("model")));
	meshes_[1]->Load(
		FileSystem::Instance().GetPathString("models") + "floor.obj",
		false);
	meshes_.push_back(std::make_unique<MeshRenderer>(ResourceManager::GetShader("cubemap")));
	meshes_[2]->Load(
		FileSystem::Instance().GetPathString("models") + "cube.obj");

	sprites_.push_back(std::make_unique<SpriteRenderer>(
		ResourceManager::GetShader("sprite"),
		Context::Instance().GetWidth(),
		Context::Instance().GetHeight()));

	audio_list_[0]->Open(FileSystem::Instance().GetPathString("audio") + "throne.ogg");
	audio_list_[0]->Play(true);
	audio_list_[0]->Volume(10.0f);
	audio_list_[0]->Pitch(1.2f);

	audio_list_[1]->Open(FileSystem::Instance().GetPathString("audio") + "powerup1.ogg");
	audio_list_[1]->Play();
}

void Sandbox::SetCallbacks() {
	glfwSetScrollCallback(Context::Instance().Get(), [](GLFWwindow* win, double xoffset, double yoffset) {
		camera_->HandleScroll(yoffset);
#if FPS == 1
		projection_ = camera_->GetProjectionMatrix(Context::Instance().GetRatio());
		ResourceManager::GetShader("model")->SetMat4("u_projection", projection_);
		ResourceManager::GetShader("cubemap")->SetMat4("projection", projection_);
#endif
	});

	glfwSetCursorPosCallback(Context::Instance().Get(), [](GLFWwindow* win, double xpos, double ypos) {
		camera_->HandleMouseCursor(xpos, ypos);
	});

	glfwSetFramebufferSizeCallback(
		Context::Instance().Get(), [](GLFWwindow*, int width, int height) {
		opengl::SetViewport(0, 0, width, height);
	});
}

void Sandbox::ProcessInput(double dt) {
	if (Context::Instance().KeyDown(KeyNum::KEY_ESCAPE))
		Context::Instance().SetCloseFlag();

	if (Context::Instance().KeyDown(KeyNum::KEY_E))
		opengl::PolygonMode();
	if (Context::Instance().KeyDown(KeyNum::KEY_Q))
		opengl::FillMode();

	if (Context::Instance().KeyDown(KeyNum::KEY_W))
		camera_->HandleKeyboard(CameraMovement::FORWARD, dt);
	if (Context::Instance().KeyDown(KeyNum::KEY_S))
		camera_->HandleKeyboard(CameraMovement::BACKWARD, dt);
	if (Context::Instance().KeyDown(KeyNum::KEY_A))
		camera_->HandleKeyboard(CameraMovement::LEFT, dt);
	if (Context::Instance().KeyDown(KeyNum::KEY_D))
		camera_->HandleKeyboard(CameraMovement::RIGHT, dt);

	if (Context::Instance().KeyDown(MouseButton::MOUSE_LEFT))
		audio_list_[1]->Play();
	if (Context::Instance().KeyDown(KeyNum::KEY_F))
		audio_list_[1]->Play();
	if (Context::Instance().KeyDown(KeyNum::KEY_R))
		audio_list_[1]->Open(FileSystem::Instance().GetPathString("audio") + "powerup1.ogg");
	if (Context::Instance().KeyDown(KeyNum::KEY_V))
		audio_list_[1]->Open(FileSystem::Instance().GetPathString("audio") + "powerup2.ogg");
}

void Sandbox::Run() {
	while (!Context::Instance()) {
		ProcessInput(Context::Instance().GetTimePerFrame());
		Render();
	}
}

void Sandbox::Render() {
	Renderer::Clear();

	light_position_.x = static_cast<float>(4 * sin(Context::Instance().GetTime() * 3));
	light_position_.z = static_cast<float>(4 * cos(Context::Instance().GetTime() * 3));

	view_ = camera_->GetViewMatrix();

	ResourceManager::GetShader("model")->SetMat4("u_view", view_);
	ResourceManager::GetShader("model")->SetVec3("u_view_pos", camera_->GetPosition());
	ResourceManager::GetShader("model")->SetVec3("u_light_pos", light_position_);

	ResourceManager::GetTexture("floor")->Bind("u_tex_sampler", 0);
	meshes_[1]->Draw(5);
	ResourceManager::GetTexture("floor")->Unbind(0);

	ResourceManager::GetTexture("cyborg")->Bind("u_tex_sampler", 0);
	meshes_[0]->Draw(5);
	ResourceManager::GetTexture("cyborg")->Unbind(0);

	view_ = camera_->GetViewMatrix(false);
	ResourceManager::GetShader("cubemap")->SetMat4("view", view_);

	ResourceManager::GetTexture("faces")->Bind("skybox", 0);
	opengl::SetCubeMapMode();
	meshes_[2]->Draw();
	opengl::ResetCubeMapMode();
	ResourceManager::GetTexture("faces")->Unbind(0);

	ResourceManager::GetTextRenderer("Wallpoet")->Draw(
		"Framerate: " + std::to_string(Context::Instance().GetFrameRate(2)).substr(0, 5),
		0.0f,
		0.0f,
		1.2f,
		glm::vec3{ 0.5f, 0.5f, 0.5f });

	sprites_[0]->Draw(
		ResourceManager::GetTexture("donut"),
		glm::vec2{ Context::Instance().GetWidth() - 100.0f, Context::Instance().GetHeight() - 100.0f },
		{ glm::vec2{ -100.0f, 0.0f }, glm::vec2{ -200.0f, 0.0f } },
		glm::vec2{ 100.0f, 100.0f });

	Context::Instance().PollEvents();
	Context::Instance().SwapBuffers();
}
