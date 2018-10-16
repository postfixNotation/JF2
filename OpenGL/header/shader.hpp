#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
	GLuint handle_{};
	std::string LoadFile(const std::string);
	enum class Type { VERTEX, FRAGMENT, PROGRAM };
	bool CheckCompileErrors(GLuint, Type) const;

public:
	Shader();
	Shader& operator=(const Shader&);
	Shader(std::string, std::string);
	virtual ~Shader();

	void Use() const;
	static bool Init();
	GLuint GetHandle() const { return handle_; }

	// shader uniform methods
	void SetBool(const std::string &name, GLboolean value) const;
	void SetInt(const std::string &name, GLint value) const;
	void SetFloat(const std::string &name, GLfloat value) const;
	void SetVec2(const std::string &name, const glm::vec2 &value) const;
	void SetVec2(const std::string &name, GLfloat x, GLfloat y) const;
	void SetVec3(const std::string &name, const glm::vec3 &value) const;
	void SetVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) const;
	void SetVec4(const std::string &name, const glm::vec4 &value) const;
	void SetVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;
	void SetMat2(const std::string &name, const glm::mat2 &mat) const;
	void SetMat3(const std::string &name, const glm::mat3 &mat) const;
	void SetMat4(const std::string &name, const glm::mat4 &mat) const;

};

#endif // SHADER_HPP_
