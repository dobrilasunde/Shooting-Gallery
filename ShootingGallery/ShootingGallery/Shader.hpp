#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.hpp"
#include "Renderer.hpp"

class Shader
{
public:
	Shader();
	~Shader();
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	void SetActive();
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	void SetVectorUniform(const char* name, const Vector3& vector);
	void SetFloatUniform(const char* name, float value);

	void SetPointLightUniform(const char* name, PointLight lights[], int size);
	void SetFloatArrayUniform(const char* name, float values[], int size);
	void SetIntUniform(const char* name, int value);

private:
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	bool IsCompiled(GLuint shader);
	bool IsValidProgram();

private:
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};