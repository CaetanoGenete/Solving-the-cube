#include "Shader.h"

#include <fstream>
#include "Renderer.h"

const char* getShaderString(unsigned int shaderType) {
	switch (shaderType) {
	case GL_FRAGMENT_SHADER:
		return "FRAGMENT";
	case GL_VERTEX_SHADER:
		return "VERTEX";
	default:
		return "UNAIVALABLE";
	}
}

unsigned int createShader(const char* shaderPath, unsigned int shaderType) {
	unsigned int shader = glCreateShader(shaderType);

	std::ifstream stream(shaderPath, std::ios::binary);
	utils::String source(stream);

	const char* shaderSource = source.getData();

	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	int success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		int length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		char* log = new char[length];

		glGetShaderInfoLog(shader, length, &length, log);

		std::cout << "ERROR:FAILED TO COMPILE " << getShaderString(shaderType) << " SHADER: " << log;

		glDeleteShader(shader);
		delete[] log;

		return 0;
	}

	return shader;
}

int Shader::getUniformLocation(const utils::String& name) {
	int location = glGetUniformLocation(shaderID, name.getData());

	if (location == -1)
		std::cout << "WARNING:COULD NOT FIND UNIFORM: " <<  name << std::endl;

	return location;
}

Shader::Shader(): shaderID(0)
{
}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	shaderID = glCreateProgram();

	unsigned int vertexShader = createShader(vertexShaderPath, GL_VERTEX_SHADER);
	unsigned int fragmentShader = createShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);

	glLinkProgram(shaderID);
	glUseProgram(shaderID);

	glDetachShader(shaderID, vertexShader);
	glDetachShader(shaderID, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader() {
	glDeleteProgram(shaderID);
}

void Shader::setUniformMat4(const utils::String & name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::bind() const {
	glUseProgram(shaderID);
}

void Shader::unBind() const {
	glUseProgram(0);
}
