#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SimpleGLApp.h"

TODO("Test todo")

class SimpleApp : public SimpleGLApp
{
protected:
    virtual bool OnInit()
    {
        _view = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -7.0f));
        _proj = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);

        _vp = _proj * _view;

        _shaderProgram = ShaderEnv::Defaults::SimpleDiffuse();

        _texture = CreateTextureFromFile("../texture.tga");

        _mesh1 = CreateCubeMesh(0.5f);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.5f, 0.0f, -2.0f));

        _mesh2 = CreateIndexedCubeMesh(0.5f);
        modelMatrix2 = glm::mat4(1);
        modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(2.5f, 0.0f, -2.0f));

        return true;
    }

    virtual void OnRender()
    {
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Clear(ClearFlags::Color | ClearFlags::Depth);

        _shaderProgram->Use();
        _texture->Use();

        _mvp = _vp * modelMatrix;
        _shaderProgram->SetUniform("mvp", _mvp);
        _mesh1->Draw();

        _mvp = _vp * modelMatrix2;
        _shaderProgram->SetUniform("mvp", _mvp);
        _mesh2->Draw();

        _shaderProgram->Unuse();
    }

    virtual void OnUpdate(float deltaTime)
    {
        modelMatrix = glm::rotate(modelMatrix, glm::radians(deltaTime * 10), glm::vec3(1.0f, 1.0f, 1.0f));
        modelMatrix2 = glm::rotate(modelMatrix2, glm::radians(deltaTime * 10), glm::vec3(1.0f, 1.0f, 1.0f));
    }

private:
    glm::mat4 _view;
    glm::mat4 _proj;
    glm::mat4 _vp;
    glm::mat4 _mvp;

    GLTexturePtr _texture;
    GLProgramPtr _shaderProgram;

    glm::mat4 modelMatrix, modelMatrix2;

    GLMeshPtr _mesh1;
    GLMeshPtr _mesh2;
};

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    Eugenix::ApplicationConfig config;
    SimpleApp app;
    return app.Run(config);
}