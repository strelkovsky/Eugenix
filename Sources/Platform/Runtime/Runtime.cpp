//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#include "Runtime.h"

#ifdef EUGENIX_GLFW_RUNTIME

#ifdef EUGENIX_OPENGL
	#include "Platform/EugenixGL.h"
#endif

#include <GLFW/glfw3.h>

namespace Eugenix
{
	// ---------------------------------------------------------------------------------------------------------
	void key_callback(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	void resize_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	namespace Runtime
	{
		DisplaySetup _setup;
		GLFWwindow* _window = nullptr;

		bool Init(const DisplaySetup& setup)
		{
			_setup = setup;

			if (glfwInit() != GLFW_TRUE)
			{
				fprintf(stderr, "glfwInit error\n");
				return false;
			}

#ifdef EUGENIX_OPENGL
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif // EUGENIX_OPENGL

#ifdef EUGENIX_DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif // EUGENIX_DEBUG

			_window = glfwCreateWindow(_setup.width, _setup.height, "Eugenix", NULL, NULL);
			if (_window == nullptr)
			{
				fprintf(stderr, "Failed to create GLFW window\n");
				glfwTerminate();
				return false;
			}

			glfwMakeContextCurrent(_window);
			glfwSetFramebufferSizeCallback(_window, resize_callback);

#ifdef EUGENIX_OPENGL
			if (!EugenixGL::Init())
				return false;
#endif // EUGENIX_OPENGL
			return true;
		}

		void Term()
		{
			glfwTerminate();
		}

		bool RunFrame()
		{
			key_callback(_window);
			glfwPollEvents();
			return !glfwWindowShouldClose(_window);
		}

		void CommitFrame()
		{
			glfwSwapBuffers(_window);	
		}
	} // namespace Runtime
} // namespace Eugenix

#endif