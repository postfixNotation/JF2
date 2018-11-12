#ifndef INDEX_BUFFER_HPP_
#define INDEX_BUFFER_HPP_

#include <GL/glew.h>

class IndexBuffer {
private:
	GLuint handle_;
	GLuint count_;
public:
	IndexBuffer(const GLuint* data, GLuint count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	inline GLuint GetCount() const { return count_; }
};

#endif // INDEX_BUFFER_HPP_
