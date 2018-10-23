#ifndef GL_HPP_
#define GL_HPP_

#include <iostream>
#include <GL/glew.h>

namespace opengl {
	bool Init();
	bool SetDefaultSetting();
	bool SetViewport(
		GLint xpos,
		GLint ypos,
		GLsizei width,
		GLsizei height);
	void SetColor(
		GLclampf r,
		GLclampf g,
		GLclampf b,
		GLclampf a);
	void DebugMessageCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar *message,
		const void *userParam);
}

#endif // GL_HPP_
