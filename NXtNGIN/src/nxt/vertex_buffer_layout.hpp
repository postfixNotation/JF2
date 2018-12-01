#ifndef VERTEX_BUFFER_LAYOUT_HPP_
#define VERTEX_BUFFER_LAYOUT_HPP_

#include <GL/glew.h>
#include <cassert>
#include <vector>

namespace nxt {
	struct VertexBufferElement {
		GLenum type;
		GLuint count; // components per vertex
		GLboolean normalized;

		GLuint GetDeltaOffset() const {
			switch (type) {
			case GL_FLOAT: return (count * sizeof(GLfloat));
			case GL_UNSIGNED_INT: return (count * sizeof(GLuint));
			case GL_UNSIGNED_BYTE: return (count * sizeof(GLubyte));
			default: assert(false); return 0;
			}
		}
	};

	class VertexBufferLayout {
	private:
		GLuint stride_;
		std::vector<VertexBufferElement> elements_;
	public:
		VertexBufferLayout() : stride_{ 0 } {}
		template <typename T>
		void Push(GLuint count);
		inline const std::vector<VertexBufferElement>& GetElements() const { return elements_; }
		inline GLuint GetStride() const { return stride_; }
	};
}

#endif // VERTEX_BUFFER_LAYOUT_HPP_
