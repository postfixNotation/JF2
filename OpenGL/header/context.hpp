#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <string>

struct Context {
private:
	// change to <memory> shared_ptr<T>
	GLFWwindow *window_{ nullptr };
	GLFWmonitor *monitor_{ nullptr };
	const GLFWvidmode *kvidmode_{ nullptr };

	size_t window_width_{ 0 };
	size_t window_height_{ 0 };
	float display_ratio_{ 0 };
	std::string window_title_{ "" };

public:
	Context();

	void SetMonitorReference();
	void SetMonitorVidmode();
	void SetMonitorDimensions();
	void SetWindowTitle(const std::string &);
	void SetWindowHints() const;
	void SetWindowWidth(size_t width) { window_width_ = width; }
	void SetWindowHeight(size_t height) { window_height_ = height; }
	static void SetWindowCloseFlag(GLFWwindow *);

	GLFWwindow *GetWindow() const { return window_; }
	const GLFWmonitor *GetMonitor() const { return monitor_; }
	const size_t &GetWindowWidth() const { return window_width_; }
	const size_t &GetWindowHeight() const { return window_height_; }
	const std::string &GetWindowTitle() const { return window_title_; }
	bool GetWindowCloseFlag() const;
	float GetDisplayRatio() { return display_ratio_; }

	void CreateWindow();
	void CreateWindowDebug();
	void SwapBuffers() const;
	void PollEvents() const;
	void TerminateWindow() const;

	void FrameRateTitle() const;
};

extern Context window;

#endif // CONTEXT_HPP_
