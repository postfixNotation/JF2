#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <config.hpp>

#include <iostream>
#include <cassert>
#include <string>
#include <cmath>

struct Context {
private:
	bool Init();
	void SetHints(
		size_t major,
		size_t minor,
		size_t samples,
		bool debug) const;

	GLFWwindow *window_;
	GLFWmonitor *monitor_;
	GLFWvidmode *video_mode_;

	size_t height_;
	size_t width_;
	float ratio_;
public:
	Context();
	~Context();

	void Create(const Config &config);
	GLFWwindow* Get() const { return window_; }
	inline void SetCloseFlag() { glfwSetWindowShouldClose(window_, GLFW_TRUE); }
	inline int GetCloseFlag() const { return glfwWindowShouldClose(window_); }
	operator bool() const { return GetCloseFlag(); }
	bool operator!() const { return !GetCloseFlag(); }

	void SetCursorMode(bool cursor_enabled);
	inline void SetCursorPos(double xpos, double ypos) { glfwSetCursorPos(window_, xpos, ypos); }
	inline size_t GetWidth() const { return width_; }
	inline size_t GetHeight() const { return height_; }
	inline float GetRatio() { return ratio_; }
	float GetFrameRate(size_t precision) const;
	void UpdateVideoMode();
	void UpdateDimensions();

	void SwapBuffers() const { glfwSwapBuffers(window_); }
	void PollEvents() const { glfwPollEvents(); }
	void Terminate();
};

#endif // CONTEXT_HPP_
