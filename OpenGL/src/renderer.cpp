#include <iostream>
#include <renderer.hpp>

void Renderer::Clear(ClearBufferBit mask) {
	switch(mask) {
		case ClearBufferBit::COLOR:
			glClear(GL_COLOR_BUFFER_BIT);
		break;
		case ClearBufferBit::DEPTH:
			glClear(GL_DEPTH_BUFFER_BIT);
		break;
		case ClearBufferBit::COLOR_DEPTH:
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		break;
		default:
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void Renderer::Render(
	const VertexArray& va,
	const IndexBuffer& ib,
	const Shader& shader
) {
	shader.Bind();
	va.Bind();
	ib.Bind();
	glDrawElements(
		GL_TRIANGLES,
		ib.GetCount(),
		GL_UNSIGNED_INT,
		nullptr
	);
}
