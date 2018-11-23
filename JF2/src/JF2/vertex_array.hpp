#ifndef VERTEX_ARRAY_HPP_
#define VERTEX_ARRAY_HPP_

#include "vertex_buffer.hpp"
#include "vertex_buffer_layout.hpp"

namespace jf2 {
	class VertexArray {
	private:
		GLuint handle_;
	public:
		VertexArray();
		VertexArray(
			const VertexBuffer& vbo,
			const VertexBufferLayout& layout
		) : VertexArray() {
			AddBuffer(vbo, layout);
		}
		~VertexArray();
		void Bind() const;
		void Unbind() const;
		void AddBuffer(
			const VertexBuffer& vbo,
			const VertexBufferLayout& layout
		);
	};
}

#endif // VERTEX_ARRAY_HPP_
