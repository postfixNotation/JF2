#include <vertex_buffer_layout.hpp>

template<typename T>
void VertexBufferLayout::Push(GLuint count) { assert(false); }

template<>
void  VertexBufferLayout::Push<GLfloat>(GLuint count) {
	elements_.push_back({ GL_FLOAT, count, GL_FALSE });
	stride_ += count * sizeof(GLfloat);
}

template<>
void  VertexBufferLayout::Push<GLuint>(GLuint count) {
	elements_.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
	stride_ += count * sizeof(GLuint);
}

template<>
void  VertexBufferLayout::Push<GLubyte>(GLuint count) {
	elements_.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
	stride_ += count * sizeof(GLubyte);
}
