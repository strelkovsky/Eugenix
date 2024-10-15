#include <cstdio>
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

#include "SimpleGLApp.h"

class SimpleApp : public SimpleGLApp
{
protected:
    virtual bool OnInit()
    {
        _model = glm::rotate(_model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        _view = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -4.0f));
        _proj = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);

        _shaderProgram = CreateProgramFromFile("../default.vs", "../default.fs");

        _shaderProgram->Use();

        _shaderProgram->Use();
        _shaderProgram->SetUniform("model", _model);
        _shaderProgram->SetUniform("view", _view);
        _shaderProgram->SetUniform("projection", _proj);
        _shaderProgram->Unuse();

        _texture = CreateTextureFromFile("../grid.png");

        _renderObject = CreateCube();

        return true;
    }

    virtual void OnRender()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _shaderProgram->Use();
        _shaderProgram->SetUniform("model", _model);

        _texture->Use();

        _renderObject->Draw();

        _shaderProgram->Unuse();
    }

    virtual void OnUpdate(float deltaTime)
    {
        _model = glm::rotate(_model, glm::radians(deltaTime * 10), glm::vec3(0.5f, 1.0f, 0.0f));
    }

private:
    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _proj;

    GLTexturePtr _texture;
    GLProgramPtr _shaderProgram;
    GLRenderObjectPtr _renderObject;
};

int main(int argc, char* argv[])
{
    SimpleApp app;
    return app.Run();
}