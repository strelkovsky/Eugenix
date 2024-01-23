#include "App/App.h"
#include "Platform/Runtime/Runtime.h"

#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

#include <vector>

//float vertices[] = {
//	 0.5f,  0.5f, 0.0f,  // top right
//	 0.5f, -0.5f, 0.0f,  // bottom right
//	-0.5f, -0.5f, 0.0f,  // bottom left
//	-0.5f,  0.5f, 0.0f   // top left 
//};
//
//unsigned int indices[] = {  // note that we start from 0!
//	0, 1, 3,   // first triangle
//	1, 2, 3    // second triangle
//};

static const GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // Треугольник 1 : начало
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // Треугольник 1 : конец
	1.0f, 1.0f,-1.0f, // Треугольник 2 : начало
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // Треугольник 2 : конец
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

// Один цвет для каждой вершины
static const GLfloat g_color_buffer_data[] = {
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
};

using namespace Eugenix;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 vertexPosition_modelspace;\n"
"layout(location = 1) in vec3 vertexColor;\n"
"uniform mat4 MVP;\n"
"out vec3 fragmentColor;\n"
"void main()\n"
"{\n"
// Выходная позиция нашей вершины: MVP * position
"gl_Position = MVP * vec4(vertexPosition_modelspace, 1);\n"
"fragmentColor = vertexColor;\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"in vec3 fragmentColor;\n"
	"out vec3 color;\n"
	"void main()\n"
	"{\n"
	"	color = fragmentColor;\n"
	"}\n";

struct SimpleMesh
{
	float* vertices;
	unsigned int* indicies;

	unsigned int vao;

	void Test(float vertices[])
	{
		int size = sizeof(vertices);
		printf("size of mesh2 - %d", size);
	}

	SimpleMesh(float* vertices, unsigned int* indicies)
	{
		int size = sizeof(vertices) / sizeof(vertices[0]);
		printf("size of mesh - %d", size);

		vertices = vertices;
		indicies = indicies;

		glGenVertexArrays(1, &vao);

		// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
		// 1. bind Vertex Array Object
		glBindVertexArray(vao);
		// 2. copy our vertices array in a buffer for OpenGL to use+
		unsigned int vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


		GLuint colorbuffer;
		glGenBuffers(1, &colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

		//unsigned int ebo = 0;
		//glGenBuffers(1, &ebo);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

		// 3. then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);

		// Второй буфер атрибутов - цвета
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // Атрибут. Здесь необязательно указывать 1, но главное, чтобы это значение совпадало с layout в шейдере..
			3,                                // Размер
			GL_FLOAT,                         // Тип
			GL_FALSE,                         // Нормализован?
			0,                                // Шаг
			(void*)0                          // Смещение
		);
	}

	void Draw()
	{
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 индексов начинающихся с 0. -> 12 треугольников -> 6 граней.
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};

class TestApp : public App
{
public:
	virtual bool OnInit()
	{
		_shaderProgram = CreateShaderProgram(&vertexShaderSource, &fragmentShaderSource);
		if (_shaderProgram == -1)
			return false;

		//_mesh = new SimpleMesh(vertices, indices);
		//_mesh->Test(vertices);

		glGenVertexArrays(1, &_vao);

		// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
		// 1. bind Vertex Array Object
		glBindVertexArray(_vao);
		// 2. copy our vertices array in a buffer for OpenGL to use+
		unsigned int vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


		GLuint colorbuffer;
		glGenBuffers(1, &colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

		//unsigned int ebo = 0;
		//glGenBuffers(1, &ebo);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

		// 3. then set our vertex attributes pointers
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//

		// Второй буфер атрибутов - цвета
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // Атрибут. Здесь необязательно указывать 1, но главное, чтобы это значение совпадало с layout в шейдере..
			3,                                // Размер
			GL_FLOAT,                         // Тип
			GL_FALSE,                         // Нормализован?
			0,                                // Шаг
			(void*)0                          // Смещение
		);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// Включить тест глубины
		glEnable(GL_DEPTH_TEST);
		// Фрагмент будет выводиться только в том, случае, если он находится ближе к камере, чем предыдущий
		glDepthFunc(GL_LESS);

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
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 индексов начинающихся с 0. -> 12 треугольников -> 6 граней.
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//_mesh->Draw();
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
			printf("failed to link shader - %s", &infoLog[0]);
			return false;
		}

		return true;
	}

	unsigned int _vao = 0;
	unsigned int _shaderProgram = 0;

	SimpleMesh* _mesh;

	GLuint MatrixID;

	glm::mat4 _MVP;
};

int main()
{
	TestApp app;
	return app.Run();
}

