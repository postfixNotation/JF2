#include "client_dev.hpp"

ClientDev::ClientDev() {}

ClientDev::~ClientDev() {
	nxt::Context::Instance().Terminate();
}

void ClientDev::Init() {
	nxt::FileSystem::Instance().SetResourceRootDir("Resources");
	nxt::FileSystem::Instance().InitSubDirs({ "fonts", "audio", "models", "shader", "textures" });

	nxt::context::Config config;
	config.opengl_major = 3;
	config.opengl_minor = 3;
	config.number_of_samples = 4;
	config.debug_context = true;
	config.cusor_enabled = true;
	config.context_title = "nxt - Parallax Effect";
	config.context_size = nxt::context::Size::DEBUG;

	nxt::Context::Instance().Create(config);
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
		nxt::FileSystem::Instance().GetPathString("fonts") + "SedgwickAve-Regular.ttf",
		"SedgwickAve");

	nxt::ResourceManager::LoadTexture(
		nxt::ResourceManager::GetShader("sprite"),
		nxt::FileSystem::Instance().GetPathString("textures") + "donut_icon.png",
		"donut");

	p_sprite_ = std::make_shared<nxt::SpriteRenderer>(
		nxt::ResourceManager::GetShader("sprite"),
		nxt::Context::Instance().GetWidth(),
		nxt::Context::Instance().GetHeight());

	p_parallax_ = std::make_unique<nxt::ParallaxRenderer>(
		p_sprite_,
		glm::fvec2{ nxt::Context::Instance().GetWidth(), nxt::Context::Instance().GetHeight() });

	std::vector<std::string> image_path_list{
		nxt::FileSystem::Instance().GetPathString("textures") + "parallax/11_background.png",
		nxt::FileSystem::Instance().GetPathString("textures") + "parallax/10_distant_clouds.png",
		nxt::FileSystem::Instance().GetPathString("textures") + "parallax/09_distant_clouds1.png",
		nxt::FileSystem::Instance().GetPathString("textures") + "parallax/08_clouds.png",
		nxt::FileSystem::Instance().GetPathString("textures") + "parallax/07_huge_clouds.png",
		nxt::FileSystem::Instance().GetPathString("textures") + "parallax/06_hill2.png",
		nxt::FileSystem::Instance().GetPathString("textures") + "parallax/05_hill1.png",
		nxt::FileSystem::Instance().GetPathString("textures") + "parallax/04_bushes.png",
		nxt::FileSystem::Instance().GetPathString("textures") + "parallax/03_distant_trees.png",
		nxt::FileSystem::Instance().GetPathString("textures") + "parallax/02_trees_and_bushes.png",
		nxt::FileSystem::Instance().GetPathString("textures") + "parallax/01_ground.png"
	};

	p_parallax_->Init(nxt::ResourceManager::GetShader("sprite"), image_path_list);

	nxt::opengl::SetSpriteMode();
}

void ClientDev::ProcessInput(float dt) {
	if (nxt::Context::Instance().KeyDown(nxt::KeyNum::KEY_ESCAPE))
		nxt::Context::Instance().SetCloseFlag();

	if (nxt::Context::Instance().KeyDown(nxt::KeyNum::KEY_E))
		nxt::opengl::PolygonMode();
	if (nxt::Context::Instance().KeyDown(nxt::KeyNum::KEY_Q))
		nxt::opengl::FillMode();

	nxt::Context::Instance().PollEvents();
}

void ClientDev::Render() {
	nxt::Renderer::Clear();
	static float x{}, y{};

	float x_half_width = nxt::Context::Instance().GetWidth() / 2;
	float y_half_width = nxt::Context::Instance().GetHeight() / 2;

	p_parallax_->Draw(0.4f, glm::fvec2{ x, y });

	x += 0.5f;
	y += 0.5f;

	x = glm::clamp<float>(x, 0, x_half_width * 2);
	y = glm::clamp<float>(y, 0, y_half_width * 2);

	//p_sprite_->Draw(
	//	nxt::ResourceManager::GetTexture("donut"),
	//	glm::fvec2{ nxt::Context::Instance().GetWidth() - 100.0f, nxt::Context::Instance().GetHeight() - 100.0f },
	//	{ glm::fvec2{ -100.0f, 0.0f }, glm::fvec2{ -200.0f, 0.0f } },
	//	glm::fvec2{ 100.0f, 100.0f });

	p_sprite_->Draw(
		nxt::ResourceManager::GetTexture("donut"),
		glm::fvec2{ nxt::Context::Instance().GetWidth() - 100.0f, nxt::Context::Instance().GetHeight() - 100.0f },
		glm::fvec2{ 100.0f, 100.0f });

	nxt::ResourceManager::GetTextRenderer("SedgwickAve")->Draw(
		"Framerate: " + std::to_string(nxt::Context::Instance().GetFrameRate(2)).substr(0, 5),
		0.0f,
		0.0f,
		1.2f,
		glm::fvec3{ 0.9f, 0.4f, 0.5f });

	nxt::Context::Instance().SwapBuffers();
}

void ClientDev::SetCallbacks() {
	glfwSetScrollCallback(nxt::Context::Instance().Get(), [](GLFWwindow* win, double xoffset, double yoffset) {});
	glfwSetCursorPosCallback(nxt::Context::Instance().Get(), [](GLFWwindow* win, double xpos, double ypos) {});

	glfwSetFramebufferSizeCallback(
		nxt::Context::Instance().Get(), [](GLFWwindow*, int width, int height) {
		nxt::opengl::SetViewport(0, 0, width, height);
	});
}

void ClientDev::Run() {
	while (!nxt::Context::Instance()) {
		ProcessInput(nxt::Context::Instance().GetTimePerFrame());
		Render();
	}
}

nxt::Application* nxt::CreateApplication() {
	return new ClientDev();
}
