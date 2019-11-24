#include "Application.h"
#include "Shader.h"


Shader::Shader()
{
	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		LOGC("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s", infoLog);
	}
	
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		LOGC("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s", infoLog);
	}
	// fragment shader
	const char* f2 = fragmentShaderSource2.c_str();
	int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &f2, NULL);
	glCompileShader(fragmentShader2);
	// check for shader compile errors
	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
		LOGC("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s", infoLog);
	}

	// link shaders
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	// check for linking errors
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		LOGC("ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s", infoLog);
	}

	// link shaders
	ID_N = glCreateProgram();
	glAttachShader(ID_N, vertexShader);
	glAttachShader(ID_N, fragmentShader2);
	glLinkProgram(ID_N);
	// check for linking errors
	glGetProgramiv(ID_N, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID_N, 512, NULL, infoLog);
		LOGC("ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s", infoLog);
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use(int i)
{
	switch (i) {
	case 0:
		glUseProgram(ID);
		break;
	case 1:
		glUseProgram(ID_N);
		break;
	}
	glEnable(GL_BLEND);
}


void Shader::stop()
{
	glUseProgram(0);
	glDisable(GL_BLEND);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const mat4x4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat);
}