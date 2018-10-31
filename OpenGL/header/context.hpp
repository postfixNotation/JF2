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
public:
	~Context();

	static Context& Instance();
	void Create(const context::Config &config);
	GLFWwindow* Get() const { return handle_; }
	inline void SetCloseFlag() { glfwSetWindowShouldClose(handle_, GLFW_TRUE); }
	inline int GetCloseFlag() const { return glfwWindowShouldClose(handle_); }
	operator bool() const { return GetCloseFlag(); }
	bool operator!() const { return !GetCloseFlag(); }

	void SetCursorMode(bool cursor_enabled);
	inline void SetCursorPos(double xpos, double ypos) { glfwSetCursorPos(handle_, xpos, ypos); }
	inline size_t GetWidth() const { return width_; }
	inline size_t GetHeight() const { return height_; }
	inline float GetRatio() { return ratio_; }
	float GetFrameRate(size_t precision) const;
	void UpdateVideoMode();
	void UpdateDimensions();

	inline bool KeyDown(int key) { return (glfwGetKey(handle_, key) == GLFW_PRESS); }
	void SwapBuffers() const { glfwSwapBuffers(handle_); }
	void PollEvents() const { glfwPollEvents(); }
	void Terminate();
private:
	Context();
	bool Init();
	void SetHints(
		size_t major,
		size_t minor,
		size_t samples,
		bool debug) const;

	GLFWwindow *handle_;
	GLFWmonitor *monitor_;
	GLFWvidmode *video_mode_;

	size_t height_;
	size_t width_;
	float ratio_;
};

#endif // CONTEXT_HPP_
