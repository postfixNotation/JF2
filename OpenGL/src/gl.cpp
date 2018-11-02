#include <gl.hpp>

namespace opengl {
	bool Init() {
		glewExperimental = GL_TRUE;
		if (GLEW_OK != glewInit()) {
			std::cerr << "ERROR: COULD NOT INITIALIZE GLEW" << std::endl;
			return false;
		}
		glGetError();
		return true;
	}

	bool SetDefaultSetting() {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		return (GL_NO_ERROR == glGetError());
	}

	void SetViewport(
		GLint xpos,
		GLint ypos,
		GLsizei width,
		GLsizei height) {
		glViewport(0, 0, width, height);
	}

	void SetColor(
		GLclampf r,
		GLclampf g,
		GLclampf b,
		GLclampf a) {
		glClearColor(r, g, b, a);
	}

	void PolygonMode() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }

	void FillMode() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

	void SetDebugMessageCallback(void APIENTRY (*callback)(GLenum source, GLenum type,
		GLuint id, GLenum severity, GLsizei length,
		const GLchar *message, const void *userParam)) {

		static GLint flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(callback, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
				GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
	}

	void APIENTRY DebugMessageCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam) {

		// IGNORE NON-SIGNIFICANT ERROR/WARNING CODES
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::cout << std::endl;
		std::cout << "DEBUG MESSAGE (" << id << "): " << message << std::endl;

		switch (source) {
		case GL_DEBUG_SOURCE_API:				std::cout << "SOURCE: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		std::cout << "SOURCE: WINDOW SYSTEM"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	std::cout << "SOURCE: SHADER COMPILER"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:		std::cout << "SOURCE: THIRD PARTY"; break;
		case GL_DEBUG_SOURCE_APPLICATION:		std::cout << "SOURCE: APPLICATION"; break;
		case GL_DEBUG_SOURCE_OTHER:				std::cout << "SOURCE: OTHER"; break;
		}
		std::cout << std::endl;

		switch (type) {
		case GL_DEBUG_TYPE_ERROR:				std::cout << "TYPE: ERROR"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	std::cout << "TYPE: DEPRECATED BEHAVIOUR"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	std::cout << "TYPE: UNDEFINED BEHAVIOUR"; break;
		case GL_DEBUG_TYPE_PORTABILITY:			std::cout << "TYPE: PORTABILITY"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:			std::cout << "TYPE: PERFORMANCE"; break;
		case GL_DEBUG_TYPE_MARKER:				std::cout << "TYPE: MARKER"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:			std::cout << "TYPE: PUSH GROUP"; break;
		case GL_DEBUG_TYPE_POP_GROUP:			std::cout << "TYPE: POP GROUP"; break;
		case GL_DEBUG_TYPE_OTHER:				std::cout << "TYPE: OTHER"; break;
		}
		std::cout << std::endl;

		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:			std::cout << "SEVERITY: HIGH"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:			std::cout << "SEVERITY: MEDIUM"; break;
		case GL_DEBUG_SEVERITY_LOW:				std::cout << "SEVERITY: LOW"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	std::cout << "SEVERITY: NOTIFICATION"; break;
		}
		std::cout << std::endl << std::endl;
	}
}
