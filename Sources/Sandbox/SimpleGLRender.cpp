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

#include "Core/Base.h"
#include "SimpleGLRender.h"

#undef GL_OFFSET
#define GL_OFFSET(x) ((const GLvoid *)(x))

GLTexture::GLTexture(unsigned char* data, int width, int height, int comp)
{
	glGenTextures(1, &_glHandle);
	glBindTexture(GL_TEXTURE_2D, _glHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
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
	glDeleteTextures(1, &_glHandle);
}

void GLTexture::Use()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _glHandle);
}

void CheckShaderCompileStatus(GLHandle shaderId)
{
	GLint success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint infoLogLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> infoLog(std::max(infoLogLength, int(1)));
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, infoLog.data());
		Eugenix::Log::Error("Shader compilation failed: %s\n", infoLog);
	}
}

void CheckProgramLinkStatus(GLHandle programId)
{
	GLint success;
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint infoLogLength;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> infoLog(std::max(infoLogLength, int(1)));
		glGetShaderInfoLog(programId, infoLogLength, NULL, infoLog.data());
		Eugenix::Log::Error("Program link failed: %s\n", infoLog);
	}
}

static std::map<std::string, GLHandle> GetUniformLocations(GLuint program)
{
	int numUni = -1;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUni);

	std::map<std::string, GLHandle> outUniforms;

	for (int i = 0; i < numUni; ++i)
	{
		int namelen;
		int num;
		GLenum type;
		char name[128];

		glGetActiveUniform(program, static_cast<GLuint>(i), sizeof(name) - 1, &namelen, &num, &type, name);
		name[namelen] = 0;
		GLuint location = glGetUniformLocation(program, name);
		outUniforms[name] = location;
	}
	return outUniforms;
}

GLShaderProgram::GLShaderProgram(const std::string& vsSource, const std::string& fsSource)
{
	const GLchar* vertexShaderRaw = vsSource.c_str();
	const GLchar* fragmentShaderRaw = fsSource.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderRaw, NULL);
	glCompileShader(vertexShader);
	CheckShaderCompileStatus(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderRaw, NULL);
	glCompileShader(fragmentShader);
	CheckShaderCompileStatus(fragmentShader);

	_glHandle = glCreateProgram();
	glAttachShader(_glHandle, vertexShader);
	glAttachShader(_glHandle, fragmentShader);
	glLinkProgram(_glHandle);
	CheckProgramLinkStatus(_glHandle);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	_uniformLocations = GetUniformLocations(_glHandle);
}

GLShaderProgram::~GLShaderProgram()
{
	// TODO : check glIsProgram
	glDeleteProgram(_glHandle);
}

void GLShaderProgram::Use()
{
	glUseProgram(_glHandle);
}

void GLShaderProgram::Unuse()
{
	glUseProgram(0);
}

void GLShaderProgram::SetUniform(const char* name, const glm::mat4& mat)
{
	//if (currentProgram != _glProgram)
	//	Use();
	glUniformMatrix4fv(_uniformLocations[name], 1, GL_FALSE, glm::value_ptr(mat));
}

size_t GetAttribSize(StandartAttribs type)
{
	if (type == StandartAttribs::PositionAttrib || type == StandartAttribs::ColorAttrib)
		return 3;
	else if (type == StandartAttribs::UvAttrib)
		return 2;

	return 3;
}

GLenum GetAttribType(StandartAttribs type)
{
	if (type == StandartAttribs::PositionAttrib || type == StandartAttribs::ColorAttrib || type == StandartAttribs::UvAttrib)
		return GL_FLOAT;

	return GL_FLOAT;
}

void ToggleAttrib(StandartAttribs attrib, GLboolean normalized, size_t vertexSize, const GLvoid* offset)
{
	glEnableVertexAttribArray((GLuint)attrib);
	glVertexAttribPointer((GLuint)attrib, GetAttribSize(attrib), GetAttribType(attrib), normalized, vertexSize, offset);
}

GLMesh::GLMesh(PrimitiveType type, uint32_t format, const void* vertData, size_t verticesSize, size_t vertexSize, const void* indData, size_t indiciesSize, size_t vertsCount, size_t indCount)
	: _topology(type), _vertsCount(vertsCount), _indexCount(indCount)
{
	glGenVertexArrays(1, &_meshVao);
	glBindVertexArray(_meshVao);

	glGenBuffers(1, &_meshVbo);
	glBindBuffer(GL_ARRAY_BUFFER, _meshVbo);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertData, GL_STATIC_DRAW);

	if (format & VertexAttribs::Position)
	{
		ToggleAttrib(StandartAttribs::PositionAttrib, GL_FALSE, vertexSize, GL_OFFSET(0));
	}
	if (format & VertexAttribs::Uv)
	{
		ToggleAttrib(StandartAttribs::UvAttrib, GL_FALSE, vertexSize, GL_OFFSET(sizeof(glm::vec3)));
	}

	if (_indexCount > 0)
	{
		glGenBuffers(1, &_meshIbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshIbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiciesSize, indData, GL_STATIC_DRAW);
	}
}

GLMesh::~GLMesh()
{
	if (_meshVbo && glIsBuffer(_meshVbo))
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &_meshVbo);
	}

	if (_meshIbo && glIsBuffer(_meshIbo))
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &_meshIbo);
	}

	if (_meshVao && glIsVertexArray(_meshVao))
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &_meshVao);
	}
}

void GLMesh::Draw() const
{
	glBindVertexArray(_meshVao);

	static const GLenum glPrimitive[] =
	{
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP,
	};

	if (_indexCount > 0)
		glDrawElements(glPrimitive[(int)_topology], _indexCount, GL_UNSIGNED_INT, NULL);
	else
		glDrawArrays(glPrimitive[(int)_topology], 0, _vertsCount);
}

namespace ShaderEnv
{
	GLuint currentProgram = -1;

	namespace Defaults
	{
		GLProgramPtr SimpleDiffuse()
		{
			static GLProgramPtr simpleDiffuseProgram = nullptr;
			if (simpleDiffuseProgram == nullptr)
			{
				const char* vsSource = R"(
				#version 330 core
				layout (location = 0) in vec3 position;
				layout (location = 1) in vec2 texCoord;
				uniform mat4 mvp;
				out vec2 TexCoord;
				void main()
				{
					gl_Position = mvp * vec4(position, 1.0f);
					TexCoord = texCoord;
				})";

				const char* fsSource = R"(
				#version 330 core
				in vec2 TexCoord;
				out vec4 color;
				uniform sampler2D ourTexture;
				void main()
				{
					color = texture(ourTexture, TexCoord);
				})";

				simpleDiffuseProgram = std::make_shared<GLShaderProgram>(vsSource, fsSource);
				glBindAttribLocation(simpleDiffuseProgram->Id(), (GLuint)StandartAttribs::PositionAttrib, "position");
				glBindAttribLocation(simpleDiffuseProgram->Id(), (GLuint)StandartAttribs::UvAttrib, "uv");
			}
			return simpleDiffuseProgram;
		}
	} // namespace Defaults
} // namespace ShaderEnv