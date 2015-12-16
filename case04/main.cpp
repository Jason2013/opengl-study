

#include <iostream>
using namespace std;
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "globjs.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{

	try {

		GlfwObj glfw;

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GlfwWinObj window(1024, 768, "Case 04");

		glfwMakeContextCurrent(window);

		glfwSetKeyCallback(window, key_callback);

		GlewObj glew;

		glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

		static GLfloat vertexBuffer[] = {
			-0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f,
			 0.6f,  0.5f,
			//-0.5f, -0.5f,
			 0.5f, -0.5f
		};

		static GLfloat colorBuffer[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
		};

		static GLfloat offsetBuffer[] = {
			0.05f, 0.05f,
			0.10f, 0.10f,
			0.15f, 0.15f,
			0.20f, 0.20f,
			0.25f, 0.25f,
			0.30f, 0.30f,
		};

		static GLuint indexBuffer[] = {
			0, 1, 2, 3, 0, 4
		};

		VertexArrayObj vao;

		BufferObj buffer, color, offset, index;

		// vertex
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		// color
		glBindBuffer(GL_ARRAY_BUFFER, color);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colorBuffer), colorBuffer, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribDivisor(1, 2);
		glEnableVertexAttribArray(1);

		// offset
		glBindBuffer(GL_ARRAY_BUFFER, offset);
		glBufferData(GL_ARRAY_BUFFER, sizeof(offsetBuffer), offsetBuffer, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribDivisor(2, 1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer), indexBuffer, GL_STATIC_DRAW);

		GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
		glUseProgram(programID);

		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 6);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	catch (const glfwInitExcept& /*e*/)
	{
		cerr << "Failed to initialize GLFW!" << endl;
	}
	catch (const glewExcept& /*e*/)
	{
		cerr << "Failed to initialize GLEW!" << endl;
	}
	catch (const glfwWinExcept& /*e*/)
	{
		cerr << "Failed to create window!" << endl;
	}

	return 0;
}
