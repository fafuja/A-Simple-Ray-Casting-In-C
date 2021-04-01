#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaders.h"
#define true 1
#define false 0

const int wWidth = 800;
const int wHeight = 700;

double mouseX_NDC;
double mouseY_NDC;

static void sceneRender();
static void processInput(GLFWwindow* window);
static void processMousePosition(GLFWwindow* window, double xpos, double ypos);

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
	
	if(!window)
	{
		printf("Error trying to create window. \n");
		glfwTerminate();
		return -1;
	}	

	glfwMakeContextCurrent(window);
	
	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD. \n");
		return -1;
	}

	glViewport(0,0,wWidth, wHeight);
	
	while(!glfwWindowShouldClose(window))
	{
		glfwSetCursorPosCallback(window, processMousePosition);
		processInput(window);
		sceneRender();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
static void processMousePosition(GLFWwindow* window, double xpos, double ypos)
{
	mouseX_NDC = ((2*xpos)/wWidth - 1);
	mouseY_NDC = ((2*ypos)/wHeight - 1);
	printf("%f %f \n", mouseX_NDC, mouseY_NDC);
}
static void processInput(GLFWwindow* window)
{

}

static void createObject()
{
	unsigned int VAO, VBO, vertexShader, fragmentShader, shaderProgram;
	int success;
	char infoLog[512];
	// Defining shaders and linking them
	// Compiling Vertex Shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("%s \n",infoLog);
	}
	memset(infoLog, 0, sizeof(infoLog));

	// Compiling Fragment Shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("%s \n",infoLog);
	}
	memset(infoLog, 0, sizeof(infoLog));

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("%s \n",infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Setting vertices 
	float vertices[] = {
        	0.0f, 0.0f, 0.0f,  // left 
        	0.5f, 0.5f, 0.0f,  // right
        	0.0f, 0.0f, 0.0f,  // top 
	};
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);	
	
	// Enabling the attribute we just set up.
	glEnableVertexAttribArray(0);
	
	glUseProgram(shaderProgram);
	//glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
}
static void sceneRender()
{
	glClearColor(0.2f, 0.1f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	createObject();
}

