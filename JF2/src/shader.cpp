#include <shader.hpp>

Shader& Shader::operator=(const Shader &shader) {
	if (this == &shader) return *this;
	handle_ = shader.GetHandle();
	return *this;
}

GLint Shader::GetUniformLocation(const std::string &name) {
	if (uniform_locs_.find(name) != uniform_locs_.end()) {
		return uniform_locs_[name];
	}
	uniform_locs_[name] = glGetUniformLocation(handle_, name.c_str());
	assert(uniform_locs_[name] != -1);
	return (uniform_locs_[name]);
}

Shader::Shader(const std::string &vert_file, const std::string &frag_file) {
	std::string vertex_string = FileSystem::Instance().GetContent(vert_file);
	std::string fragment_string = FileSystem::Instance().GetContent(frag_file);

	const GLchar* vertex_src = vertex_string.c_str();
	const GLchar* fragment_src = fragment_string.c_str();

	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vertex_src, nullptr);
	glCompileShader(vert_shader);
	CheckCompileErrors(vert_shader, Type::VERTEX);

	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &fragment_src, nullptr);
	glCompileShader(frag_shader);
	CheckCompileErrors(frag_shader, Type::FRAGMENT);

	handle_ = glCreateProgram();
	glAttachShader(handle_, vert_shader);
	glAttachShader(handle_, frag_shader);
	glLinkProgram(handle_);
	CheckCompileErrors(handle_, Type::PROGRAM);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);
}

// shader uniform methods
void Shader::SetBool(const std::string &name, GLboolean value) {
	Bind();
	glUniform1i(GetUniformLocation(name), static_cast<GLint>(value));
}

void Shader::SetInt(const std::string &name, GLint value) {
	Bind();
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string &name, GLfloat value) {
	Bind();
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetVec2(const std::string &name, const glm::vec2 &value) {
	Bind();
	glUniform2fv(
		GetUniformLocation(name),
		1,
		reinterpret_cast<const GLfloat*>(glm::value_ptr(value))
	);
}

void Shader::SetVec2(const std::string &name, GLfloat x, GLfloat y) {
	Bind();
	glUniform2f(GetUniformLocation(name), x, y);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) {
	Bind();
	glUniform3fv(
		GetUniformLocation(name),
		1,
		glm::value_ptr(value)
	);
}

void Shader::SetVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) {
	Bind();
	glUniform3f(GetUniformLocation(name), x, y, z);
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) {
	Bind();
	glUniform4fv(
		GetUniformLocation(name),
		1,
		reinterpret_cast<const GLfloat*>(glm::value_ptr(value))
	);
}

void Shader::SetVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	Bind();
	glUniform4f(GetUniformLocation(name), x, y, z, w);
}

void Shader::SetMat2(const std::string &name, const glm::mat2 &mat) {
	Bind();
	glUniformMatrix2fv(
		GetUniformLocation(name),
		1,
		GL_FALSE,
		reinterpret_cast<const GLfloat*>(glm::value_ptr(mat))
	);
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &mat) {
	Bind();
	glUniformMatrix3fv(
		GetUniformLocation(name),
		1,
		GL_FALSE,
		reinterpret_cast<const GLfloat*>(glm::value_ptr(mat))
	);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) {
	Bind();
	glUniformMatrix4fv(
		GetUniformLocation(name),
		1,
		GL_FALSE,
		reinterpret_cast<const GLfloat*>(glm::value_ptr(mat))
	);
}

bool Shader::CheckCompileErrors(GLuint shader, Type ct) const {
	GLint result{};
	GLint length{};
	std::string log{};

	switch (ct) {
	case Type::PROGRAM:
		glValidateProgram(shader);
		glGetProgramiv(
			shader,
			GL_LINK_STATUS,
			&result
		);
		if (result != GL_TRUE) {
			std::cout << "SHADER PROGRAM LINKING ERROR!" << std::endl;
		}
		glGetProgramiv(
			shader,
			GL_VALIDATE_STATUS,
			&result
		);
		if (result != GL_TRUE) {
			std::cout << "SHADER PROGRAM VALIDATION ERROR: ";
			glGetProgramiv(
				shader,
				GL_INFO_LOG_LENGTH,
				&length
			);
			if (length > 0) {
				log.resize(length);
				glGetProgramInfoLog(
					shader,
					length,
					nullptr,
					const_cast<GLchar*>(log.data())
				);
				std::cout << log << std::endl;
				return false;
			}
		}
		break;

	case Type::VERTEX:
	case Type::FRAGMENT:
		glGetShaderiv(
			shader,
			GL_COMPILE_STATUS,
			&result
		);
		if (result != GL_TRUE) {
			std::cout << "SHADER COMPILATION ERROR: ";
			glGetShaderiv(
				shader,
				GL_INFO_LOG_LENGTH,
				&length
			);
			if (length > 0) {
				log.resize(length);
				glGetShaderInfoLog(
					shader,
					length,
					nullptr,
					const_cast<GLchar*>(log.data())
				);
				std::cout << log << std::endl;
				return false;
			}
		}
		break;
	}

	return true;;
}
