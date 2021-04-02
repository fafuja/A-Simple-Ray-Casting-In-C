#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaders.h"
#include "object.h"

typedef enum { False = 0, True = 1 } bool;

const int wWidth = 800;
const int wHeight = 700;

float mouseX_NDC = 0.1f;
float mouseY_NDC = 0.1f;

float mousePosition[2];

bool CheckCollision(float* mouseP, Object two);
static void SceneRender();
static void ProcessInput(GLFWwindow* window);
static void ProcessMousePosition(GLFWwindow* window, double xpos, double ypos);

int main(void)
{
	GLFWwindow* window;
	if(!glfwInit())
		return -1;	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_RESIZABLE, False);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, True);
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

	Object o1 = {.position.x = wWidth*1.3f/2, .position.y = wHeight*1.3f/2, .size.x = wWidth*1.4f/2 - wWidth*1.3f/2, .size.y =  wHeight*1.5f/2 - wHeight*1.3f/2 };

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
	glVertexAttribPointer(0, 3, GL_FLOAT, False, 3 * sizeof(float), (void*)0);		
	// Enabling the attribute we just set up.
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Square (VAO[1])
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_indices), s_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, False, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//----
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);	
	
	while(!glfwWindowShouldClose(window))
	{
		r_vertices[3] = mouseX_NDC;
		
		r_vertices[4] = mouseY_NDC;

		glfwSetCursorPosCallback(window, ProcessMousePosition);
		for(float i = 0.0f; i <= mousePosition[0]; i++){
			for(int j = 0.0f; j <= mousePosition[1]; j++){
				float rayP[] = {i, j};
				bool ok = CheckCollision(rayP, o1);
				if(ok){
					printf("ok \n");
				}
			}
		}

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(r_vertices), r_vertices);	
		SceneRender(shaderProgram, VAO);	
		glfwPollEvents();
		glfwSwapBuffers(window);
		printf("-- \n");
	}

	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(1, VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}
static void ProcessMousePosition(GLFWwindow* window, double xpos, double ypos)
{
	mouseX_NDC = (float) ((2*xpos)/wWidth - 1);
	mouseY_NDC = (float) -((2*ypos)/wHeight - 1);
	mousePosition[0] = (float) xpos;
	mousePosition[1] = (float) (mouseY_NDC + 1) * wHeight / 2;
}
static void ProcessInput(GLFWwindow* window)
{

}
static void SceneRender(unsigned int shaderProgram, unsigned int *VAO)
{
	glClearColor(0.2f, 0.1f, 0.5f, 1.0fi);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glBindVertexArray(VAO[1]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
bool CheckCollision(float* mouseP, Object two) 
{
        bool collisionX; 
       	if(mouseP[0] >= two.position.x && two.position.x + two.size.x >= mouseP[0])
        {
        	collisionX = True;
	}
    	
        bool collisionY;
       	if(mouseP[1] >= two.position.y && two.position.y + two.size.y >= mouseP[1])
        {
        	collisionY = True;
        }
    	
        if(collisionX && collisionY)
        {
        	return True;
        }
        else
        {
        	return False;
        }
}
