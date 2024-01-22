//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#include "Runtime.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

namespace Eugenix
{
	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	void processInput(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}

	namespace Runtime
	{
		GLFWwindow* _window = nullptr;

		bool Init()
		{
			// glfw: initialize and configure
			// ------------------------------
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			// glfw window creation
			// --------------------
			_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Eugenix", NULL, NULL);
			if (_window == nullptr)
			{
				fprintf(stderr, "Failed to create GLFW window\n");
				glfwTerminate();
				return false;
			}
			glfwMakeContextCurrent(_window);
			glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

			// Initialise glew
			glewExperimental = true; // Needed in core profile
			if (glewInit() != GLEW_OK) 
			{
				fprintf(stderr, "Failed to initialize GLEW\n");
				return false;
			}

			return true;
		}

		void Term()
		{
			// glfw: terminate, clearing all previously allocated GLFW resources.
			// ------------------------------------------------------------------
			glfwTerminate();
		}

		bool RunFrame()
		{
			processInput(_window);
			
			return !glfwWindowShouldClose(_window);
		}

		void CommitFrame()
		{
			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(_window);
			glfwPollEvents();
		}
	} // namespace Runtime
} // namespace Eugenix