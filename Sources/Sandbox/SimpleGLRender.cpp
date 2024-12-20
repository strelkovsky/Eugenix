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
#include "Platform/OpenGL/EugenixGL.h"
#include "SimpleGLRender.h"

#undef GL_OFFSET
#define GL_OFFSET(x) ((const GLvoid *)(x))

GLTexture::GLTexture(unsigned char* data, int width, int height, int comp)
{
	OPENGL_CALL(glGenTextures(1, &_glHandle));
	OPENGL_CALL(glBindTexture(GL_TEXTURE_2D, _glHandle));

	OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	OPENGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, comp == 3 ? GL_RGB : GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));

	OPENGL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
	OPENGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

GLTexture::~GLTexture()
{
	// TODO : check glIsBuffer
	//OPENGL_CALL(glDeleteTextures(1, &_glHandle));
}

void GLTexture::Use()
{
	OPENGL_CALL(glActiveTexture(GL_TEXTURE0));
	OPENGL_CALL(glBindTexture(GL_TEXTURE_2D, _glHandle));
}

void CheckShaderCompileStatus(GLHandle shaderId)
{
	GLint success;
	OPENGL_CALL(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLint infoLogLength;
		OPENGL_CALL(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength));
		std::vector<char> infoLog(std::max(infoLogLength, int(1)));
		OPENGL_CALL(glGetShaderInfoLog(shaderId, infoLogLength, NULL, infoLog.data()));
		Eugenix::Log::Error("Shader compilation failed: %s\n", infoLog);
	}
}

void CheckProgramLinkStatus(GLHandle programId)
{
	GLint success;
	OPENGL_CALL(glGetProgramiv(programId, GL_LINK_STATUS, &success));
	if (!success)
	{
		GLint infoLogLength;
		OPENGL_CALL(glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength));
		std::vector<char> infoLog(std::max(infoLogLength, int(1)));
		OPENGL_CALL(glGetShaderInfoLog(programId, infoLogLength, NULL, infoLog.data()));
		Eugenix::Log::Error("Program link failed: %s\n", infoLog);
	}
}

static std::map<std::string, GLHandle> GetUniformLocations(GLuint program)
{
	int numUni = -1;
	OPENGL_CALL(glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUni));

	std::map<std::string, GLHandle> outUniforms;

	for (int i = 0; i < numUni; ++i)
	{
		int namelen;
		int num;
		GLenum type;
		char name[128];

		OPENGL_CALL(glGetActiveUniform(program, static_cast<GLHandle>(i), sizeof(name) - 1, &namelen, &num, &type, name));
		name[namelen] = 0;
		GLuint location = OPENGL_CALL(glGetUniformLocation(program, name));

		Eugenix::Log::Info("Uniform %s index - %d \n", name, location);

		outUniforms[name] = location;
	}
	return outUniforms;
}

static std::map<std::string, GLHandle> GetAttribLocations(GLuint program)
{
	int numAttrs = -1;
	OPENGL_CALL(glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttrs));

	std::map<std::string, GLHandle> outAttributes;

	for (int i = 0; i < numAttrs; ++i)
	{
		int namelen;
		int num;
		GLenum type;
		char name[128];

		OPENGL_CALL(glGetActiveAttrib(program, static_cast<GLHandle>(i), sizeof(name) - 1, &namelen, &num, &type, name));

		name[namelen] = 0;
		GLuint location = glGetAttribLocation(program, name);

		Eugenix::Log::Info("Attribute %s index - %d \n", name, location);

		outAttributes[name] = location;
	}
	return outAttributes;
}

GLShaderProgram::GLShaderProgram(const std::string& vsSource, const std::string& fsSource)
{
	const GLchar* vertexShaderRaw = vsSource.c_str();
	const GLchar* fragmentShaderRaw = fsSource.c_str();

	GLuint vertexShader = OPENGL_CALL(glCreateShader(GL_VERTEX_SHADER));
	OPENGL_CALL(glShaderSource(vertexShader, 1, &vertexShaderRaw, NULL));
	OPENGL_CALL(glCompileShader(vertexShader));
	CheckShaderCompileStatus(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	OPENGL_CALL(glShaderSource(fragmentShader, 1, &fragmentShaderRaw, NULL));
	OPENGL_CALL(glCompileShader(fragmentShader);)
	CheckShaderCompileStatus(fragmentShader);

	_glHandle = glCreateProgram();
	OPENGL_CALL(glAttachShader(_glHandle, vertexShader));
	OPENGL_CALL(glAttachShader(_glHandle, fragmentShader));
	OPENGL_CALL(glLinkProgram(_glHandle));
	CheckProgramLinkStatus(_glHandle);

	OPENGL_CALL(glDeleteShader(vertexShader));
	OPENGL_CALL(glDeleteShader(fragmentShader));

	_attribLocations = GetAttribLocations(_glHandle);
	_uniformLocations = GetUniformLocations(_glHandle);
}

GLShaderProgram::~GLShaderProgram()
{
	// TODO : check glIsProgram
	//OPENGL_CALL(glDeleteProgram(_glHandle));
}

void GLShaderProgram::Use()
{
	OPENGL_CALL(glUseProgram(_glHandle));
}

void GLShaderProgram::Unuse()
{
	OPENGL_CALL(glUseProgram(0));
}

void GLShaderProgram::SetUniform(const char* name, const glm::mat4& mat)
{
	//if (currentProgram != _glProgram)
	//	Use();
	OPENGL_CALL(glUniformMatrix4fv(_uniformLocations[name], 1, GL_FALSE, glm::value_ptr(mat)));
}

size_t GetAttribSize(StandartAttribs type)
{
	if (type == StandartAttribs::Position || type == StandartAttribs::Color)
		return 3;
	else if (type == StandartAttribs::Uv)
		return 2;

	return 3;
}

GLenum GetAttribType(StandartAttribs type)
{
	if (type == StandartAttribs::Position || type == StandartAttribs::Color || type == StandartAttribs::Uv)
		return GL_FLOAT;

	return GL_FLOAT;
}

GLMesh::GLMesh(PrimitiveType type, uint32_t format, const void* vertData, size_t verticesSize, size_t vertexSize, const void* indData, size_t indiciesSize, size_t vertsCount, size_t indCount)
	: _topology(type), _vertsCount(vertsCount), _indexCount(indCount)
{
	DoBuffersWork(vertData, verticesSize, indData, indiciesSize);
	DoAttribsWork(format, vertexSize);
}

void GLMesh::DoBuffersWork(const void* vertData, size_t verticesSize, const void* indData, size_t indiciesSize)
{
	OPENGL_CALL(glGenVertexArrays(1, &_meshVao));
	OPENGL_CALL(glBindVertexArray(_meshVao));

	OPENGL_CALL(glGenBuffers(1, &_meshVbo));
	OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _meshVbo));
	OPENGL_CALL(glBufferData(GL_ARRAY_BUFFER, verticesSize, vertData, GL_STATIC_DRAW));

	if (_indexCount > 0)
	{
		OPENGL_CALL(glGenBuffers(1, &_meshIbo));
		OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshIbo));
		OPENGL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiciesSize, indData, GL_STATIC_DRAW));
	}
}

void ToggleAttrib(StandartAttribs attrib, GLboolean normalized, size_t vertexSize, const GLvoid* offset)
{
	OPENGL_CALL(glEnableVertexAttribArray((GLuint)attrib));
	OPENGL_CALL(glVertexAttribPointer((GLuint)attrib, GetAttribSize(attrib), GetAttribType(attrib), normalized, vertexSize, offset));
}

void GLMesh::DoAttribsWork(uint32_t format, size_t vertexSize)
{
	if (format & VertexAttribs::Position)
	{
		ToggleAttrib(StandartAttribs::Position, GL_FALSE, vertexSize, GL_OFFSET(0));
	}
	if (format & VertexAttribs::Uv)
	{
		ToggleAttrib(StandartAttribs::Uv, GL_FALSE, vertexSize, GL_OFFSET(sizeof(glm::vec3)));
	}
}

GLMesh::~GLMesh()
{
	if (_meshVbo && glIsBuffer(_meshVbo))
	{
		OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		OPENGL_CALL(glDeleteBuffers(1, &_meshVbo));
	}

	if (_meshIbo && glIsBuffer(_meshIbo))
	{
		OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		OPENGL_CALL(glDeleteBuffers(1, &_meshIbo));
	}

	if (_meshVao && glIsVertexArray(_meshVao))
	{
		OPENGL_CALL(glBindVertexArray(0));
		OPENGL_CALL(glDeleteVertexArrays(1, &_meshVao));
	}
}

void GLMesh::Draw() const
{
	OPENGL_CALL(glBindVertexArray(_meshVao));

	static const GLenum glPrimitive[] =
	{
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP,
	};

	if (_indexCount > 0)
	{
		OPENGL_CALL(glDrawElements(glPrimitive[(int)_topology], _indexCount, GL_UNSIGNED_INT, NULL));
	}
	else
	{
		OPENGL_CALL(glDrawArrays(glPrimitive[(int)_topology], 0, _vertsCount));
	}
}

uint32_t Render::RenderQueue::Render()
{
	unsigned int s = _queue.size();

	for (unsigned int i = 0; i < _queue.size();)
	{
		glBindVertexArray(_queue[i]->vao);
		glDrawArrays(GL_TRIANGLES, 0, _queue[i]->vertsCount);
		_queue.erase(_queue.begin() + i);
	}

	return s;
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
				out vec2 fragTexcoord;
				void main()
				{
					gl_Position = mvp * vec4(position, 1.0f);
					fragTexcoord = texCoord;
				})";

				const char* fsSource = R"(
				#version 330 core
				uniform sampler2D ourTexture;
				in vec2 fragTexcoord;
				out vec4 color;
				void main()
				{
					color = texture(ourTexture, fragTexcoord);
				})";

				simpleDiffuseProgram = std::make_shared<GLShaderProgram>(vsSource, fsSource);
				OPENGL_CALL(glBindAttribLocation(simpleDiffuseProgram->Id(), (GLuint)StandartAttribs::Position, "position"));
				OPENGL_CALL(glBindAttribLocation(simpleDiffuseProgram->Id(), (GLuint)StandartAttribs::Uv, "uv"));
			}
			return simpleDiffuseProgram;
		}
	} // namespace Defaults
} // namespace ShaderEnv