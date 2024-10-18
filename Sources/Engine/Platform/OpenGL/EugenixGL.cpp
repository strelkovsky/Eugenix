//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#include "Platform/Platform.h"

#ifdef EUGENIX_OPENGL

#include "Core/Base.h"
#include "EugenixGL.h"

#define OPENGL_INT_PRINT_INFO(name) \
	GLint info_ ## name; \
	glGetIntegerv(name, &info_ ## name); \
	Log::Info(#name " = %d\n", info_ ## name);

namespace Eugenix
{
	namespace EugenixGL
	{
		GLenum oglError = GL_NO_ERROR;

		const char* GetErrorString(GLenum err)
		{
			switch (err)
			{
			case GL_INVALID_ENUM:
				return "Invalid enum";
				break;
			case GL_INVALID_VALUE:
				return "Invalid value";
				break;
			case GL_INVALID_OPERATION:
				return "Invalid operation";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				return "Invalid framebuffer operation";
				break;
			case(GL_STACK_OVERFLOW):
				return "Stack overflow";
				break;
			case(GL_STACK_UNDERFLOW):
				return "Stack underflow";
				break;
			case GL_OUT_OF_MEMORY:
				return "Out of memory";
				break;
			case GL_CONTEXT_LOST:
				return "Context lost";
				break;
			default:
				break;
			}

			return "Unknown error";
		}

#ifdef EUGENIX_OPENGL_NATIVE
		namespace GLFunctions
		{
			bool LoadGlFuncs();
		}
#endif

#ifdef EUGENIX_DEBUG
		void GLAPIENTRY DebugMessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
		{
			Log::Error("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
				(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
				type, severity, message);
		}
#endif // EUGENIX_DEBUG 

		bool Init()
		{
#ifdef EUGENIX_OPENGL_GLEW
			Log::Info("Use GLEW impl (with glewExperimental). Version - %s\n", glewGetString(GLEW_VERSION));
			glewExperimental = GL_TRUE;
			GLenum glewError = glewInit();
			if (glewError != GLEW_OK)
			{
				Log::Fatal("Failed to init GLEW! %s", glewGetErrorString(glewError));
				return false;
			}
			else
			{
				Log::Info("GLEW successfully initialited.\n");
			}

			//Check OpenGL version
			if (GL_VERSION_4_6)
			{
				Log::Info("OpenGL 4.6 supported\n");
			}
			else if (GLEW_VERSION_4_5)
			{
				Log::Info("OpenGL 4.3 supported\n");
			}
			else if (GLEW_VERSION_4_4)
			{
				Log::Info("OpenGL 4.3 supported\n");
			}
			else if (GLEW_VERSION_4_3)
			{
				Log::Info("OpenGL 4.3 supported\n");
			}
			else if (GLEW_VERSION_4_2)
			{
				Log::Info("OpenGL 4.2 supported\n");
			}
			else if (GLEW_VERSION_4_1) 
			{
				Log::Info("OpenGL 4.1 supported\n");
			}
			else if (GLEW_VERSION_4_0) 
			{
				Log::Info("OpenGL 4.0 supported\n");
			}
			else if (GLEW_VERSION_3_3) 
			{
				Log::Info("OpenGL 3.3 supported\n");
			}
			else 
			{
				Log::Fatal("OpenGL 3.3 is not supported! Check you videocard's driver!\n");
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

#ifdef EUGENIX_DEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(DebugMessageCallback, 0);
#endif // EUGENIX_DEBUG

			return true;
		}
	} // namespace EugenixGL
} // namespace Eugenix

#endif // EUGENIX_OPENGL
