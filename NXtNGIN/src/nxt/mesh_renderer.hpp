#ifndef MESH_HPP_
#define MESH_HPP_

#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "renderer.hpp"

namespace nxt {
	struct Vertex {
		glm::fvec3 position;
		glm::fvec3 normal;
		glm::fvec2 tex_coords;
	};

	enum class FaceType {
		V,
		VVN,
		VVTVN,
		NOT_DEFINED
	};

	class MeshRenderer {
	private:
		static constexpr GLuint kVerticesPerQuad{ 6 };
		static constexpr GLuint kTriangleStride{ 3 };
		static constexpr GLuint kQuadStride{ 4 };

		std::vector<Vertex> vertices_;
		std::vector<GLuint> indices_;
		size_t face_count_;
		bool is_face_quad_;
		bool loaded_;

		std::shared_ptr<Shader> shader_;
		std::shared_ptr<IndexBuffer> ib_;
		std::shared_ptr<VertexArray> va_;

		static FaceType EvalSplitRes(
			const std::vector<GLuint>& input_vec,
			bool is_face_quad = true);
		static std::vector<GLuint> Split(
			std::string value,
			char delimiter = '/');
		void InitIndices();
		void InitBuffers();
	public:
		MeshRenderer(std::shared_ptr<Shader>);
		~MeshRenderer();

		bool Load(
			const std::string& filename,
			bool is_face_quad = true);
		void Draw(
			GLsizei count = 1,
			std::shared_ptr<Shader> shader = std::shared_ptr<Shader>{ nullptr }) const;
	};
}

#endif //MESH_HPP_
