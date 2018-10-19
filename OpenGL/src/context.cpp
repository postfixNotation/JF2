#include <context.hpp>

Context::Context() {
	if (!glfwInit()) {
		std::cerr << "ERROR: COULD NOT START GLFW3" << std::endl;
	}
}

void Context::SetMonitorReference() {
	monitor_ = glfwGetPrimaryMonitor();
}

void Context::SetMonitorVidmode() {
	kvidmode_ = glfwGetVideoMode(monitor_);
}

void Context::SetMonitorDimensions() {
	window_width_ = kvidmode_->width;
	window_height_ = kvidmode_->height;
	display_ratio_ = static_cast<float>(window_width_) / static_cast<float>(window_height_);
}

void Context::SetWindowTitle(const std::string &wt) {
	window_title_ = wt;
}

void Context::SetWindowHints() const {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RED_BITS, kvidmode_->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, kvidmode_->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, kvidmode_->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, kvidmode_->refreshRate);
}

void Context::SetWindowCloseFlag(GLFWwindow *win) {
	glfwSetWindowShouldClose(win, 1);
}

bool Context::GetWindowCloseFlag() const {
	return (glfwWindowShouldClose(window_) == 0);
}

void Context::CreateWindow() {
	// Exception handling might be
	// appropriate

	window_ = glfwCreateWindow(
		window_width_,
		window_height_,
		window_title_.c_str(),
		monitor_,
		nullptr
	);

	if (!window_) {
		std::cerr << "ERROR: COULD NOT OPEN WINDOW WITH GLFW3" << std::endl;
		TerminateWindow();
	}

	glfwMakeContextCurrent(window_);
}

void Context::FrameRateTitle() const {
	static size_t number_of_frames{};
	static double time_elapsed{};
	static double time_previous{};
	static std::ostringstream title_stream{};
	title_stream.precision(4);

	++number_of_frames;
	time_elapsed = glfwGetTime();

	if ((time_elapsed - time_previous) > 0.5) {
		double fps = number_of_frames / (time_elapsed - time_previous);
		title_stream << "Snake Unlimited - FPS: ";
		title_stream << fps;
		glfwSetWindowTitle(window_, title_stream.str().c_str());

		title_stream.clear();
		title_stream.str("");
		number_of_frames = 0;
		time_previous = time_elapsed;
	}
}

void Context::CreateWindowDebug() {
	window_ = glfwCreateWindow(
		window_width_ / 2,
		window_height_ / 2,
		window_title_.c_str(),
		nullptr,
		nullptr
	);

	if (!window_) {
		std::cerr << "ERROR: COULD NOT OPEN WINDOW WITH GLFW3" << std::endl;
		TerminateWindow();
	}

	glfwMakeContextCurrent(window_);
}

void Context::SwapBuffers() const { glfwSwapBuffers(window_); }
void Context::PollEvents() const { glfwPollEvents(); }
void Context::TerminateWindow() const { glfwTerminate(); }

Context window;