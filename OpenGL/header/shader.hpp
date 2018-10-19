#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
	GLuint handle_{};
	std::map<std::string, GLint> uniform_locs_;
	enum class Type { VERTEX, FRAGMENT, PROGRAM };

	std::string LoadFile(const std::string&);
	GLint GetUniformLocation(const std::string &name);
	bool CheckCompileErrors(GLuint, Type) const;

public:
	Shader(const std::string&, const std::string&);
	~Shader() { glDeleteProgram(handle_); }
	Shader& operator=(const Shader&);

	void Bind() const { glUseProgram(handle_); }
	void Unbind() const { glUseProgram(0); }

	// move to a generic GL class
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
