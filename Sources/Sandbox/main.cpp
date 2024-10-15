#include <cstdio>
#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#define GLEW_STATIC
#include <gl/glew.h>

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class CChronometer
{
public:
    CChronometer()
        : m_lastTime(std::chrono::system_clock::now())
    {}

    float GrabDeltaTime()
    {
        auto newTime = std::chrono::system_clock::now();
        auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - m_lastTime);
        m_lastTime = newTime;
        return 0.001f * float(timePassed.count());
    };

private:
    std::chrono::system_clock::time_point m_lastTime;
};

glm::mat4 model;
glm::mat4 view;
glm::mat4 proj;

GLuint texture;
GLuint CreateTexture(const std::string& name)
{
    GLuint _tex;

    int width, height, comp;
    unsigned char* data = stbi_load(name.c_str(), &width, &height, &comp, STBI_rgb);

    glGenTextures(1, &_tex);
    glBindTexture(GL_TEXTURE_2D, _tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (comp == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if (comp == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return _tex;
}

std::string loadStringFromFile(const std::string& filename)
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

GLuint VBO, VAO;
GLuint shaderProgram;
bool initRenderData()
{
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -4.0f));
    proj = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);

    auto vsSource = loadStringFromFile("../default.vs");
    auto fsSource = loadStringFromFile("../default.fs");

    const GLchar* vertexShaderRaw = vsSource.c_str();
    const GLchar* fragmentShaderRaw = fsSource.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderRaw, NULL);
    glCompileShader(vertexShader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Vertex shader compilation failed - %s\n", infoLog);
        // TODO : use default material & don't stop init
        return false;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderRaw, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Fragment shader compilation failed - %s\n", infoLog);
        // TODO : use default material & don't stop init
        return false;
    }
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Shader program linking failed - %s\n", infoLog);
        // TODO : use default material & don't stop init
        return false;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

    glUseProgram(0);

    GLfloat points[] = {
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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);


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
    //glBindVertexArray(0);
    texture = CreateTexture("../grid.png");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    return true;
}

int main(int argc, char* argv[])
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

    SDL_Window* mainWindow = SDL_CreateWindow("Eugenix", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!mainWindow)
    {
        printf("Window could not be created! - %s", SDL_GetError());
        return 1;
    }

    //Create context
    SDL_GLContext oglContext = SDL_GL_CreateContext(mainWindow);
    if (oglContext == NULL)
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

        //Initialize render data
        if (!initRenderData())
        {
            printf("Unable to initialize OpenGL!\n");
            return 1;
        }


        int width, height;
        SDL_GetWindowSize(mainWindow, &width, &height);
        glViewport(0, 0, width, height);

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

    bool isRunning = true;

    CChronometer chronometer;
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

        const float deltaSeconds = chronometer.GrabDeltaTime();
       // printf("frame time: %f", deltaSeconds);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glUseProgram(0);

        SDL_GL_SwapWindow(mainWindow);       
    }

    return 0;
}