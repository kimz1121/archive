// #include <glad/glad>

#include "../dependencies/GLAD/include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

//창크기 변화 인터럽트 콜백

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int main()
{
	//초기 설정
	glfwInit();
	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1000, 600, "HI window", NULL, NULL);


	//콜백 함수 등록
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	
	if(window == NULL)
	{
		cout<<"fialed to make window"<<endl;
		glfwTerminate();
		return -1;
	}	
	glfwMakeContextCurrent(window);

	if(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == NULL)
	{
		cout<<"failed to init GLAD"<<endl;
		return -1;
	}

	glViewport(0, 0, 1000, 600);

	



	glfwTerminate();
	return 0;
}


