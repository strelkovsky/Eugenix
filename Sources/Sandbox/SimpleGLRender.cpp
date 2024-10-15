#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SimpleGLRender.h"

GLTexture::GLTexture(unsigned char* data, int width, int height, int comp)
{
	glGenTextures(1, &_glTexture);
	glBindTexture(GL_TEXTURE_2D, _glTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (comp == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else if (comp == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLTexture::~GLTexture()
{
	// TODO : check glIsBuffer
	glDeleteTextures(1, &_glTexture);
}

void GLTexture::Use()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _glTexture);
}

GLShaderProgram::GLShaderProgram(const std::string& vsSource, const std::string& fsSource)
{
	const GLchar* vertexShaderRaw = vsSource.c_str();
	const GLchar* fragmentShaderRaw = fsSource.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderRaw, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderRaw, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	_glProgram = glCreateProgram();
	glAttachShader(_glProgram, vertexShader);
	glAttachShader(_glProgram, fragmentShader);
	glLinkProgram(_glProgram);

	glGetProgramiv(_glProgram, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(_glProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

GLShaderProgram::~GLShaderProgram()
{
	// TODO : check glIsProgram
	glDeleteProgram(_glProgram);
}

void GLShaderProgram::Use()
{
	glUseProgram(_glProgram);
}

void GLShaderProgram::Unuse()
{
	glUseProgram(0);
}

void GLShaderProgram::SetUniform(const char* name, const glm::mat4& mat)
{
	auto transformLoc = glGetUniformLocation(_glProgram, name);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mat));
}

GLRenderObject::GLRenderObject(GLHandle vao, int count)
{
	_vao = vao;
	_count = count;
}

GLRenderObject::~GLRenderObject()
{

	return;
	// TODO : check glIsBuffer
	if (_vao)
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &_vao);
	}
}

void GLRenderObject::Draw() const
{
	glBindVertexArray(_vao);

	glDrawArrays(GL_TRIANGLES, 0, _count);

	glBindVertexArray(0);
}
