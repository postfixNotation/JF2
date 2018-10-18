#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <GL/glew.h>

#include <vertex_array.hpp>
#include <index_buffer.hpp>
#include <shader.hpp>

enum class ClearBufferBit {
	COLOR,
	DEPTH,
	COLOR_DEPTH
};

class Renderer {
private:
public:
	Renderer() = delete;
	static void Clear(ClearBufferBit mask = ClearBufferBit::COLOR_DEPTH);
	static void Render(
		const VertexArray& va,
		const IndexBuffer& ib,
		const Shader& shader
	);
};

#endif // RENDERER_HPP_
