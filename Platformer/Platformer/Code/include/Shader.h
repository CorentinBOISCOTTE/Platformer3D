#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "ResourceManager.h"
#include "Math/matrice.h"

class Shader : public IResource
{
public:
	Shader();
	void SetVertexShader(std::filesystem::path const& filename);
	void SetFragmentShader(std::filesystem::path const& filename);
	bool Link();

	void UseShader();
	void SetUniformVector2D(std::string name, Vector2D value);
	void SetUniformVector3D(std::string name, Vector3D value);
	void SetUniformVector4D(std::string name, Vector4D value); 
	void SetUniformMatrix4x4(std::string name, mat4x4 value);
	void SetUniformColor(std::string name, Vector4D value);
	void SetUniformFloat(std::string name, float value);
	void SetUniformInt(std::string name, int value);
	void Load() override;
	void Unload() override {}
	bool IsLoaded() override { return true; }

	GLuint GetProgram() const { return shaderProgram; }

private:
	std::filesystem::path vertPath = "";
	std::filesystem::path fragPath = "";
	std::string vertStr{};
	std::string fragStr{};

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
};