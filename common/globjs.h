#ifndef __GLOBJS_H_
#define __GLOBJS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class MyExcept
{
};

class glfwExcept : public MyExcept
{

};

class glfwInitExcept : public MyExcept
{

};

class glfwWinExcept : public MyExcept
{

};


class glewExcept : public MyExcept
{

};

class glExcept : public MyExcept
{

};

class GlfwObj
{
public:
	GlfwObj()
	{
		if (glfwInit() != GL_TRUE)
			throw glfwExcept();
	}
	~GlfwObj() { glfwTerminate(); }
};

class GlewObj
{
public:
	GlewObj()
	{
		if (glewInit() != GLEW_OK)
			throw glewExcept();
	}
};

class GlfwWinObj
{
public:
	GlfwWinObj(int width, int height, const char * title)
	{
		_win = glfwCreateWindow(width, height, title, 0, 0);
		if (!_win)
			throw glfwWinExcept();
	}
	~GlfwWinObj() { glfwDestroyWindow(_win); }
	operator GLFWwindow * () { return _win; }

private:
	GLFWwindow * _win;
};

class VertexArrayObj
{
public:
	VertexArrayObj(bool immediate= true)
	{ 
		glGenVertexArrays(1, &id);
		if (immediate)
			glBindVertexArray(id);
	}
	~VertexArrayObj() { glDeleteVertexArrays(1, &id); }
	operator GLuint() { return id; }
private:
	GLuint id;
};

class BufferObj
{
public:
	BufferObj() { glGenBuffers(1, &id); }
	~BufferObj() { glDeleteBuffers(1, &id); }
	operator GLuint() { return id; }
private:
	GLuint id;
};

#endif // __GLOBJS_H_
