#include "virtual_show_room.hpp"

#define FPS 1

glm::mat4 VirtualShowRoom::projection;
std::unique_ptr<nxt::Camera> VirtualShowRoom::camera;

VirtualShowRoom::VirtualShowRoom() {}

VirtualShowRoom::~VirtualShowRoom() {
	nxt::Context::Instance().Terminate();
}

void VirtualShowRoom::Init() {
	camera =
#if FPS == 1
		std::make_unique<nxt::FPSCamera>(glm::fvec3{ 2.0f, 5.0f, 12.0f });
#else
		std::make_unique<nxt::OrbitCamera>();
#endif

	nxt::FileSystem::Instance().SetResourceRootDir("Resources");
	nxt::FileSystem::Instance().InitSubDirs({ "fonts", "audio", "models", "shader", "textures" });

	audio_list_.push_back(std::make_unique<nxt::Music>());

	nxt::context::Config config;
	config.opengl_major = 3;
	config.opengl_minor = 3;
	config.number_of_samples = 4;
	config.debug_context = true;
	config.cusor_enabled = false;
	config.context_title = "Virtual Show Room";
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

	light_position_ = glm::fvec3{ 0.0f, 4.0f, -1.0f };
	projection = camera->GetProjectionMatrix(nxt::Context::Instance().GetRatio());
	floor_model_ = glm::scale<float>(glm::fvec3{ 15.0f, 1.0f, 15.0f });

	cupboard_model_ = glm::translate<float>(glm::fvec3{ 0.0f, 0.0f, -8.0f });
	cupboard_model_ *= glm::rotate<float>(glm::radians<float>(-90), glm::fvec3{ 0.0f,1.0f,0.0f });

	sofa_model_ = glm::scale<float>(glm::fvec3{ 1.2f,1.2f,1.2f });
	sofa_model_ *= glm::translate<float>(glm::fvec3{ -7.0f, 0.0f, 0.0f });

	tv_model_ = glm::scale<float>(glm::fvec3{ 1.2f,1.2f,1.2f });
	tv_model_ *= glm::translate<float>(glm::fvec3{ 6.2f, 2.12f, 0.0f });

	lowboard_model_ = glm::translate<float>(glm::fvec3{ 7.0f, 0.0f, 0.0f });
	lowboard_model_ *= glm::rotate<float>(glm::radians<float>(90), glm::fvec3{ 0.0f,1.0f,0.0f });

	lamp_model_ = glm::scale<float>(glm::fvec3{ 1.2f, 1.2f, 1.2f });
	lamp_model_ *= glm::translate<float>(glm::fvec3{ 0.0f, 1.8f, 0.0f });

	table_model_ = glm::scale<float>(glm::fvec3{ 1.2f,1.2f,1.2f });

	nxt::ResourceManager::LoadShader(
		nxt::FileSystem::Instance().GetPathString("shader") + "cubemap_vert_shader.glsl",
		nxt::FileSystem::Instance().GetPathString("shader") + "cubemap_frag_shader.glsl",
		"cubemap");
	nxt::ResourceManager::LoadShader(
		nxt::FileSystem::Instance().GetPathString("shader") + "font_vert.glsl",
		nxt::FileSystem::Instance().GetPathString("shader") + "font_frag.glsl",
		"text");
	nxt::ResourceManager::LoadShader(
		nxt::FileSystem::Instance().GetPathString("shader") + "mesh_vert_phong.glsl",
		nxt::FileSystem::Instance().GetPathString("shader") + "mesh_frag_phong.glsl",
		"model");

	std::vector<std::string> cubemap_textures{
	nxt::FileSystem::Instance().GetPathString("textures") + "bricks.jpg",
	nxt::FileSystem::Instance().GetPathString("textures") + "bricks.jpg",
	nxt::FileSystem::Instance().GetPathString("textures") + "bricks.jpg",
	nxt::FileSystem::Instance().GetPathString("textures") + "bricks.jpg",
	nxt::FileSystem::Instance().GetPathString("textures") + "bricks.jpg",
	nxt::FileSystem::Instance().GetPathString("textures") + "bricks.jpg" };

	nxt::ResourceManager::LoadTexture(
		nxt::ResourceManager::GetShader("cubemap"),
		cubemap_textures,
		"faces");
	nxt::ResourceManager::LoadTexture(
		nxt::ResourceManager::GetShader("model"),
		nxt::FileSystem::Instance().GetPathString("textures") + "wood_floor.jpg",
		"floor");
	nxt::ResourceManager::LoadTexture(
		nxt::ResourceManager::GetShader("model"),
		nxt::FileSystem::Instance().GetPathString("textures") + "cupboard_table.jpg",
		"cupboard_table");
	nxt::ResourceManager::LoadTexture(
		nxt::ResourceManager::GetShader("model"),
		nxt::FileSystem::Instance().GetPathString("textures") + "tv.jpg",
		"tv");
	nxt::ResourceManager::LoadTexture(
		nxt::ResourceManager::GetShader("model"),
		nxt::FileSystem::Instance().GetPathString("textures") + "sofa.jpg",
		"sofa");
	nxt::ResourceManager::LoadTexture(
		nxt::ResourceManager::GetShader("model"),
		nxt::FileSystem::Instance().GetPathString("textures") + "lowboard.jpg",
		"lowboard");
	nxt::ResourceManager::LoadTexture(
		nxt::ResourceManager::GetShader("model"),
		nxt::FileSystem::Instance().GetPathString("textures") + "lamp.jpg",
		"lamp");

	nxt::ResourceManager::LoadTextRenderer(
		nxt::ResourceManager::GetShader("text"),
		nxt::Context::Instance().GetWidth(),
		nxt::Context::Instance().GetHeight(),
		20,
		nxt::FileSystem::Instance().GetPathString("fonts") + "Wallpoet-Regular.ttf",
		"Wallpoet");

	nxt::ResourceManager::GetShader("model")->SetMat4("u_projection", projection);
	nxt::ResourceManager::GetShader("model")->SetVec3("u_light_color", glm::fvec3{ 1.0f, 1.0f, 1.0f });
	nxt::ResourceManager::GetShader("model")->SetVec3("u_view_pos", camera->GetPosition());
	nxt::ResourceManager::GetShader("model")->SetVec3("u_light_pos", light_position_);
	nxt::ResourceManager::GetShader("cubemap")->SetInt("skybox", 0);
	nxt::ResourceManager::GetShader("cubemap")->SetMat4("projection", projection);

	meshes_.push_back(std::make_unique<nxt::MeshRenderer>(nxt::ResourceManager::GetShader("model")));
	meshes_[0]->Load(
		nxt::FileSystem::Instance().GetPathString("models") + "floor.obj",
		false);

	meshes_.push_back(std::make_unique<nxt::MeshRenderer>(nxt::ResourceManager::GetShader("cubemap")));
	meshes_[1]->Load(
		nxt::FileSystem::Instance().GetPathString("models") + "cube.obj");

	meshes_.push_back(std::make_unique<nxt::MeshRenderer>(nxt::ResourceManager::GetShader("model")));
	meshes_[2]->Load(
		nxt::FileSystem::Instance().GetPathString("models") + "cupboard.obj",
		false);

	meshes_.push_back(std::make_unique<nxt::MeshRenderer>(nxt::ResourceManager::GetShader("model")));
	meshes_[3]->Load(
		nxt::FileSystem::Instance().GetPathString("models") + "table.obj",
		false);

	meshes_.push_back(std::make_unique<nxt::MeshRenderer>(nxt::ResourceManager::GetShader("model")));
	meshes_[4]->Load(
		nxt::FileSystem::Instance().GetPathString("models") + "tv.obj",
		false);

	meshes_.push_back(std::make_unique<nxt::MeshRenderer>(nxt::ResourceManager::GetShader("model")));
	meshes_[5]->Load(
		nxt::FileSystem::Instance().GetPathString("models") + "sofa.obj",
		false);

	meshes_.push_back(std::make_unique<nxt::MeshRenderer>(nxt::ResourceManager::GetShader("model")));
	meshes_[6]->Load(
		nxt::FileSystem::Instance().GetPathString("models") + "lowboard.obj",
		false);

	meshes_.push_back(std::make_unique<nxt::MeshRenderer>(nxt::ResourceManager::GetShader("model")));
	meshes_[7]->Load(
		nxt::FileSystem::Instance().GetPathString("models") + "lamp.obj",
		false);

	audio_list_[0]->Open(nxt::FileSystem::Instance().GetPathString("audio") + "waves.ogg");
	audio_list_[0]->Play(true);
	audio_list_[0]->Volume(10.0f);
	audio_list_[0]->Pitch(1.0f);
}

void VirtualShowRoom::ProcessInput(float dt) {
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
}

void VirtualShowRoom::Render() {
	nxt::Renderer::Clear();

	view_ = camera->GetViewMatrix();
	nxt::ResourceManager::GetShader("model")->SetMat4("u_view", view_);
	nxt::ResourceManager::GetShader("model")->SetVec3("u_view_pos", camera->GetPosition());

	nxt::ResourceManager::GetShader("model")->SetMat4("u_model", floor_model_);
	nxt::ResourceManager::GetTexture("floor")->Bind("u_tex_sampler", 0);
	meshes_[0]->Draw();
	nxt::ResourceManager::GetTexture("floor")->Unbind(0);

	nxt::ResourceManager::GetShader("model")->SetMat4("u_model", cupboard_model_);
	nxt::ResourceManager::GetTexture("cupboard_table")->Bind("u_tex_sampler", 0);
	meshes_[2]->Draw();
	nxt::ResourceManager::GetTexture("cupboard_table")->Unbind(0);

	nxt::ResourceManager::GetShader("model")->SetMat4("u_model", table_model_);
	nxt::ResourceManager::GetTexture("cupboard_table")->Bind("u_tex_sampler", 0);
	meshes_[3]->Draw();
	nxt::ResourceManager::GetTexture("cupboard_table")->Unbind(0);

	nxt::ResourceManager::GetShader("model")->SetMat4("u_model", tv_model_);
	nxt::ResourceManager::GetTexture("tv")->Bind("u_tex_sampler", 0);
	meshes_[4]->Draw();
	nxt::ResourceManager::GetTexture("tv")->Unbind(0);

	nxt::ResourceManager::GetShader("model")->SetMat4("u_model", sofa_model_);
	nxt::ResourceManager::GetTexture("sofa")->Bind("u_tex_sampler", 0);
	meshes_[5]->Draw();
	nxt::ResourceManager::GetTexture("sofa")->Unbind(0);

	nxt::ResourceManager::GetShader("model")->SetMat4("u_model", lowboard_model_);
	nxt::ResourceManager::GetTexture("lowboard")->Bind("u_tex_sampler", 0);
	meshes_[6]->Draw();
	nxt::ResourceManager::GetTexture("lowboard")->Unbind(0);

	nxt::ResourceManager::GetShader("model")->SetMat4("u_model", lamp_model_);
	nxt::ResourceManager::GetTexture("lamp")->Bind("u_tex_sampler", 0);
	nxt::opengl::DisableCullFace();
	meshes_[7]->Draw();
	nxt::opengl::EnableCullFace();
	nxt::ResourceManager::GetTexture("lamp")->Unbind(0);

	view_ = camera->GetViewMatrix(false);
	nxt::ResourceManager::GetShader("cubemap")->SetMat4("view", view_);

	nxt::ResourceManager::GetTexture("faces")->Bind("skybox", 0);
	nxt::opengl::SetCubeMapMode();
	meshes_[1]->Draw();
	nxt::opengl::ResetCubeMapMode();
	nxt::ResourceManager::GetTexture("faces")->Unbind(0);

	nxt::ResourceManager::GetTextRenderer("Wallpoet")->Draw(
		"Framerate: " + std::to_string(nxt::Context::Instance().GetFrameRate(2)).substr(0, 5),
		0.0f,
		0.0f,
		1.2f,
		glm::fvec3{ 0.5f, 0.5f, 0.5f });

	nxt::Context::Instance().PollEvents();
	nxt::Context::Instance().SwapBuffers();
}

void VirtualShowRoom::SetCallbacks() {
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

void VirtualShowRoom::Run() {
	while (!nxt::Context::Instance()) {
		ProcessInput(nxt::Context::Instance().GetTimePerFrame());
		Render();
	}
}

nxt::Application* nxt::CreateApplication() {
	return new VirtualShowRoom();
}
