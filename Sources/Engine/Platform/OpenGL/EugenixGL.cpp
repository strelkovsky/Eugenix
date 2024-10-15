//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#include "Platform/Platform.h"

#ifdef EUGENIX_OPENGL
#include "Core/Base.h"
#include "EugenixGL.h"

#ifdef EUGENIX_OPENGL_GLEW
#	define GLEW_STATIC
#	include <GL/glew.h>
#endif // EUGENIX_OPENGL_GLEW

#define OPENGL_INT_PRINT_INFO(name) \
	GLint info_ ## name; \
	glGetIntegerv(name, &info_ ## name); \
	Log::Info(#name " = %d\n", info_ ## name);

namespace Eugenix
{
	namespace EugenixGL
	{
		bool Init()
		{
#ifdef EUGENIX_OPENGL_GLEW
			Log::Info("Use GLEW impl. Version - %s\n", glewGetString(GLEW_VERSION));
			//Initialize GLEW
			glewExperimental = GL_TRUE;
			GLenum glewError = glewInit();
			if (glewError != GLEW_OK)
			{
				Log::Fatal("Error initializing GLEW! %s", glewGetErrorString(glewError));
				return false;
			}
#elif EUGENIX_OPENGL_NATIVE
			Log::Info("Use navive OpenGL impl\n");
#	error "EugenixGL: Only GLEW init support at the moment!"
#endif // EUGENIX_OPENGL_GLEW

			Log::Info("[ OpenGL dumps ]\n");

			GLint major, minor;
			glGetIntegerv(GL_MAJOR_VERSION, &major);
			glGetIntegerv(GL_MINOR_VERSION, &minor);
			Log::Info("OpenGL render context information:\n"
				"  Renderer       : %s\n"
				"  Vendor         : %s\n"
				"  Version        : %s\n"
				"  GLSL version   : %s\n"
				"  OpenGL version : %d.%d\n\n",
				(const char*)glGetString(GL_RENDERER),
				(const char*)glGetString(GL_VENDOR),
				(const char*)glGetString(GL_VERSION),
				(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION),
				major, minor
			);
			Log::Info("----------------------------------------------------\n\n");

			Log::Info("Supported Extensions:\n");

			GLint n, i;
			glGetIntegerv(GL_NUM_EXTENSIONS, &n);
			for (i = 0; i < n; i++)
			{
				Log::Info("\t%s\n", glGetStringi(GL_EXTENSIONS, i));
			}

			OPENGL_INT_PRINT_INFO(GL_MAX_VERTEX_ATTRIBS);
			OPENGL_INT_PRINT_INFO(GL_MAX_TEXTURE_IMAGE_UNITS);

			glEnable(GL_DEPTH_TEST);
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClearDepth(1.0f);

			return true;
		}
	} // namespace EugenixGL
} // namespace Eugenix

#endif // EUGENIX_OPENGL
