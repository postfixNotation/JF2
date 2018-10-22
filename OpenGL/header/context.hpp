#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <string>
#include <cmath>

using OpenGLMajor = size_t;
using OpenGLMinor = size_t;
using NumberOfSamples = size_t;

enum class ContextSize {
	DEBUG,
	MAXIMIZED,
	FULLSCREEEN
};

enum class ContextStates {
	ENABLED,
	DISABLED
};

struct Context {
private:
	bool Init();

	GLFWwindow *window_;
	GLFWmonitor *monitor_;
	GLFWvidmode *video_mode_;

	ContextSize contextsize;
	size_t height_;
	size_t width_;
	float ratio_;
public:
	Context();
	~Context();
	void SetHints(
		OpenGLMajor major,
		OpenGLMinor minor,
		NumberOfSamples samples,
		bool resizable,
		bool debug) const;
	void Create(const std::string &title, ContextSize size = ContextSize::DEBUG);

	GLFWwindow* Get() const { return window_; }
	inline void SetCloseFlag() { glfwSetWindowShouldClose(window_, GLFW_TRUE); }
	inline int GetCloseFlag() const { return glfwWindowShouldClose(window_); }
	operator bool() const { return GetCloseFlag(); }
	bool operator!() const { return !GetCloseFlag(); }

	inline void SetCursorPos(double xpos, double ypos) { glfwSetCursorPos(window_, xpos, ypos); }
	inline size_t GetWidth() const { return width_; }
	inline size_t GetHeight() const { return height_; }
	inline float GetRatio() { return ratio_; }
	float GetFrameRate(size_t precision) const;
	void UpdateVideoMode();
	void UpdateDimensions();
	void SetCursorMode(ContextStates state = ContextStates::ENABLED);

	void SwapBuffers() const { glfwSwapBuffers(window_); }
	void PollEvents() const { glfwPollEvents(); }
	void Terminate();
};

#endif // CONTEXT_HPP_
