#define GLEW_STATIC
#include <gl/glew.h>

#include <memory>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "AppLayer/App.h"
#include "Core/Base.h"

#include "SimpleGLApp.h"

GLTexturePtr SimpleGLApp::CreateTextureFromFile(const std::string& textureFile)
{
    int width, height, comp;
    unsigned char* data = stbi_load(textureFile.c_str(), &width, &height, &comp, STBI_rgb);

    auto texture = std::make_shared<GLTexture>(data, width, height, comp);

    stbi_image_free(data);

    return texture;  
}

void SimpleGLApp::Clear(uint32_t flags)
{
	GLbitfield glMask = 0;

	if (flags & ClearFlags::Color)
		glMask |= GL_COLOR_BUFFER_BIT;
	if (flags & ClearFlags::Depth)
		glMask |= GL_DEPTH_BUFFER_BIT;
	if (flags & ClearFlags::Stencil)
		glMask |= GL_STENCIL_BUFFER_BIT;

	glClear(glMask);
}

GLProgramPtr SimpleGLApp::CreateProgramFromFile(const std::string& vsSourceFile, const std::string& fsSourceFile)
{
    return CreateProgramFromSource(loadStringFromFile(vsSourceFile), loadStringFromFile(fsSourceFile));
}

GLProgramPtr SimpleGLApp::CreateProgramFromSource(const std::string& vsSource, const std::string& fsSource)
{
    return std::make_shared<GLShaderProgram>(vsSource, fsSource);
}

struct CubeVertex
{
	glm::vec3 m_pos;
	glm::vec2 m_tex;

	CubeVertex() {}

	CubeVertex(glm::vec3 pos, glm::vec2 tex)
	{
		m_pos = pos;
		m_tex = tex;
	}
};

GLMeshPtr SimpleGLApp::CreateCubeMesh(float size)
{
	static const CubeVertex points[] =
	{
		CubeVertex(glm::vec3(-size, -size, -size), glm::vec2(0.0f, 0.0f)),
		CubeVertex(glm::vec3(size, -size, -size), glm::vec2(1.0f, 0.0f)),
		CubeVertex(glm::vec3(size, size, -size), glm::vec2(1.0f, 1.0f)),
		CubeVertex(glm::vec3(size, size, -size), glm::vec2(1.0f, 1.0f)),
		CubeVertex(glm::vec3(-size, size, size), glm::vec2(0.0f, 1.0f)),
		CubeVertex(glm::vec3(-size, -size, -size), glm::vec2(0.0f, 0.0f)),

		CubeVertex(glm::vec3(-size, -size, size), glm::vec2(0.0f, 0.0f)),
		CubeVertex(glm::vec3(size, -size, size), glm::vec2(1.0f, 0.0f)),
		CubeVertex(glm::vec3(size, size, size), glm::vec2(1.0f, 1.0f)),
		CubeVertex(glm::vec3(size, size, size), glm::vec2(1.0f, 1.0f)),
		CubeVertex(glm::vec3(-size, size, size), glm::vec2(0.0f, 1.0f)),
		CubeVertex(glm::vec3(-size, -size, size), glm::vec2(0.0f, 0.0f)),

		CubeVertex(glm::vec3(-size, size, size), glm::vec2(1.0f, 0.0f)),
		CubeVertex(glm::vec3(-size, size, -size), glm::vec2(1.0f, 1.0f)),
		CubeVertex(glm::vec3(-size, -size, -size), glm::vec2(0.0f, 1.0f)),
		CubeVertex(glm::vec3(-size, size, size), glm::vec2(0.0f, 1.0f)),
		CubeVertex(glm::vec3(-size, --size, size), glm::vec2(0.0f, 0.0f)),
		CubeVertex(glm::vec3(-size, size, size), glm::vec2(1.0f, 0.0f)),

		CubeVertex(glm::vec3(size, size, size), glm::vec2(1.0f, 0.0f)),
		CubeVertex(glm::vec3(size, size, -size), glm::vec2(1.0f, 1.0f)),
		CubeVertex(glm::vec3(size, -size, -size), glm::vec2(0.0f, 1.0f)),
		CubeVertex(glm::vec3(size, -size, -size), glm::vec2(0.0f, 1.0f)),
		CubeVertex(glm::vec3(size, -size, size), glm::vec2(0.0f, 0.0f)),
		CubeVertex(glm::vec3(size, size, size), glm::vec2(0.0f, 0.0f)),

		CubeVertex(glm::vec3(-size, -size, -size), glm::vec2(0.0f, 1.0f)),
		CubeVertex(glm::vec3(size, -size, -size), glm::vec2(1.0f, 1.0f)),
		CubeVertex(glm::vec3(size, -size, size), glm::vec2(1.0f, 0.0f)),
		CubeVertex(glm::vec3(size, -size, size), glm::vec2(1.0f, 0.0f)),
		CubeVertex(glm::vec3(-size, -size, size), glm::vec2(0.0f, 0.0f)),
		CubeVertex(glm::vec3(-size, -size, -size), glm::vec2(0.0f, 1.0f)),

		CubeVertex(glm::vec3(-size, size, -size), glm::vec2(0.0f, 1.0f)),
		CubeVertex(glm::vec3(size, size, -size), glm::vec2(1.0f, 1.0f)),
		CubeVertex(glm::vec3(size, size, size), glm::vec2(1.0f, 0.0f)),
		CubeVertex(glm::vec3(size, size, size), glm::vec2(1.0f, 0.0f)),
		CubeVertex(glm::vec3(-size, size, size), glm::vec2(0.0f, 0.0f)),
		CubeVertex(glm::vec3(-size, size, -size), glm::vec2(0.0f, 1.0f)),
	};

	uint32_t format = VertexAttribs::Position | VertexAttribs::Uv;
	return std::make_shared<GLMesh>(PrimitiveType::Triangles, format, points, sizeof(points), sizeof(CubeVertex), nullptr, 0, EUGENIX_ARRAY_SIZE(points), 0);
}

GLMeshPtr SimpleGLApp::CreateIndexedCubeMesh(float s)
{
	static const CubeVertex cubeVertices[] =
	{
		// front
		CubeVertex(glm::vec3(-s, s, s), glm::vec2(0.0f, 1.0f)),
		CubeVertex(glm::vec3(s, s, s), glm::vec2(1.0f, 1.0f)),
		CubeVertex(glm::vec3(s,-s, s), glm::vec2(1.0f, 0.0f)),
		CubeVertex(glm::vec3(-s,-s, s), glm::vec2(0.0f, 0.0f)),

		// back
		CubeVertex(glm::vec3(s, s,-s), glm::vec2(0.0f, 1.0f)),
		CubeVertex(glm::vec3(-s, s,-s), glm::vec2(1.0f, 1.0f)),
		CubeVertex(glm::vec3(-s,-s,-s), glm::vec2(1.0f, 0.0f)),
		CubeVertex(glm::vec3(s,-s,-s), glm::vec2(0.0f, 0.0f)),

		// top
		CubeVertex(glm::vec3(-s, s,-s), glm::vec2(0.0f, 1.0f)),
		CubeVertex(glm::vec3(s, s,-s), glm::vec2(1.0f, 1.0f)),
		CubeVertex(glm::vec3(s, s, s), glm::vec2(1.0f, 0.0f)),
		CubeVertex(glm::vec3(-s, s, s), glm::vec2(0.0f, 0.0f)),

		// bottom
		CubeVertex(glm::vec3(s,-s,-s), glm::vec2(0.0f, 1.0f)),
		CubeVertex(glm::vec3(-s,-s,-s), glm::vec2(1.0f, 1.0f)),
		CubeVertex(glm::vec3(-s,-s, s), glm::vec2(1.0f, 0.0f)),
		CubeVertex(glm::vec3(s,-s, s), glm::vec2(0.0f, 0.0f)),

		// left
		CubeVertex(glm::vec3(-s, s,-s), glm::vec2(0.0f, 1.0f)),
		CubeVertex(glm::vec3(-s, s, s), glm::vec2(1.0f, 1.0f)),
		CubeVertex(glm::vec3(-s,-s, s), glm::vec2(1.0f, 0.0f)),
		CubeVertex(glm::vec3(-s,-s,-s), glm::vec2(0.0f, 0.0f)),

		// right
		CubeVertex(glm::vec3(s, s, s), glm::vec2(0.0f, 1.0f)),
		CubeVertex(glm::vec3(s, s,-s), glm::vec2(1.0f, 1.0f)),
		CubeVertex(glm::vec3(s,-s,-s), glm::vec2(1.0f, 0.0f)),
		CubeVertex(glm::vec3(s,-s, s), glm::vec2(0.0f, 0.0f)),
	};

	static const uint32_t cubeIndices[] =
	{
		0, 3, 1,  1, 3, 2, // front
		4, 7, 5,  5, 7, 6, // back
		8,11, 9,  9,11,10, // top
		12,15,13, 13,15,14, // bottom
		16,19,17, 17,19,18, // left
		20,23,21, 21,23,22  // right
	};

	uint32_t format = VertexAttribs::Position | VertexAttribs::Uv;
	return std::make_shared<GLMesh>(PrimitiveType::Triangles, format, cubeVertices, sizeof(cubeVertices), sizeof(CubeVertex), cubeIndices, sizeof(cubeIndices), EUGENIX_ARRAY_SIZE(cubeVertices),
		EUGENIX_ARRAY_SIZE(cubeIndices));
}

GLHandle SimpleGLApp::CreateVAO()
{
    GLHandle handle;
    glGenVertexArrays(1, &handle);
    glBindVertexArray(handle);

    return handle;
}

GLHandle SimpleGLApp::CreateVertexBuffer(size_t size, const void* data, GLHandle usage)
{
	GLHandle handle;
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);

	return handle;
}

GLHandle SimpleGLApp::CreateIndexBuffer()
{
    return GLHandle();
}
