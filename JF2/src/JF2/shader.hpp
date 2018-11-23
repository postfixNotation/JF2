#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "filesystem.hpp"

namespace jf2 {
	class Shader {
	public:
		Shader(const std::string&, const std::string&);
		~Shader() { glDeleteProgram(handle_); }
		Shader& operator=(const Shader&);

		void Bind() const { glUseProgram(handle_); }
		void Unbind() const { glUseProgram(0); }
		GLuint GetHandle() const { return handle_; }

		// shader uniform methods
		void SetBool(const std::string &name, GLboolean value);
		void SetInt(const std::string &name, GLint value);
		void SetFloat(const std::string &name, GLfloat value);
		void SetVec2(const std::string &name, const glm::fvec2 &value);
		void SetVec2(const std::string &name, GLfloat x, GLfloat y);
		void SetVec3(const std::string &name, const glm::fvec3 &value);
		void SetVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z);
		void SetVec4(const std::string &name, const glm::fvec4 &value);
		void SetVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		void SetMat2(const std::string &name, const glm::fmat2 &mat);
		void SetMat3(const std::string &name, const glm::fmat3 &mat);
		void SetMat4(const std::string &name, const glm::fmat4 &mat);
	private:
		GLuint handle_{};
		std::map<std::string, GLint> uniform_locs_;
		enum class Type { VERTEX, FRAGMENT, PROGRAM };
		bool CheckCompileErrors(GLuint, Type) const;
		GLint GetUniformLocation(const std::string &name);
	};
}

#endif // SHADER_HPP_
