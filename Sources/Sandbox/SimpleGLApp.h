#pragma once

#include "AppLayer/App.h"
#include "SimpleGLRender.h"

class SimpleGLApp : public App
{
protected:
	GLProgramPtr CreateProgramFromFile(const std::string& vsSourceFile, const std::string& fsSourceFile);
	GLProgramPtr CreateProgramFromSource(const std::string& vsSource, const std::string& fsSource);

	GLTexturePtr CreateTextureFromFile(const std::string& textureFile);

	GLRenderObjectPtr CreateCube();

	GLHandle CreateVAO();
	GLHandle CreateVertexBuffer(size_t size, const void* data, GLenum usage);
	GLHandle CreateIndexBuffer();
};