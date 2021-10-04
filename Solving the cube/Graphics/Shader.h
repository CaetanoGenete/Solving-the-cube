#pragma once

#include "CUtils.h"
#include "glm\mat4x4.hpp"

class Shader {
private:
	unsigned int shaderID; // the id of the shader

	int getUniformLocation(const utils::String& name); // retrieves the line that the unfirform is on

public:

	Shader();

	//Takes in the source to a vertex shader and fragment shader file and compiles them
	Shader(const char* vertexShader, const char* fragmentShader);
	
	~Shader();

	void setUniformMat4(const utils::String& name, const glm::mat4& matrix);

	void bind() const; //Set this shader as the current one being used
	void unBind() const; 
};