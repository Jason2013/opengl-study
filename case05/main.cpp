

#include <iostream>
using namespace std;
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "globjs.h"
#include "bitmap.h"

GLfloat g_scaleFactor = 1.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		g_scaleFactor += 0.1f;
		std::cout << "g_scaleFactor=" << g_scaleFactor << std::endl;
	}
	else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		g_scaleFactor -= 0.1f;
		std::cout << "g_scaleFactor=" << g_scaleFactor << std::endl;
	}
}

GLuint LoadBmpTex(const char * szFileName)
{
	//GLbyte* gltReadBMPBits(const char *szFileName, int *nWidth, int *nHeight);
	int width, height;
	GLbyte * data = gltReadBMPBits(szFileName, &width, &height);
	if (data == NULL)
		//return 0;
		throw FileNotFound(szFileName);

	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	return texture;
}

int main()
{

	try {

		GlfwObj glfw;

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GlfwWinObj window(1024, 768, "Case 05");

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

		static GLfloat uvBuffer[] = {
			0.0f, 0.0f,
			1.0f,  1.0f,
			0.0f,  1.0f,
			1.0f,  1.0f,
			//0.0f, 0.0f,
			1.0f, 0.0f
		};

		static GLuint indexBuffer[] = {
			0, 1, 2, 3, 0, 4
		};

		VertexArrayObj vao;

		BufferObj buffer, /*color, */uv, index;

		// vertex
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		// uv
		glBindBuffer(GL_ARRAY_BUFFER, uv);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uvBuffer), uvBuffer, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		//glVertexAttribDivisor(1, 2);
		glEnableVertexAttribArray(1);

		// offset
		//glBindBuffer(GL_ARRAY_BUFFER, offset);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(offsetBuffer), offsetBuffer, GL_STATIC_DRAW);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		//glVertexAttribDivisor(2, 1);
		//glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer), indexBuffer, GL_STATIC_DRAW);

		GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
		glUseProgram(programID);

		GLuint texture = LoadBmpTex("marble.bmp");

		GLint texID = glGetUniformLocation(programID, "texID");
		glUniform1i(texID, 0);

		GLint scaleFactor = glGetUniformLocation(programID, "scaleFactor");
		glUniform1f(scaleFactor, g_scaleFactor);


		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glUniform1f(scaleFactor, g_scaleFactor);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	catch (const FileNotFound& ex)
	{
		cerr << "Failed to open file: " << ex._filename << endl;
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
