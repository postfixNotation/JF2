#include "vertex_buffer.hpp"

namespace nxt {
	VertexBuffer::VertexBuffer(const GLvoid* data, GLuint size, DrawType draw_type) {
		glGenBuffers(1, &handle_);
		glBindBuffer(GL_ARRAY_BUFFER, handle_);
		if (DrawType::STATIC == draw_type)
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		else if (DrawType::DYNAMIC == draw_type)
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &handle_);
	}

	void VertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, handle_);
	}

	void VertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::BufferSubData(const GLvoid* data, GLuint size) const {
		glBufferSubData(
			GL_ARRAY_BUFFER,
			0,
			size,
			data
		);
	}
}
