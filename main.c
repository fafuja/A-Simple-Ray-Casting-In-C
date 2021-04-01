#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaders.h"
#define true 1
#define false 0

const int wWidth = 800;
const int wHeight = 700;

float mouseX_NDC = 0.5f;
float mouseY_NDC = 0.5f;

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
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif
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
	
	//----
	unsigned int VAO[2], VBO[2], EBO, vertexShader, fragmentShader, shaderProgram;
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

	// Assigning ray and square vertices
	float r_vertices[] = {
        	0.0f, 0.0f, 0.0f, 
        	mouseX_NDC, mouseY_NDC, 0.0f,
        	0.0f, 0.0f, 0.0f 
	};
	
	float s_vertices[] = {
		0.3f, 0.3f, 0.0f,
	       	0.3f, 0.5f, 0.0f,
		0.4f, 0.3f, 0.0f,
		0.4f, 0.5f, 0.0f
	};

	unsigned int s_indices[] = {
		3, 2, 1,
		2, 0, 1
	};

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	// Ray (VAO[0])
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(r_vertices), r_vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);		
	// Enabling the attribute we just set up.
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Square (VAO[1])
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_indices), s_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//----
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);	
	
	while(!glfwWindowShouldClose(window))
	{
		r_vertices[3] = mouseX_NDC;
		r_vertices[4] = mouseY_NDC;

		glfwSetCursorPosCallback(window, processMousePosition);
		processInput(window);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(r_vertices), r_vertices);	
		sceneRender(shaderProgram, VAO);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(1, VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}
static void processMousePosition(GLFWwindow* window, double xpos, double ypos)
{
	mouseX_NDC = (float) ((2*xpos)/wWidth - 1);
	mouseY_NDC = (float) -((2*ypos)/wHeight - 1);
}
static void processInput(GLFWwindow* window)
{

}
static void sceneRender(unsigned int shaderProgram, unsigned int *VAO)
{
	glClearColor(0.2f, 0.1f, 0.5f, 1.0fi);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glBindVertexArray(VAO[1]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
