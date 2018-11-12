#ifndef VERTEX_BUFFER_HPP_
#define VERTEX_BUFFER_HPP_

#include <GL/glew.h>

enum class DrawType {
	STATIC,
	DYNAMIC
};

class VertexBuffer {
private:
	GLuint handle_;
public:
	VertexBuffer(
		const GLvoid* data,
		GLuint size,
		DrawType draw_type = DrawType::STATIC
	);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void BufferSubData(const GLvoid* data, GLuint size) const;
};

#endif // VERTEX_BUFFER_HPP_
