#include "index_buffer.hpp"

IndexBuffer::IndexBuffer(const GLuint* data, GLuint count) : count_{ count } {
	glGenBuffers(1, &handle_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		count * sizeof(GLuint),
		reinterpret_cast<const GLvoid*>(data),
		GL_STATIC_DRAW
	);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &handle_);
}

void IndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_);
}

void IndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
