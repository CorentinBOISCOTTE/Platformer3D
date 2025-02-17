#include "Shader.h"
#include <tracy/Tracy.hpp>

Shader::Shader()
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();
}

void Shader::SetVertexShader(std::filesystem::path const& filename)
{
	vertPath = filename;
}

void Shader::SetFragmentShader(std::filesystem::path const& filename)
{
	fragPath = filename;
}

bool Shader::Link()
{
	const char* vertString = vertStr.c_str();
	glShaderSource(vertexShader, 1, &vertString, NULL);
	glCompileShader(vertexShader);

	int vertSuccess;
	char vertInfoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertSuccess);
	if (!vertSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, vertInfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED : " << vertInfoLog << '\n';
		return false;
	}

	const char* fragString = fragStr.c_str();
	glShaderSource(fragmentShader, 1, &fragString, NULL);
	glCompileShader(fragmentShader);

	int fragSuccess;
	char fragInfoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);
	if (!fragSuccess)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, fragInfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED : " << fragInfoLog << '\n';
		return false;
	}

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int programSuccess;
	char programInfoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programSuccess);
	if (!programSuccess)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, programInfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED : " << programInfoLog << '\n';
		return false;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

void Shader::UseShader()
{
	glUseProgram(shaderProgram);
}

void Shader::SetUniformVector2D(std::string name, Vector2D value)
{
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform2f(location, value.x, value.y);
}

void Shader::SetUniformVector3D(std::string name, Vector3D value)
{
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetUniformVector4D(std::string name, Vector4D value)
{
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetUniformColor(std::string name, Vector4D value)
{
	UseShader();
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetUniformFloat(std::string name, float value)
{
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform1f(location, value);
}

void Shader::SetUniformMatrix4x4(std::string name, mat4x4 value)
{
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, value.data);
}

void Shader::SetUniformInt(std::string name, int value)
{
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform1i(location, value);
}

void Shader::Load()
{
	ZoneScoped;
	std::ifstream vertFile(vertPath);
	std::string vertFileContent((std::istreambuf_iterator<char>(vertFile)), std::istreambuf_iterator<char>());
	vertStr = vertFileContent;

	std::ifstream fragFile(fragPath);
	std::string fragFileContent((std::istreambuf_iterator<char>(fragFile)), std::istreambuf_iterator<char>());
	fragStr = fragFileContent;
}
