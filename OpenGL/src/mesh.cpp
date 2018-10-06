#include <mesh.hpp>

Mesh::Mesh() : loaded_(false), number_quads_{}, indices_{ 0, 1, 2, 0, 2, 3 } {}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &vbo_);
	if (obj_loading_type_ == ObjLoadingType::QUADS) { glDeleteBuffers(1, &ibo_); }
}

std::vector<GLuint> Mesh::Split(std::string value, char delimiter) {
	std::vector<GLuint> result{};
	std::istringstream iss{};
	std::string str_buffer{};
	size_t size_t_buffer{};

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

FaceType Mesh::EvalSplitRes(const std::vector<GLuint>& input_vec, const ObjLoadingType& olt) {
	switch (olt) {
	case ObjLoadingType::TRIANGLES:
		if (input_vec.size() == 9) { return FaceType::VVTVN; }
		else if (input_vec.size() == 6) { return FaceType::VVN; }
		else if (input_vec.size() == 3) { return FaceType::V; }
		else { return FaceType::NOT_DEFINED; }
		break;
	case ObjLoadingType::QUADS:
		if (input_vec.size() == 12) { return FaceType::VVTVN; }
		else if (input_vec.size() == 8) { return FaceType::VVN; }
		else if (input_vec.size() == 4) { return FaceType::V; }
		else { return FaceType::NOT_DEFINED; }
		break;
	}

	return FaceType::NOT_DEFINED;
}

bool Mesh::LoadObj(const std::string& filename,
	const ObjLoadingType& obj_loading_type) {
	obj_loading_type_ = obj_loading_type;

	std::vector<size_t> vertex_indices, uv_indices, normal_indices;
	std::vector<glm::vec3> temp_vertices, temp_normals;
	std::vector<glm::vec2> temp_uvs;

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
				glm::vec3 vertex;
				iss >> vertex.x; iss >> vertex.y; iss >> vertex.z;
				temp_vertices.push_back(vertex);
			}
			else if (type_mesh == "vt") {
				glm::vec2 uv;
				iss >> uv.s; iss >> uv.t;
				temp_uvs.push_back(uv);
			}
			else if (type_mesh == "vn") {
				glm::vec3 normal;
				iss >> normal.x; iss >> normal.y; iss >> normal.z;
				temp_normals.push_back(glm::normalize(normal));
			}
			else if (type_mesh == "f") {
				std::vector<GLuint> face = Split(iss.str());
				FaceType ft = EvalSplitRes(face, obj_loading_type_);
				constexpr GLubyte kFaceStride{ 3 };
				const size_t kVertPerFace{ static_cast<size_t>(obj_loading_type_) };

				if (ft == FaceType::NOT_DEFINED) { break; }

				switch (ft) {
				case FaceType::VVTVN:
					for (size_t i{ 0 }; i < kVertPerFace; ++i) {
						vertex_indices.push_back(face[kFaceStride*i]);
						uv_indices.push_back(face[kFaceStride*i + 1]);
						normal_indices.push_back(face[kFaceStride*i + 2]);
					}
					break;
				case FaceType::VVN:
					for (size_t i{ 0 }; i < kVertPerFace; ++i) {
						vertex_indices.push_back(face[2 * i]);
						normal_indices.push_back(face[2 * i + 1]);
					}
					break;
				case FaceType::V:
					for (size_t i{ 0 }; i < kVertPerFace; ++i) {
						vertex_indices.push_back(face[i]);
					}
					break;
				case FaceType::NOT_DEFINED:
					return false;
				}

				if (obj_loading_type_ == ObjLoadingType::QUADS) { ++number_quads_; }
			}
		}
		file_input.close();

		for (size_t i = 0; i < vertex_indices.size(); i++) {
			Vertex mesh_vertex;

			if (temp_vertices.size() > 0) {
				glm::vec3 vertex = temp_vertices[vertex_indices[i] - 1];
				mesh_vertex.position = vertex;
			}
			if (temp_normals.size() > 0) {
				glm::vec3 normal = temp_normals[normal_indices[i] - 1];
				mesh_vertex.normal = normal;
			}
			if (temp_uvs.size() > 0) {
				glm::vec2 uv = temp_uvs[uv_indices[i] - 1];
				mesh_vertex.tex_coords = uv;
			}

			vertices_.push_back(mesh_vertex);
		}

		if (obj_loading_type_ == ObjLoadingType::QUADS) { InitIBO(); }
		InitBuffers();
		return (loaded_ = true);
	}
	return false;
}

void Mesh::InitIBO() {
	if (number_quads_ == 0) return;

	std::vector<GLuint>::iterator it{};
	std::vector<GLuint> input{};
	constexpr GLuint kIndexStride{ 4 };

	for (size_t i{}; i < number_quads_; ++i) {
		it = indices_.end();
		input = {
			*(it - 6) + kIndexStride,
			*(it - 5) + kIndexStride,
			*(it - 4) + kIndexStride,
			*(it - 3) + kIndexStride,
			*(it - 2) + kIndexStride,
			*(it - 1) + kIndexStride
		};

		indices_.insert(
			indices_.end(),
			input.begin(),
			input.end()
		);
	}
}

void Mesh::InitBuffers() {
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	if (obj_loading_type_ == ObjLoadingType::QUADS) { glGenBuffers(1, &ibo_); }

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices_.size() * sizeof(Vertex),
		vertices_.data(),
		GL_STATIC_DRAW
	);

	// vertex positions
	glBindVertexArray(vao_);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		nullptr
	);
	glEnableVertexArrayAttrib(vao_, 0);

	// normals attribute
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		reinterpret_cast<const GLvoid*>(3 * sizeof(GLfloat))
	);
	glEnableVertexArrayAttrib(vao_, 1);

	// vertex texture coordinates
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		reinterpret_cast<const GLvoid*>(6 * sizeof(GLfloat))
	);
	glEnableVertexArrayAttrib(vao_, 2);

	if (obj_loading_type_ == ObjLoadingType::QUADS) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			indices_.size() * sizeof(GLuint),
			indices_.data(),
			GL_STATIC_DRAW
		);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (obj_loading_type_ == ObjLoadingType::QUADS) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
}

void Mesh::Draw() const {
	if (!loaded_) return;

	glBindVertexArray(vao_);
	if (obj_loading_type_ == ObjLoadingType::QUADS) {
		glDrawElements(
			GL_TRIANGLES,
			number_quads_ * kVerticesPerQuad,
			GL_UNSIGNED_INT,
			nullptr
		);
	}
	else {
		glDrawArrays(
			GL_TRIANGLES,
			0,
			vertices_.size()
		);
	}
	glBindVertexArray(0);
}
