#pragma once

#include <glm/glm.hpp>
#include <map>

using GLHandle = GLuint; 

struct GLTexture final
{
	GLTexture(unsigned char* data, int width, int height, int comp);
	~GLTexture();

	void Use();

private:
	GLuint _glTexture;
};
using GLTexturePtr = std::shared_ptr<GLTexture>;

struct GLShaderProgram final
{
	GLShaderProgram(const std::string& vsSource, const std::string& fsSource);
	~GLShaderProgram();

	void Use();
	void Unuse();

	void SetUniform(const char* name, const glm::mat4& mat);

private:
	GLuint _glProgram;
	std::map<std::string, GLint> _uniformLocations;
};
using GLProgramPtr = std::shared_ptr<GLShaderProgram>;

struct GLRenderObject final
{
public:
	GLRenderObject(GLHandle _vao, int _count);
	~GLRenderObject();

	void Draw() const;

private:
	GLuint _vao;

	int _count;
};
using GLRenderObjectPtr = std::shared_ptr<GLRenderObject>;

namespace ShaderEnv
{
	namespace Defaults
	{
		GLProgramPtr SimpleDiffuse();
	} // namespace Defaults
} // namespace ShaderEnv