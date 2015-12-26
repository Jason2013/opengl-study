

#include <iostream>
using namespace std;
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "globjs.h"
#include "bitmap.h"

GLfloat g_scaleFactor = 1.0f;
GLfloat g_scaleFactorFrag = 1.0f;


GLint WrapState[] = {
	GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT, GL_REPEAT, GL_MIRROR_CLAMP_TO_EDGE };

const char * WrapStateName[] = { "GL_CLAMP_TO_EDGE", "GL_CLAMP_TO_BORDER", "GL_MIRRORED_REPEAT", "GL_REPEAT", "GL_MIRROR_CLAMP_TO_EDGE" };

GLint g_wrap_s = 3;
GLint g_wrap_t = 3;

GLint FilterState[] = {
	GL_NEAREST,
GL_LINEAR,
GL_NEAREST_MIPMAP_NEAREST,
GL_LINEAR_MIPMAP_NEAREST,
GL_NEAREST_MIPMAP_LINEAR,
GL_LINEAR_MIPMAP_LINEAR,
};


const char * FilterName[] = {
"GL_NEAREST",
"GL_LINEAR",
"GL_NEAREST_MIPMAP_NEAREST",
"GL_LINEAR_MIPMAP_NEAREST",
"GL_NEAREST_MIPMAP_LINEAR",
"GL_LINEAR_MIPMAP_LINEAR",
};

GLint g_mag_filter = 1;
GLint g_min_filter = 1;




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
	else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		g_scaleFactorFrag -= 0.1f;
		std::cout << "g_scaleFactorFrag=" << g_scaleFactorFrag << std::endl;
	}
	else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		g_scaleFactorFrag += 0.1f;
		std::cout << "g_scaleFactorFrag=" << g_scaleFactorFrag << std::endl;
	}
	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		//g_scaleFactor -= 0.1f;
		g_wrap_s += 1;
		if (g_wrap_s >= sizeof(WrapState) / sizeof(WrapState[0]))
			g_wrap_s = 0;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapState[g_wrap_s]);
		std::cout << "GL_TEXTURE_WRAP_S=" << WrapStateName[g_wrap_s] << std::endl;
	}
	else if (key == GLFW_KEY_T && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		//g_scaleFactor -= 0.1f;
		g_wrap_t += 1;
		if (g_wrap_t >= sizeof(WrapState) / sizeof(WrapState[0]))
			g_wrap_t = 0;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapState[g_wrap_t]);
		std::cout << "GL_TEXTURE_WRAP_T=" << WrapStateName[g_wrap_t] << std::endl;
	}
	else if (key == GLFW_KEY_G && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		g_mag_filter += 1;
		if (g_mag_filter >= 2/*sizeof(FilterState) / sizeof(FilterState[0])*/)
			g_mag_filter = 0;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterState[g_mag_filter]);
		std::cout << "GL_TEXTURE_MAG_FILTER=" << FilterName[g_mag_filter] << std::endl;
	}
	else if (key == GLFW_KEY_N && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		g_min_filter += 1;
		if (g_min_filter >= sizeof(FilterState) / sizeof(FilterState[0]))
			g_min_filter = 0;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterState[g_min_filter]);
		std::cout << "GL_TEXTURE_MIN_FILTER=" << FilterName[g_min_filter] << std::endl;
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

void SetupRC()
{
	//
}

int main()
{

	try {

		GlfwObj glfw;

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GlfwWinObj window(1024, 768, "Case 08");

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

		GLint scaleFactorFrag = glGetUniformLocation(programID, "scaleFactorFrag");
		glUniform1f(scaleFactorFrag, g_scaleFactorFrag);

		SetupRC();


		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glUniform1f(scaleFactor, g_scaleFactor);
			glUniform1f(scaleFactorFrag, g_scaleFactorFrag);
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
