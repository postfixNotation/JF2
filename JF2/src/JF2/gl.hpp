#ifndef GL_HPP_
#define GL_HPP_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace jf2 {
	namespace opengl {
		bool Init();
		bool SetDefaultSetting();
		void SetViewport(
			GLint xpos,
			GLint ypos,
			GLsizei width,
			GLsizei height);
		void SetColor(
			GLclampf r,
			GLclampf g,
			GLclampf b,
			GLclampf a);
		void PolygonMode();
		void FillMode();
		void SetCubeMapMode();
		void ResetCubeMapMode();
		void SetDebugMessageCallback(void(*callback)(
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar *message,
			const void *userParam));
		void APIENTRY DebugMessageCallback(
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar *message,
			const void *userParam);
	}
}

#endif // GL_HPP_
