#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define true 1
#define false 0

const int wWidth = 800;
const int wHeight = 700;

float mouseX;
float mouseY;

void sceneRender();
void processInput();


int main(void)
{
	GLFWwindow* window;
	if(!glfwInit())
		return -1;	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(wWidth, wHeight, "Simple Ray Casting in C", NULL, NULL);
	
}
