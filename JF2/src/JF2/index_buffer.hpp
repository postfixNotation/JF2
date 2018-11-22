#ifndef INDEX_BUFFER_HPP_
#define INDEX_BUFFER_HPP_

#include <GL/glew.h>

class IndexBuffer {
public:
	IndexBuffer(const GLuint* data, GLuint count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	inline GLuint GetCount() const { return count_; }
private:
	GLuint handle_;
	GLuint count_;
};

#endif // INDEX_BUFFER_HPP_
