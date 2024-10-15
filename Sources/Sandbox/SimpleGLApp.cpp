#define GLEW_STATIC
#include <gl/glew.h>

#include <memory>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "AppLayer/App.h"

#include "SimpleGLApp.h"

GLTexturePtr SimpleGLApp::CreateTextureFromFile(const std::string& textureFile)
{
    int width, height, comp;
    unsigned char* data = stbi_load(textureFile.c_str(), &width, &height, &comp, STBI_rgb);

    auto texture = std::make_shared<GLTexture>(data, width, height, comp);

    stbi_image_free(data);

    return texture;  
}

GLProgramPtr SimpleGLApp::CreateProgramFromFile(const std::string& vsSourceFile, const std::string& fsSourceFile)
{
    return CreateProgramFromSource(loadStringFromFile(vsSourceFile), loadStringFromFile(fsSourceFile));
}

GLProgramPtr SimpleGLApp::CreateProgramFromSource(const std::string& vsSource, const std::string& fsSource)
{
    return std::make_shared<GLShaderProgram>(vsSource, fsSource);
}

GLRenderObjectPtr SimpleGLApp::CreateCube()
{
	static const GLfloat points[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	auto vao = CreateVAO();
	auto vbo = CreateVertexBuffer(sizeof(points), &points[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,                   // number of attribute
		3,                   // size of "pos" (vec3) 
		GL_FLOAT,            // type of "pos"
		GL_FALSE,            // do not normalize
		5 * sizeof(GLfloat),   // step between vertex (pos - 3, uv - 2)
		(GLvoid*)0);         // offset


	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,                   // number of attribute
		2,                   // size of "uv" (vec2) 
		GL_FLOAT,            // type of "uv"
		GL_FALSE,            // do not normalize
		5 * sizeof(GLfloat), // step between vertex (pos - 3, uv - 2)
		(GLvoid*)(3 * sizeof(GLfloat))); // offset (after pos)

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return std::make_shared<GLRenderObject>(vao, 36);
}

GLHandle SimpleGLApp::CreateVAO()
{
    GLHandle handle;
    glGenVertexArrays(1, &handle);
    glBindVertexArray(handle);

    return handle;
}

GLHandle SimpleGLApp::CreateVertexBuffer(size_t size, const void* data, GLenum usage)
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
