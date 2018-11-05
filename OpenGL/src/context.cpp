#include <context.hpp>

Context& Context::Instance() {
	static std::unique_ptr<Context> instance = std::unique_ptr<Context>(new Context());
	return *instance;
}

Context::Context() {
	if (!glfwInit()) {
		std::cerr << "ERROR: COULD NOT START GLFW3" << std::endl;
	}
	if (!Init()) {
		std::cerr << "ERROR: COULD NOT INITIALIZE CONTEXT" << std::endl;
	}
}

Context::~Context() { Terminate(); }

void Context::Terminate() {
	glfwDestroyWindow(handle_);
	handle_ = nullptr;
	glfwTerminate();
}

bool Context::Init() {
	monitor_ = glfwGetPrimaryMonitor();
	UpdateVideoMode();

	width_ = video_mode_->width;
	height_ = video_mode_->height;
	ratio_ = static_cast<float>(width_) / height_;

	if (monitor_ && video_mode_) return true;
	return false;
}

void Context::UpdateVideoMode() { video_mode_ = const_cast<GLFWvidmode*>(glfwGetVideoMode(monitor_)); }

void Context::SetHints(
	size_t major,
	size_t minor,
	size_t samples,
	bool debug) const {
	glfwWindowHint(GLFW_SAMPLES, samples);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RED_BITS, video_mode_->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, video_mode_->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, video_mode_->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, video_mode_->refreshRate);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, static_cast<size_t>(debug));
}

void Context::SetCursorMode(bool cursor_enabled) {
	if (cursor_enabled) {
		glfwSetInputMode(handle_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(handle_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void Context::Create(const context::Config &config) {
	SetHints(
		config.opengl_major,
		config.opengl_minor,
		config.number_of_samples,
		config.debug_context
	);
	switch (config.context_size) {
		case context::Size::DEBUG:
			handle_ = glfwCreateWindow(
				(width_ = width_ / 2),
				(height_ = height_ / 2),
				config.context_title.c_str(),
				nullptr,
				nullptr
			);
			break;
		case context::Size::MAXIMIZED:
			handle_ = glfwCreateWindow(
				width_,
				height_,
				config.context_title.c_str(),
				nullptr,
				nullptr
			);
			break;
		case context::Size::FULLSCREEEN:
			handle_ = glfwCreateWindow(
				width_,
				height_,
				config.context_title.c_str(),
				monitor_,
				nullptr
			);
			break;
	}

	if (!handle_) {
		std::cerr << "ERROR: COULD NOT OPEN WINDOW WITH GLFW3" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(handle_);
	SetCursorMode(config.cusor_enabled);
}

void Context::UpdateDimensions() {
	UpdateVideoMode();
	width_ = video_mode_->width;
	height_ = video_mode_->height;
	ratio_ = static_cast<float>(width_) / height_;
}

float Context::GetFrameRate(size_t precision) const {
	static size_t number_of_frames;
	static double time_elapsed;
	static double time_previous;
	static double fps;

	++number_of_frames;
	time_elapsed = glfwGetTime();

	if ((time_elapsed - time_previous) > 0.5) {
		fps = number_of_frames / (time_elapsed - time_previous);
		number_of_frames = 0;
		time_previous = time_elapsed;
	}
	fps = static_cast<int>(fps * pow(10, precision) + 0.5) / pow(10, precision);
	return static_cast<float>(fps);
}

void Context::SetIcon(const std::string& file) const {
	std::unique_ptr<GLFWimage> image{ new GLFWimage() };
	int width, height, components;
	unsigned char *image_data;

	image_data = stbi_load(
		file.c_str(),
		&width,
		&height,
		&components,
		0
	);
	if (image_data == nullptr) return;

	image->width = width;
	image->height = height;
	image->pixels = image_data;

	glfwSetWindowIcon(handle_, 1, image.get());
}
