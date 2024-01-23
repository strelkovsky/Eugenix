#include "App/App.h"
#include "Platform/Runtime/Runtime.h"

#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

#include <vector>

float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
};

unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

using namespace Eugenix;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 vertexPosition_modelspace;\n"
"uniform mat4 MVP;\n"
"void main()\n"
"{\n"
// Выходная позиция нашей вершины: MVP * position
"gl_Position = MVP * vec4(vertexPosition_modelspace, 1);\n"
	"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n";

class TestApp : public App
{
public:
	virtual bool OnInit()
	{
		_shaderProgram = CreateShaderProgram(&vertexShaderSource, &fragmentShaderSource);
		if (_shaderProgram == -1)
			return false;

		glGenVertexArrays(1, &_vao);

		// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
		// 1. bind Vertex Array Object
		glBindVertexArray(_vao);
		// 2. copy our vertices array in a buffer for OpenGL to use+
		unsigned int vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		unsigned int ebo = 0;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// 3. then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 View = glm::lookAt(
			glm::vec3(4, 3, 3), // Камера находится в мировых координатах (4,3,3)
			glm::vec3(0, 0, 0), // И направлена в начало координат
			glm::vec3(0, 1, 0)  // "Голова" находится сверху
		);
		// Матрица модели : единичная матрица (Модель находится в начале координат)
		glm::mat4 Model = glm::mat4(1.0f);  // Индивидуально для каждой модели

		// Итоговая матрица ModelViewProjection, которая является результатом перемножения наших трех матриц
		_MVP = Projection * View * Model; // Помните, что умножение матрицы производиться в обратном порядке

		MatrixID = glGetUniformLocation(_shaderProgram, "MVP");

		return true;
	}
	virtual void OnRender()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(_shaderProgram);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &_MVP[0][0]);

		glBindVertexArray(_vao);
		//glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

private:

	int CreateShaderProgram(const GLchar *const* vsSource, const GLchar *const* fsSource)
	{
		unsigned int shaderProgramId = -1;

		unsigned int vertexShaderId = CreateShader(GL_VERTEX_SHADER, vsSource);
		if (vertexShaderId == -1)
			return shaderProgramId;

		unsigned int fragmentShaderId = CreateShader(GL_FRAGMENT_SHADER, fsSource);
		if (fragmentShaderId == -1)
			return shaderProgramId;

		shaderProgramId = glCreateProgram();
		glAttachShader(shaderProgramId, vertexShaderId);
		glAttachShader(shaderProgramId, fragmentShaderId);
		glLinkProgram(shaderProgramId);

		if (!CheckShaderProgramStatus(shaderProgramId))
		{
			return -1;
		}

		//glUseProgram(shaderProgram);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		return shaderProgramId;
	}

	int CreateShader(GLenum shaderType, const GLchar *const* source)
	{
		unsigned int shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, source, NULL);
		glCompileShader(shaderId);

		return CheckShaderCompilation(shaderId) ? shaderId : -1;
	}

	bool CheckShaderCompilation(unsigned int shaderId)
	{
		int  success;
		glGetShaderiv(shaderId, GL_LINK_STATUS, &success);
		if (!success)
		{
			int infoLogLength;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
			std::vector<char> infoLog(infoLogLength + 1);
			glGetShaderInfoLog(shaderId, infoLogLength, NULL, &infoLog[0]);
			printf("failed to compile shader - %s", &infoLog[0]);
			return false;
		}

		return true;
	}

	bool CheckShaderProgramStatus(unsigned int programId)
	{
		int  success;
		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success)
		{
			int infoLogLength;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
			std::vector<char> infoLog(infoLogLength + 1);
			glGetProgramInfoLog(programId, infoLogLength, NULL, &infoLog[0]);
			printf("failed to compile shader - %s", &infoLog[0]);
			return false;
		}

		return true;
	}

	unsigned int _vao = 0;
	unsigned int _shaderProgram = 0;

	GLuint MatrixID;

	glm::mat4 _MVP;
};

int main()
{
	TestApp app;
	return app.Run();
}

