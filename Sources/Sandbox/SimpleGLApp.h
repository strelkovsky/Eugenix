#pragma once

#include "AppLayer/App.h"
#include "Core/Base.h"
#include "SimpleGLRender.h"

class SimpleGLApp : public Eugenix::App
{
protected:
	void Clear(uint32_t flags);

	GLProgramPtr CreateProgramFromFile(const std::string& vsSourceFile, const std::string& fsSourceFile);
	GLProgramPtr CreateProgramFromSource(const std::string& vsSource, const std::string& fsSource);

	GLTexturePtr CreateTextureFromFile(const std::string& textureFile);

	GLMeshPtr CreateCubeMesh(float size);
	GLMeshPtr CreateIndexedCubeMesh(float size);

	GLHandle CreateVAO();
	GLHandle CreateVertexBuffer(size_t size, const void* data, GLHandle usage);
	GLHandle CreateIndexBuffer();
};