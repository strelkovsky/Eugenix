//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#include "CompileConfig.h"
#include "Core/Base.h"

#ifdef EUGENIX_OPENGL

#include "EugenixGL.h"

#define OPENGL_INT_PRINT_DEBUG(name) \
	GLint info_ ## name; \
	glGetIntegerv(name, &info_ ## name); \
	fprintf(stderr, #name " = %d\n", info_ ## name);

namespace Eugenix
{
	namespace EugenixGL
	{
		bool Init()
		{
#ifdef EUGENIX_OPENGL_GLEW

			glewExperimental = GL_TRUE; // Needed in core profile
			GLenum result = glewInit();
			if (result != GLEW_OK)
			{
				fprintf(stderr, "glewInit error: %s\n", glewGetErrorString(result));
				return false;
			}

#elif EUGENIX_OPENGL_NATIVE

#endif //EUGENIX_OPENGL_GLEW

#ifdef EUGENIX_DEBUG

			fprintf(stderr, "[ OpenGL dumps ]\n");

#ifdef EUGENIX_OPENGL_GLEW
			fprintf(stderr, "Use GLEW impl. Version - %s\n", glewGetString(GLEW_VERSION));
#elif EUGENIX_OPENGL_NATIVE
			fprintf(stderr, "Use navive gl impl\n");
#endif //EUGENIX_OPENGL_GLEW

			GLint major, minor;
			glGetIntegerv(GL_MAJOR_VERSION, &major);
			glGetIntegerv(GL_MINOR_VERSION, &minor);
			fprintf(stderr, "OpenGL render context information:\n"
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

			fprintf(stderr, "----------------------------------------------------\n\n");

			fprintf(stderr, "Extensions:\n");

			GLint n, i;
			glGetIntegerv(GL_NUM_EXTENSIONS, &n);
			for (i = 0; i < n; i++)
			{
				fprintf(stderr, "\t%s\n", glGetStringi(GL_EXTENSIONS, i));
			}

			OPENGL_INT_PRINT_DEBUG(GL_MAX_VERTEX_ATTRIBS);
			OPENGL_INT_PRINT_DEBUG(GL_MAX_TEXTURE_IMAGE_UNITS);

#endif // EUGENIX_DEBUG

			return true;
		}
	} // namespace EugenixGL
} // namespace Eugenix

#endif