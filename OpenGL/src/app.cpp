#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Box2D/Box2D.h>

#include <jf2.hpp>

#define FPS 1
constexpr float near = 0.1f;
constexpr float far = 100.0f;

std::unique_ptr<Camera> camera =
#if FPS == 1
	std::make_unique<FPSCamera>(glm::vec3{0.0f, 0.0f, 20.0f});
#else
	std::make_unique<OrbitCamera>();
#endif

glm::mat4 projection{}, model_mat{}, view{};
std::unique_ptr<Audio> music = std::make_unique<Music>();
std::unique_ptr<Audio> sound = std::make_unique<Sound>();

void ProcessKeyInput(double dt);
void SetCallbacks();

int main(int argc, const char **argv) {
	FileSystem::Instance().SetResourceRootDir("Resources");
	FileSystem::Instance().InitSubDirs( {"fonts", "audio", "models", "shader", "textures"} );

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
	Context::Instance().SetIcon(
		FileSystem::Instance().GetPathString("textures")+"android_2.png");

	opengl::Init();
	opengl::SetDefaultSetting();
	opengl::SetViewport(
		0,
		0,
		Context::Instance().GetWidth(),
		Context::Instance().GetHeight());
	opengl::SetColor(1.0f, 0.9f, 0.8f, 1.0f);
	opengl::SetDebugMessageCallback(opengl::DebugMessageCallback);

	SetCallbacks();
	std::vector<std::string> faces{
		FileSystem::Instance().GetPathString("textures")+"skybox/right.jpg",
		FileSystem::Instance().GetPathString("textures")+"skybox/left.jpg",
		FileSystem::Instance().GetPathString("textures")+"skybox/top.jpg",
		FileSystem::Instance().GetPathString("textures")+"skybox/bottom.jpg",
		FileSystem::Instance().GetPathString("textures")+"skybox/front.jpg",
		FileSystem::Instance().GetPathString("textures")+"skybox/back.jpg"};
	//std::vector<std::string> stars{
	//	FileSystem::Instance().GetPathString("textures")+"stars.jpg",
	//	FileSystem::Instance().GetPathString("textures")+"stars.jpg",
	//	FileSystem::Instance().GetPathString("textures")+"stars.jpg",
	//	FileSystem::Instance().GetPathString("textures")+"stars.jpg",
	//	FileSystem::Instance().GetPathString("textures")+"stars.jpg",
	//	FileSystem::Instance().GetPathString("textures")+"stars.jpg"
	//};

	ResourceManager::LoadShader(
		FileSystem::Instance().GetPathString("shader")+"cubemap_vert_shader.glsl",
		FileSystem::Instance().GetPathString("shader")+"cubemap_frag_shader.glsl",
		"cubemap");
	ResourceManager::GetShader("cubemap")->SetInt("skybox", 0);

	ResourceManager::LoadTexture(
		ResourceManager::GetShader("cubemap"),
		faces,
		"faces");
	//ResourceManager::LoadTexture(
	//	ResourceManager::GetShader("cubemap"),
	//	stars,
	//	"faces");

	ResourceManager::LoadShader(
		FileSystem::Instance().GetPathString("shader")+"mesh_vert_shader.glsl",
		FileSystem::Instance().GetPathString("shader")+"mesh_frag_shader.glsl",
		"model");
	ResourceManager::GetShader("model")->SetMat4("model", model_mat);

	ResourceManager::LoadTexture(
		ResourceManager::GetShader("model"),
		FileSystem::Instance().GetPathString("textures")+"cyborg_diffuse.png",
		"cyborg");

	ResourceManager::LoadShader(
		FileSystem::Instance().GetPathString("shader")+"font_vert.glsl",
		FileSystem::Instance().GetPathString("shader")+"font_frag.glsl",
		"text");

	ResourceManager::LoadShader(
		FileSystem::Instance().GetPathString("shader")+"sprite_vert_shader.glsl",
		FileSystem::Instance().GetPathString("shader")+"sprite_frag_shader.glsl",
		"sprite");

	ResourceManager::LoadTextRenderer(
		ResourceManager::GetShader("text"),
		Context::Instance().GetWidth(),
		Context::Instance().GetHeight(),
		20,
		FileSystem::Instance().GetPathString("fonts")+"Wallpoet-Regular.ttf",
		"Wallpoet");

	ResourceManager::LoadTexture(
		ResourceManager::GetShader("sprite"),
		FileSystem::Instance().GetPathString("textures")+"tux.png",
		"tux");

	std::unique_ptr<MeshRenderer> model = std::make_unique<MeshRenderer>(ResourceManager::GetShader("model"));
	model->Load(
		FileSystem::Instance().GetPathString("models")+"cyborg.obj",
		false);

	std::unique_ptr<MeshRenderer> cube_map_mesh{
		std::unique_ptr<MeshRenderer>(new MeshRenderer(ResourceManager::GetShader("cubemap"))) };
	cube_map_mesh->Load(
		FileSystem::Instance().GetPathString("models")+"cube.obj");

	std::unique_ptr<SpriteRenderer> sprite = std::unique_ptr<SpriteRenderer>(
		new SpriteRenderer(ResourceManager::GetShader("sprite"),
		Context::Instance().GetWidth(),
		Context::Instance().GetHeight()));

	music->Open(FileSystem::Instance().GetPathString("audio")+"throne.ogg");
	music->Play(true);
	music->Volume(10.0f);
	music->Pitch(1.2f);
	sound->Open(FileSystem::Instance().GetPathString("audio")+"powerup1.ogg");
	sound->Play();

	double previous_time{ glfwGetTime() };

	#if FPS == 0
		projection = glm::perspective(
			glm::radians(camera->GetFov()),
			Context::Instance().GetRatio(),
			near,
			far);
		ResourceManager::GetShader("model")->SetMat4("projection", projection);
	#endif

	while (!Context::Instance()) {
		ProcessKeyInput(glfwGetTime() - previous_time);
		previous_time = glfwGetTime();

		Renderer::Clear();

		view = camera->GetViewMatrix();
		#if FPS == 1
			projection = glm::perspective(
				glm::radians(camera->GetFov()),
				Context::Instance().GetRatio(),
				near,
				far);
			ResourceManager::GetShader("model")->SetMat4("projection", projection);
		#endif
		ResourceManager::GetShader("model")->SetMat4("view", view);

		ResourceManager::GetTexture("cyborg")->Bind("tex_sampler", 0);
		model->Draw();
		ResourceManager::GetTexture("cyborg")->Unbind(0);

		view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
		ResourceManager::GetShader("cubemap")->SetMat4("view", view);
		ResourceManager::GetShader("cubemap")->SetMat4("projection", projection);

		ResourceManager::GetTexture("faces")->Bind("skybox", 0);
		glDepthFunc(GL_LEQUAL);
		glFrontFace(GL_CW);
		cube_map_mesh->Draw();
		glFrontFace(GL_CCW);
		glDepthFunc(GL_LESS);
		ResourceManager::GetTexture("faces")->Unbind(0);

		ResourceManager::GetTextRenderer("Wallpoet")->Draw(
			"Framerate: "+std::to_string(Context::Instance().GetFrameRate(2)),
			0.0f,
			0.0f,
			1.2f,
			glm::vec3{ 0.5f, 0.5f, 0.5f });
		sprite->Draw(
			ResourceManager::GetTexture("tux"),
			glm::vec2{ Context::Instance().GetWidth() - 100.0f, Context::Instance().GetHeight() - 100.0f },
			glm::vec2{ 100.0f, 100.0f });

		Context::Instance().PollEvents();
		Context::Instance().SwapBuffers();
	}

	Context::Instance().Terminate();
	return 0;
}

void ProcessKeyInput(double dt) {
	if (Context::Instance().KeyDown(KeyNum::KEY_ESCAPE))
		Context::Instance().SetCloseFlag();

	if (Context::Instance().KeyDown(KeyNum::KEY_E))
		opengl::PolygonMode();
	if (Context::Instance().KeyDown(KeyNum::KEY_Q))
		opengl::FillMode();

	if (Context::Instance().KeyDown(KeyNum::KEY_W))
		camera->HandleKeyboard(CameraMovement::FORWARD, dt);
	if (Context::Instance().KeyDown(KeyNum::KEY_S))
		camera->HandleKeyboard(CameraMovement::BACKWARD, dt);
	if (Context::Instance().KeyDown(KeyNum::KEY_A))
		camera->HandleKeyboard(CameraMovement::LEFT, dt);
	if (Context::Instance().KeyDown(KeyNum::KEY_D))
		camera->HandleKeyboard(CameraMovement::RIGHT, dt);

	if (Context::Instance().KeyDown(MouseButton::MOUSE_LEFT))
		sound->Play();
	if (Context::Instance().KeyDown(KeyNum::KEY_F))
		sound->Play();
	if (Context::Instance().KeyDown(KeyNum::KEY_R))
		sound->Open(FileSystem::Instance().GetPathString("audio")+"powerup1.ogg");
	if (Context::Instance().KeyDown(KeyNum::KEY_V))
		sound->Open(FileSystem::Instance().GetPathString("audio")+"powerup2.ogg");
}

void SetCallbacks() {
	glfwSetScrollCallback(Context::Instance().Get(), [](GLFWwindow* win, double xoffset, double yoffset) {
		camera->HandleScroll(yoffset);
		#if FPS == 1
			projection = glm::perspective(
				glm::radians(camera->GetFov()),
				Context::Instance().GetRatio(),
				near,
				far
			);
		#endif
	});

	glfwSetCursorPosCallback(Context::Instance().Get(), [](GLFWwindow* win, double xpos, double ypos) {
		camera->HandleMouseCursor(xpos, ypos);
	});

	glfwSetFramebufferSizeCallback(
		Context::Instance().Get(), [](GLFWwindow*, int width, int height) {
		opengl::SetViewport(0, 0, width, height);
	});
}
