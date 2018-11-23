#include "mesh_renderer.hpp"

namespace jf2 {
	MeshRenderer::MeshRenderer(std::shared_ptr<Shader> shader) :
		shader_{ shader }, loaded_{ false }, face_count_{} {}

	MeshRenderer::~MeshRenderer() {}

	std::vector<GLuint> MeshRenderer::Split(
		std::string value,
		char delimiter) {

		std::vector<GLuint> result{};
		std::istringstream iss{};
		std::string str_buffer{};
		GLuint size_t_buffer{};

		std::replace(
			begin(value),
			end(value),
			delimiter,
			' ');

		iss.str(value);
		iss >> str_buffer;
		while (iss >> size_t_buffer) {
			result.push_back(size_t_buffer);
		}
		return result;
	}

	FaceType MeshRenderer::EvalSplitRes(
		const std::vector<GLuint>& input_vec,
		bool is_face_quad) {

		if (is_face_quad) {
			if (input_vec.size() == 12) { return FaceType::VVTVN; }
			else if (input_vec.size() == 8) { return FaceType::VVN; }
			else if (input_vec.size() == 4) { return FaceType::V; }
			else { return FaceType::NOT_DEFINED; }
		}
		else {
			if (input_vec.size() == 9) { return FaceType::VVTVN; }
			else if (input_vec.size() == 6) { return FaceType::VVN; }
			else if (input_vec.size() == 3) { return FaceType::V; }
			else { return FaceType::NOT_DEFINED; }
		}

		return FaceType::NOT_DEFINED;
	}

	bool MeshRenderer::Load(
		const std::string& filename,
		bool is_face_quad) {

		is_face_quad_ = is_face_quad;
		std::vector<size_t> vertex_indices, uv_indices, normal_indices;
		std::vector<glm::fvec3> temp_vertices, temp_normals;
		std::vector<glm::fvec2> temp_uvs;

		if (filename.find(".obj") != std::string::npos) {
			std::ifstream file_input(filename, std::ios::in);
			if (!file_input) {
				std::cerr << "CANNOT OPEN " << filename << std::endl;
				return false;
			}
			std::cout << "LOADING OBJ FILE " << filename << std::endl;

			std::string line_buffer{};
			std::string type_mesh{};
			std::istringstream iss{};

			while (std::getline(file_input, line_buffer)) {
				iss.clear();
				iss.str(line_buffer);
				type_mesh.clear();
				iss >> type_mesh;

				if (type_mesh == "v") {
					glm::fvec3 vertex;
					iss >> vertex.x; iss >> vertex.y; iss >> vertex.z;
					temp_vertices.push_back(std::move(vertex));
				}
				else if (type_mesh == "vt") {
					glm::fvec2 uv;
					iss >> uv.s; iss >> uv.t;
					temp_uvs.push_back(std::move(uv));
				}
				else if (type_mesh == "vn") {
					glm::fvec3 normal;
					iss >> normal.x; iss >> normal.y; iss >> normal.z;
					temp_normals.push_back(std::move(glm::normalize(normal)));
				}
				else if (type_mesh == "f") {
					std::vector<GLuint> face = Split(iss.str());
					FaceType face_type = EvalSplitRes(face, is_face_quad_);
					const size_t kVertPerFace = is_face_quad_ ? 4 : 3;

					if (face_type == FaceType::NOT_DEFINED) break;

					switch (face_type) {
					case FaceType::VVTVN:
						for (size_t i{ 0 }; i < kVertPerFace; ++i) {
							vertex_indices.push_back(std::move(face[3 * i]));
							uv_indices.push_back(std::move(face[3 * i + 1]));
							normal_indices.push_back(std::move(face[3 * i + 2]));
						}
						break;
					case FaceType::VVN:
						for (size_t i{ 0 }; i < kVertPerFace; ++i) {
							vertex_indices.push_back(std::move(face[2 * i]));
							normal_indices.push_back(std::move(face[2 * i + 1]));
						}
						break;
					case FaceType::V:
						for (size_t i{ 0 }; i < kVertPerFace; ++i) {
							vertex_indices.push_back(std::move(face[i]));
						}
						break;
					case FaceType::NOT_DEFINED:
						return false;
					}

					++face_count_;
				}
			}
			file_input.close();

			for (size_t i{ 0 }; i < vertex_indices.size(); i++) {
				Vertex mesh_vertex;

				if (temp_vertices.size() > 0) {
					glm::fvec3 vertex = temp_vertices[vertex_indices[i] - 1];
					mesh_vertex.position = vertex;
				}
				if (temp_normals.size() > 0) {
					glm::fvec3 normal = temp_normals[normal_indices[i] - 1];
					mesh_vertex.normal = normal;
				}
				if (temp_uvs.size() > 0) {
					glm::fvec2 uv = temp_uvs[uv_indices[i] - 1];
					mesh_vertex.tex_coords = uv;
				}

				vertices_.push_back(std::move(mesh_vertex));
			}

			if (is_face_quad_) {
				indices_ = { 0, 1, 2, 0, 2, 3 };
			}
			else {
				indices_ = { 0, 1, 2 };
			}
			InitIndices();
			InitBuffers();
			return (loaded_ = true);
		}
		return false;
	}

	void MeshRenderer::InitIndices() {
		std::vector<GLuint>::iterator it{};
		std::vector<GLuint> input{};

		if (is_face_quad_) {
			for (size_t i{ 1 }; i < face_count_; ++i) {
				it = indices_.end();
				input = {
					*(it - 6) + kQuadStride,
					*(it - 5) + kQuadStride,
					*(it - 4) + kQuadStride,
					*(it - 3) + kQuadStride,
					*(it - 2) + kQuadStride,
					*(it - 1) + kQuadStride
				};

				indices_.insert(
					indices_.end(),
					input.begin(),
					input.end());
			}
		}
		else {
			for (size_t i{ 1 }; i < face_count_; ++i) {
				it = indices_.end();
				input = {
					*(it - 3) + kTriangleStride,
					*(it - 2) + kTriangleStride,
					*(it - 1) + kTriangleStride
				};

				indices_.insert(
					indices_.end(),
					input.begin(),
					input.end());
			}
		}
	}

	void MeshRenderer::InitBuffers() {
		VertexBuffer vb{
			vertices_.data(),
			static_cast<GLuint>(vertices_.size() * sizeof(Vertex)) };

		VertexBufferLayout vbl{};
		// vertex positions
		vbl.Push<GLfloat>(3);
		// normals attribute
		vbl.Push<GLfloat>(3);
		// vertex texture coordinates
		vbl.Push<GLfloat>(2);

		va_ = std::make_shared<VertexArray>(vb, vbl);
		ib_ = std::make_shared<IndexBuffer>(indices_.data(), indices_.size());

		ib_->Unbind();
		va_->Unbind();
		vb.Unbind();
	}

	void MeshRenderer::Draw(
		GLsizei count,
		std::shared_ptr<Shader> shader) const {
		if (!loaded_) return;
		if (shader.get() != nullptr) {
			Renderer::Render(*va_, *ib_, *shader, count);
		}
		else {
			Renderer::Render(*va_, *ib_, *shader_, count);
		}
	}
}
