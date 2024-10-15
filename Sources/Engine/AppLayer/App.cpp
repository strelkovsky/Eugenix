#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>


#define GLEW_STATIC
#include <gl/glew.h>

#include <SDL.h>

#include "App.h"

const int DEFAULT_WIDTH = 1024, DEFAULT_HEIGHT = 768;

std::string App::loadStringFromFile(const std::string& filename)
{
    std::ifstream in;
    // Open a stream to the file.
    in.open(filename.c_str());
    if (!in)
    {
        std::cerr << filename + " is not a valid file." << std::endl;
        return "";
    }
    std::stringstream buffer;
    // Read the stream in a buffer.
    buffer << in.rdbuf();
    // Create a string based on the content of the buffer.
    std::string line = buffer.str();
    in.close();
    return line;
}

int App::Run()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("SDL_Init error - %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    _window = SDL_CreateWindow("Eugenix", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!_window)
    {
        printf("Window could not be created! - %s", SDL_GetError());
        return 1;
    }

    _glContext = SDL_GL_CreateContext(_window);
    if (_glContext == NULL)
    {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    else
    {
        //Initialize GLEW
        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        if (glewError != GLEW_OK)
        {
            printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
        }

        //Use Vsync
        if (SDL_GL_SetSwapInterval(1) < 0)
        {
            printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
        }

        int width, height;
        SDL_GetWindowSize(_window, &width, &height);
        glViewport(0, 0, width, height);

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearDepth(1.0f);

        GLint nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

        printf("Vendor:         %s\n", glGetString(GL_VENDOR));
        printf("Renderer:       %s\n", glGetString(GL_RENDERER));
        printf("Version:        %s\n", glGetString(GL_VERSION));
        printf("Shader version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

        GLint major, minor;
        GLint nExtensions;

        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
        std::clog << "########## ContextLog ##########" << std::endl;
        std::clog << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::clog << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::clog << "Version string: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        std::clog << "Version number: " << major << "." << minor << std::endl;
        std::clog << "OpenGL extensions supported:" << std::endl;
        for (int i = 0; i < nExtensions; ++i)
            std::clog << glGetStringi(GL_EXTENSIONS, i) << std::endl;
        std::clog << "################################" << std::endl;
    }

    if (!OnInit())
        return 1;

    auto status = InternalMainLoop();
    OnShutdown();

    SDL_Quit();
}

int App::InternalMainLoop()
{
    _chronometer.Reset();

    bool isRunning = true;
    SDL_Event event;
    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    isRunning = false;
                break;
            }
        }

        const float deltaTime = _chronometer.GrabDeltaTime();        
        OnUpdate(deltaTime);
        OnRender();

        SDL_GL_SwapWindow(_window);
    }

    return 0;
}