

#include <iostream>
using namespace std;
#include <GL/glew.h>
#include <GLFW/glfw3.h>

template <decltype(glfwTerminate)* FUNC>
struct glfwGuard
{
	~glfwGuard()
	{
		(*FUNC)();
	}
};

template <decltype(glfwDestroyWindow)* FUNC>
struct glfwGuardWin
{
	glfwGuardWin(GLFWwindow * win): _win(win){}
	~glfwGuardWin()
	{
		(*FUNC)(_win);
	}
	GLFWwindow * _win;
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
	if (!glfwInit())
	{
		cout << "glfwIniit() failed!" << endl;
		return -1;
	}
	glfwGuard<glfwTerminate> glfwGuardRTM;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow * window = glfwCreateWindow(1024, 768, "Case 01", 0, 0);
	if (!window)
	{
		return -1;
	}
	glfwGuardWin<glfwDestroyWindow> glfwGuardWIN(window);

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialize GLEW!" << endl;
		return -1;
	}

	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
