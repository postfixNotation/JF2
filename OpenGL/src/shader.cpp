#include <shader.hpp>

Shader::Shader(std::string vert_file, std::string frag_file) {
	std::string vertex_string = LoadFile(vert_file);
	std::string fragment_string = LoadFile(frag_file);

	const GLchar* vertex_src = vertex_string.c_str();
	const GLchar* fragment_src = fragment_string.c_str();

	vert_shader_ = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader_, 1, &vertex_src, nullptr);
	glCompileShader(vert_shader_);

	frag_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader_, 1, &fragment_src, nullptr);
	glCompileShader(frag_shader_);

	handle_ = glCreateProgram();
	glAttachShader(handle_, vert_shader_);
	glAttachShader(handle_, frag_shader_);
	glLinkProgram(handle_);

	glDeleteShader(vert_shader_);
	glDeleteShader(frag_shader_);
}

Shader::~Shader() { glDeleteProgram(handle_); }

void Shader::Use() const { glUseProgram(handle_); }

bool Shader::Init() {
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cerr << "ERROR: COULD NOT INITIALIZE GLEW" << std::endl;
		return false;
	}
	return true;
}

std::string Shader::LoadFile(const std::string file) {
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
	this->Use();
	glUniform1i(
		glGetUniformLocation(handle_, name.c_str()),
		static_cast<GLint>(value)
	);
}

void Shader::SetInt(const std::string &name, GLint value) const {
	this->Use();
	glUniform1i(
		glGetUniformLocation(handle_, name.c_str()),
		value
	);
}

void Shader::SetFloat(const std::string &name, GLfloat value) const {
	this->Use();
	glUniform1f(
		glGetUniformLocation(handle_, name.c_str()),
		value
	);
}

void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const {
	this->Use();
	glUniform2fv(
		glGetUniformLocation(handle_, name.c_str()),
		1,
		reinterpret_cast<const GLfloat*>(glm::value_ptr(value))
	);
}

void Shader::SetVec2(const std::string &name, GLfloat x, GLfloat y) const {
	this->Use();
	glUniform2f(
		glGetUniformLocation(handle_, name.c_str()),
		x,
		y
	);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const {
	this->Use();
	glUniform3fv(
		glGetUniformLocation(handle_, name.c_str()),
		1,
		glm::value_ptr(value)
	);
}

void Shader::SetVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) const {
	this->Use();
	glUniform3f(
		glGetUniformLocation(handle_, name.c_str()),
		x,
		y,
		z
	);
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const {
	this->Use();
	glUniform4fv(
		glGetUniformLocation(handle_, name.c_str()),
		1,
		reinterpret_cast<const GLfloat*>(glm::value_ptr(value))
	);
}

void Shader::SetVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const {
	this->Use();
	glUniform4f(
		glGetUniformLocation(handle_, name.c_str()),
		x,
		y,
		z,
		w
	);
}

void Shader::SetMat2(const std::string &name, const glm::mat2 &mat) const {
	this->Use();
	glUniformMatrix2fv(
		glGetUniformLocation(handle_, name.c_str()),
		1,
		GL_FALSE,
		reinterpret_cast<const GLfloat*>(glm::value_ptr(mat))
	);
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &mat) const {
	this->Use();
	glUniformMatrix3fv(
		glGetUniformLocation(handle_, name.c_str()),
		1,
		GL_FALSE,
		reinterpret_cast<const GLfloat*>(glm::value_ptr(mat))
	);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const {
	this->Use();
	glUniformMatrix4fv(
		glGetUniformLocation(handle_, name.c_str()),
		1,
		GL_FALSE,
		reinterpret_cast<const GLfloat*>(glm::value_ptr(mat))
	);
}

