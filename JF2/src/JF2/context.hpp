#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <iostream>
#include <memory>
#include <string>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb-master/stb_image.h>

#include "non_copyable.hpp"
#include "non_moveable.hpp"
#include "config.hpp"
#include "keys.hpp"

namespace jf2 {
	struct Context : public NonCopyable, public NonMoveable {
	public:
		~Context();

		static Context& Instance();
		GLFWwindow* Get() const;
		operator bool() const;
		bool operator!() const;

		bool KeyDown(KeyNum key);
		bool KeyDown(MouseButton button);
		void Create(const context::Config &config);
		void SetCloseFlag();
		void SetCursorMode(bool cursor_enabled);
		void SetIcon(const std::string& file) const;
		void SetCursorPos(float xpos, float ypos);
		void UpdateVideoMode();
		void UpdateDimensions();
		void SwapBuffers() const;
		void PollEvents() const;
		void Terminate();

		int GetWidth() const;
		int GetHeight() const;
		int GetCloseFlag() const;

		float GetFrameRate(size_t precision) const;
		float GetRatio();
		float GetTime() const;
		float GetTimePerFrame() const;
	private:
		Context();
		bool Init();
		void SetHints(
			int major, int minor,
			int samples, bool debug) const;

		GLFWwindow *handle_;
		GLFWmonitor *monitor_;
		GLFWvidmode *video_mode_;

		int height_;
		int width_;
		float ratio_;
	};
}

#endif // CONTEXT_HPP_
