#pragma once

#include <string>
#include <GL/glew.h>

class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();

	void CreateShaders(const std::string& vsFilePath, const std::string& fsFilePath);
	void LinkShaders();
	void AddAttribute(const std::string& attributeName);

	void Use();
	void Unuse();

private:
	void CompileShader(const std::string& filePath, GLuint id);

private:
	int _numAttributes;
	GLuint _programID;
	GLuint _vertexShaderID;
	GLuint _fragmentShaderID;
};