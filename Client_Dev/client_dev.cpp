#include "client_dev.hpp"

ClientDev::ClientDev() {}

ClientDev::~ClientDev() {
	jf2::Context::Instance().Terminate();
}

void ClientDev::Init() {
	jf2::FileSystem::Instance().SetResourceRootDir("Resources");
	jf2::FileSystem::Instance().InitSubDirs({ "fonts", "audio", "models", "shader", "textures" });

	jf2::context::Config config;
	config.opengl_major = 3;
	config.opengl_minor = 3;
	config.number_of_samples = 4;
	config.debug_context = true;
	config.cusor_enabled = true;
	config.context_title = "JF2 - Parallax Effect";
	config.context_size = jf2::context::Size::DEBUG;

	jf2::Context::Instance().Create(config);
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

	jf2::ResourceManager::LoadTexture(
		jf2::ResourceManager::GetShader("sprite"),
		jf2::FileSystem::Instance().GetPathString("textures") + "parallax/01_ground.png",
		"ground");
	jf2::ResourceManager::LoadTexture(
		jf2::ResourceManager::GetShader("sprite"),
		jf2::FileSystem::Instance().GetPathString("textures") + "parallax/02_trees_and_bushes.png",
		"trees_bushes");
	jf2::ResourceManager::LoadTexture(
		jf2::ResourceManager::GetShader("sprite"),
		jf2::FileSystem::Instance().GetPathString("textures") + "parallax/03_distant_trees.png",
		"distant_trees");
	jf2::ResourceManager::LoadTexture(
		jf2::ResourceManager::GetShader("sprite"),
		jf2::FileSystem::Instance().GetPathString("textures") + "parallax/04_bushes.png",
		"bushes");
	jf2::ResourceManager::LoadTexture(
		jf2::ResourceManager::GetShader("sprite"),
		jf2::FileSystem::Instance().GetPathString("textures") + "parallax/05_hill1.png",
		"hill1");
	jf2::ResourceManager::LoadTexture(
		jf2::ResourceManager::GetShader("sprite"),
		jf2::FileSystem::Instance().GetPathString("textures") + "parallax/11_background.png",
		"background");

	sprite_ = std::make_unique<jf2::SpriteRenderer>(
		jf2::ResourceManager::GetShader("sprite"),
		jf2::Context::Instance().GetWidth(),
		jf2::Context::Instance().GetHeight());
}

void ClientDev::ProcessInput(float dt) {
	if (jf2::Context::Instance().KeyDown(jf2::KeyNum::KEY_ESCAPE))
		jf2::Context::Instance().SetCloseFlag();

	if (jf2::Context::Instance().KeyDown(jf2::KeyNum::KEY_E))
		jf2::opengl::PolygonMode();
	if (jf2::Context::Instance().KeyDown(jf2::KeyNum::KEY_Q))
		jf2::opengl::FillMode();

	jf2::Context::Instance().PollEvents();
}

void ClientDev::Render() {
	static unsigned int stride{ 0 };
	++stride;
	if (stride > jf2::Context::Instance().GetWidth() / 5) stride = 0;
	jf2::Renderer::Clear();

	jf2::ResourceManager::GetTextRenderer("Wallpoet")->Draw(
		"Framerate: " + std::to_string(jf2::Context::Instance().GetFrameRate(2)).substr(0, 5),
		0.0f,
		0.0f,
		1.2f,
		glm::fvec3{ 0.5f, 0.5f, 0.5f });

	// Put this in an appropriate class method
	glDepthFunc(GL_LEQUAL);

	sprite_->Draw(
		jf2::ResourceManager::GetTexture("background"),
		glm::fvec2{ 0.0f, 0.0f },
		glm::fvec2{ jf2::Context::Instance().GetWidth(), jf2::Context::Instance().GetHeight() });
	sprite_->Draw(
		jf2::ResourceManager::GetTexture("hill1"),
		glm::fvec2{ 0.0f - 0.6f * stride, 0.0f },
		glm::fvec2{ jf2::Context::Instance().GetWidth(), jf2::Context::Instance().GetHeight() });
	sprite_->Draw(
		jf2::ResourceManager::GetTexture("bushes"),
		glm::fvec2{ 0.0f - 0.5f * stride, 0.0f },
		glm::fvec2{ jf2::Context::Instance().GetWidth(), jf2::Context::Instance().GetHeight() });
	sprite_->Draw(
		jf2::ResourceManager::GetTexture("distant_trees"),
		glm::fvec2{ 0.0f - 0.1f * stride, 0.0f },
		glm::fvec2{ jf2::Context::Instance().GetWidth(), jf2::Context::Instance().GetHeight() });
	sprite_->Draw(
		jf2::ResourceManager::GetTexture("trees_bushes"),
		glm::fvec2{ 0.0f + 0.1f * stride, 0.0f },
		glm::fvec2{ jf2::Context::Instance().GetWidth(), jf2::Context::Instance().GetHeight() });
	sprite_->Draw(
		jf2::ResourceManager::GetTexture("ground"),
		glm::fvec2{ 0.0f + 0.5f * stride, 0.0f },
		glm::fvec2{ jf2::Context::Instance().GetWidth(), jf2::Context::Instance().GetHeight() });

	//sprite_->Draw(
	//	jf2::ResourceManager::GetTexture("donut"),
	//	glm::fvec2{ jf2::Context::Instance().GetWidth() - 100.0f, jf2::Context::Instance().GetHeight() - 100.0f },
	//	{ glm::fvec2{ -100.0f, 0.0f }, glm::fvec2{ -200.0f, 0.0f } },
	//	glm::fvec2{ 100.0f, 100.0f });
	sprite_->Draw(
		jf2::ResourceManager::GetTexture("donut"),
		glm::fvec2{ jf2::Context::Instance().GetWidth() - 100.0f, jf2::Context::Instance().GetHeight() - 100.0f },
		glm::fvec2{ 100.0f, 100.0f });

	jf2::Context::Instance().SwapBuffers();
}

void ClientDev::SetCallbacks() {
	glfwSetScrollCallback(jf2::Context::Instance().Get(), [](GLFWwindow* win, double xoffset, double yoffset) {});

	glfwSetCursorPosCallback(jf2::Context::Instance().Get(), [](GLFWwindow* win, double xpos, double ypos) {});

	glfwSetFramebufferSizeCallback(
		jf2::Context::Instance().Get(), [](GLFWwindow*, int width, int height) {
		jf2::opengl::SetViewport(0, 0, width, height);
	});
}

void ClientDev::Run() {
	while (!jf2::Context::Instance()) {
		ProcessInput(jf2::Context::Instance().GetTimePerFrame());
		Render();
	}
}

jf2::Application* jf2::CreateApplication() {
	return new ClientDev();
}
