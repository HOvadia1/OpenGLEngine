#include "GLSLProgram.h"
#include "Errors.h"
#include <vector>
#include <fstream>

GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
{	
}

GLSLProgram::~GLSLProgram()
{
}

void GLSLProgram::CreateShaders(const std::string& vsFilePath, const std::string& fsFilePath)
{
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	if(_vertexShaderID == 0)
	{
		fatalError("Failed to create Vertex Shader!");
	}

	if(_fragmentShaderID == 0)
	{
		fatalError("Failed to create fragment shader!");
	}

	CompileShader(vsFilePath, _vertexShaderID);
	CompileShader(fsFilePath, _fragmentShaderID);
}

void GLSLProgram::CompileShader(const std::string& filePath, GLuint id)
{
	std::ifstream shaderFile(filePath);
	if(shaderFile.fail())
	{
		perror(filePath.c_str());
		fatalError("Failed to open: " + filePath);
	}

	std::string fileContents = "";
	std::string line;

	while(std::getline(shaderFile, line))
	{
		fileContents += line + "\n";
	}

	shaderFile.close();

	const char* content = fileContents.c_str();
	glShaderSource(id, 1, &content, nullptr);
	glCompileShader(id);

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if(success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(id);

		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shader " + filePath + " failed to compile!");
	}
}

void GLSLProgram::LinkShaders()
{
	_programID = glCreateProgram();

	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	glLinkProgram(_programID);

	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, &isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		glDeleteProgram(_programID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		std::printf("%s\n", &(errorLog[0]));
		fatalError("Failed to link shaders to program.");
	}

	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

void GLSLProgram::AddAttribute(const std::string& attributeName)
{
	glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
}

void GLSLProgram::Use()
{
	glUseProgram(_programID);

	for (size_t i = 0; i < _numAttributes; i++)
	{
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::Unuse()
{
	glUseProgram(0);

	for (size_t i = 0; i < _numAttributes; i++)
	{
		glDisableVertexAttribArray(i);
	}
}


