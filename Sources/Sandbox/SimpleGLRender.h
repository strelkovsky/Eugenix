#pragma once

#include <SDL.h>

#include <glm/glm.hpp>
#include <map>

enum /*struct*/ ClearFlags : uint8_t
{
	Color = 1,
	Depth,
	Stencil,

	All = Color | Depth | Stencil
};

using GLHandle = unsigned int; 

struct GLHandledObject
{
	GLHandle Id() { return _glHandle; }
	bool IsValid() { return _glHandle > 0; }

protected:
	GLHandle _glHandle;
};

struct GLTexture final : GLHandledObject
{
	GLTexture(unsigned char* data, int width, int height, int comp);
	~GLTexture();

	void Use();
};
using GLTexturePtr = std::shared_ptr<GLTexture>;

struct GLShaderProgram final : GLHandledObject
{
	GLShaderProgram(const std::string& vsSource, const std::string& fsSource);
	~GLShaderProgram();

	void Use();
	void Unuse();

	void SetUniform(const char* name, const glm::mat4& mat);

private:
	std::map<std::string, GLHandle> _uniformLocations;
};
using GLProgramPtr = std::shared_ptr<GLShaderProgram>;

enum VertexAttribs : uint32_t
{
	Position = 1,
	Uv,
};

enum struct PrimitiveType : uint16_t
{
	Triangles,
	TriangleStrip
};

enum struct StandartAttribs : uint16_t
{
	PositionAttrib,
	UvAttrib,
	ColorAttrib,
};

struct GLMesh final : GLHandledObject
{
	GLMesh(PrimitiveType type, uint32_t format, const void* vertData, size_t verticesSize, size_t vertexSize, const void* indData, size_t indiciesSize, size_t vertsCount, size_t indCount);
	~GLMesh();

	void Draw() const;

private:
	PrimitiveType _topology;
	GLHandle _meshVao;
	GLHandle _meshVbo;
	GLHandle _meshIbo;

	int _vertsCount{ 0 };
	int _indexCount{ 0 };
};
using GLMeshPtr = std::shared_ptr<GLMesh>;

namespace ShaderEnv
{
	namespace Defaults
	{
		GLProgramPtr SimpleDiffuse();
	} // namespace Defaults
} // namespace ShaderEnv