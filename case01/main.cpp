

#include <iostream>
using namespace std;
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

int main()
{
	if (!glfwInit())
	{
		cout << "glfwIniit() failed!" << endl;
		return -1;
	}
	glfwGuard<glfwTerminate> glfwGuardRTM;

	GLFWwindow * window = glfwCreateWindow(1024, 768, "Case 01", 0, 0);
	if (!window)
	{
		return -1;
	}
	glfwGuardWin<glfwDestroyWindow> glfwWin(window);

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
