#include <vertex_array.hpp>

VertexArray::VertexArray() { glGenVertexArrays(1, &handle_); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &handle_); }

void VertexArray::Bind() const { glBindVertexArray(handle_); }
void VertexArray::Unbind() const { glBindVertexArray(0); }

void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout) {
	Bind();
	vbo.Bind();
	const auto& elements = layout.GetElements();
	GLuint offset{ 0 };
	for (size_t i{}; i < elements.size(); ++i) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(
			i,
			element.count,
			element.type,
			element.normalized,
			layout.GetStride(),
			reinterpret_cast<GLvoid*>(offset)
		);
		offset += element.GetDeltaOffset();
	}
}
