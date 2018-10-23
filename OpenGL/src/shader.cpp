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
	std::string vertex_string = LoadFile(vert_file);
	std::string fragment_string = LoadFile(frag_file);

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

std::string Shader::LoadFile(const std::string& file) {
	std::ifstream ifs;
	std::stringstream ss;

	try {
		ifs.open(file, std::ifstream::in);

		if (!ifs.fail()) {
			ss << ifs.rdbuf();
		}

		ifs.close();
	}
	catch (std::exception ex) {
		std::cout << "EXCEPTION: LOADING SHADER SOURCE" << std::endl;
		std::cout << ex.what() << std::endl;
	}
	return ss.str();
}

// shader uniform methods
void Shader::SetBool(const std::string &name, GLboolean value) const {
	Bind();
	glUniform1i(glGetUniformLocation(handle_, name.c_str()), static_cast<GLint>(value));
}

void Shader::SetInt(const std::string &name, GLint value) const {
	Bind();
	glUniform1i(glGetUniformLocation(handle_, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, GLfloat value) const {
	Bind();
	glUniform1f(glGetUniformLocation(handle_, name.c_str()), value);
}

void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const {
	Bind();
	glUniform2fv(
		glGetUniformLocation(handle_, name.c_str()),
		1,
		reinterpret_cast<const GLfloat*>(glm::value_ptr(value))
	);
}

void Shader::SetVec2(const std::string &name, GLfloat x, GLfloat y) const {
	Bind();
	glUniform2f(glGetUniformLocation(handle_, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const {
	Bind();
	glUniform3fv(
		glGetUniformLocation(handle_, name.c_str()),
		1,
		glm::value_ptr(value)
	);
}

void Shader::SetVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) const {
	Bind();
	glUniform3f(glGetUniformLocation(handle_, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const {
	Bind();
	glUniform4fv(
		glGetUniformLocation(handle_, name.c_str()),
		1,
		reinterpret_cast<const GLfloat*>(glm::value_ptr(value))
	);
}

void Shader::SetVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const {
	Bind();
	glUniform4f(glGetUniformLocation(handle_, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string &name, const glm::mat2 &mat) const {
	Bind();
	glUniformMatrix2fv(
		glGetUniformLocation(handle_, name.c_str()),
		1,
		GL_FALSE,
		reinterpret_cast<const GLfloat*>(glm::value_ptr(mat))
	);
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &mat) const {
	Bind();
	glUniformMatrix3fv(
		glGetUniformLocation(handle_, name.c_str()),
		1,
		GL_FALSE,
		reinterpret_cast<const GLfloat*>(glm::value_ptr(mat))
	);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const {
	Bind();
	glUniformMatrix4fv(
		glGetUniformLocation(handle_, name.c_str()),
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
