

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

GLuint fbo;
GLuint rbo[4];
GLenum renderbuff[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
GLenum defaultbuff[1] = { GL_BACK };

//bool g_useFbo = true;
bool g_useFbo = false;

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
	else if (key == GLFW_KEY_F2 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		g_useFbo = !g_useFbo;
		//g_min_filter += 1;
		//if (g_min_filter >= sizeof(FilterState) / sizeof(FilterState[0]))
		//	g_min_filter = 0;
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterState[g_min_filter]);
		//std::cout << "g_useFbo=" << g_useFbo << std::endl;
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
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	glGenRenderbuffers(4, rbo);

	glBindRenderbuffer(GL_RENDERBUFFER, rbo[0]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, 1024, 768);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo[1]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, 1024, 768);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo[2]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, 1024, 768);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo[3]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);

	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo[0]);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, rbo[1]);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_RENDERBUFFER, rbo[2]);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo[3]);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}

int main()
{

	try {

		GlfwObj glfw;

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_DOUBLEBUFFER, )

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


		GLint winbuff;
		while (!glfwWindowShouldClose(window))
		{
			// save default render buffer
			//glGetIntegerv(GL_DRAW_BUFFER, &winbuff);

			//fprintf(stdout, "winbuff=%d, %d, g_useFbo=%d\n", winbuff, GL_BACK, g_useFbo);
			glClear(GL_COLOR_BUFFER_BIT);

			if (g_useFbo)
			{
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
				glDrawBuffers(3, renderbuff);
				//fprintf(stdout, "+++++++++++++++++++++++++++\n");
				//GLenum fboStatus =  glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
				//GLenum fboStatusR = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
				//fprintf(stdout, ">>>>   %d, %d, %d\n", fboStatus, fboStatusR, GL_FRAMEBUFFER_COMPLETE);

				
				GLfloat vClearColor[4];
				glGetFloatv(GL_COLOR_CLEAR_VALUE, vClearColor);
				glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				glClearColor(vClearColor[0], vClearColor[1], vClearColor[2], vClearColor[3]);

			}

			//glClear(GL_COLOR_BUFFER_BIT);
			glUniform1f(scaleFactor, g_scaleFactor);
			glUniform1f(scaleFactorFrag, g_scaleFactorFrag);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			if (g_useFbo)

			{
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
				glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

				//GLenum fboStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
				//GLenum fboStatusR = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
				//fprintf(stdout, "<<<<   %d, %d, %d\n", fboStatus, fboStatusR, GL_FRAMEBUFFER_COMPLETE);

				glDrawBuffer(GL_BACK);

				glReadBuffer(GL_COLOR_ATTACHMENT1);
				glBlitFramebuffer(0, 0, 1024 / 2, 768 / 2, 0, 0, 1024 / 2, 768 / 2, GL_COLOR_BUFFER_BIT, GL_NEAREST);

				glReadBuffer(GL_COLOR_ATTACHMENT0);
				glBlitFramebuffer(1024/2, 0, 1024, 768/2, 1024 / 2, 0, 1024, 768 / 2, GL_COLOR_BUFFER_BIT, GL_NEAREST);

				glReadBuffer(GL_COLOR_ATTACHMENT2);
				glBlitFramebuffer(1024 / 2, 768 / 2, 1024, 768, 1024 / 2, 768 / 2, 1024, 768, GL_COLOR_BUFFER_BIT, GL_LINEAR);


				//fprintf(stdout, "----------------------------\n");
				//glBindFramebuffer(gl_read_f)

			}



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
