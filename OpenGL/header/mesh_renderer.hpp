#ifndef MESH_HPP_
#define MESH_HPP_

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>

constexpr size_t kVerticesPerQuad{ 6 };

struct Vertex {
	glm::vec3 position{};
	glm::vec3 normal{};
	glm::vec2 tex_coords{};
};

struct Color {
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
};

enum class ObjLoadingType {
	TRIANGLES = 3,
	QUADS = 4
};

enum class FaceType {
	V,
	VVN,
	VVTVN,
	NOT_DEFINED
};

// inherit from abstract class
class MeshRenderer {
private:
	ObjLoadingType obj_loading_type_;
	std::vector<Vertex> vertices_;
	std::vector<GLuint> indices_;
	GLuint vbo_, vao_, ibo_;
	size_t number_quads_;
	bool loaded_;

	static FaceType EvalSplitRes(
		const std::vector<GLuint>& input_vec,
		const ObjLoadingType& olt = ObjLoadingType::QUADS
	);
	static std::vector<GLuint> Split(
		std::string value,
		char delimiter = '/'
	);
	void InitIBO();
	void InitBuffers();
public:
	MeshRenderer();
	~MeshRenderer();

	bool LoadObj(
		const std::string& filename,
		const ObjLoadingType& obj_loading_type = ObjLoadingType::QUADS
	);
	void Draw() const;
};

#endif //MESH_HPP_

