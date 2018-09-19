// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"




unsigned int CreateVAO(float* vertices, unsigned int vertSize, int* indices, unsigned int indSize, float* colors, unsigned int colorSize);

void runProgram(GLFWwindow* window)
{
	// Enable depth (Z) buffer (accept "closest" fragment)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Configure miscellaneous OpenGL settings
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set default colour after clearing the colour buffer
	glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

	// Set up your scene here (create Vertex Array Objects, etc.)
	float vertices[27] ={-0.6f, 0.0f, 0.0f,
						  0.2f, 0.0f, 0.0f,
						   -0.2f, 0.6f, 0.0f,
					       -0.4f, 0.0f, -0.1f,
						   0.4f , -0.0f, -0.1f,
						   0.0f , 0.6f, -0.1f, 
						   -0.2f , -0.0f, -0.8f,
						  0.6f , 0.0f, -0.8f,
						   0.2f , 0.6f, -0.8f,
						  
						 };
	
	
	
	/* { 0.6f,-0.8f,-0.0f,
						0.0f, 0.4f,0.0f,
						-0.8f,-0.2f,0.0f };*/

	int indices[9] = { 0,1,2   , 3,4,5, 6,7,8};

	//{ 0,1,2};
	float colors[36] = { 0.0f, 0.0f, 1.0f, 0.3f
						,0.0f, 0.0f, 1.0f, 0.3f 
						,0.0f, 0.0f, 1.0f, 0.3f 
						
						,1.0f, 0.0f, 0.0f, 0.3f
						,1.0f, 0.0f, 0.0f, 0.3f
						,1.0f, 0.0f, 0.0f, 0.3f
						
						,0.0f, 1.0f, 0.0f, 0.3f
						,0.0f, 1.0f, 0.0f, 0.3f
						,0.0f, 1.0f, 0.0f, 0.3f
	};
	
	float transfMatrix[16] ={ 5,0,0,0,
							  0,1,0,0,
							  0,0,1,0,
							  0,0,0,1 };

	unsigned int vertLen = sizeof(vertices)/sizeof(float);
	unsigned int indLen = sizeof(indices)/sizeof(int);
	unsigned int colorLen = sizeof(colors) / sizeof(float);

	unsigned int object = 0;

	


	object = CreateVAO(vertices, vertLen, indices, indLen, colors, colorLen);

	Gloom::Shader shader;
	shader.makeBasicShader("C:\\Users\\Simon Smeets\\Documents\\Unief\\Erasmus\\TDT4195\\gloom\\gloom\\shaders\\simple.vert",
						   "C:\\Users\\Simon Smeets\\Documents\\Unief\\Erasmus\\TDT4195\\gloom\\gloom\\shaders\\simple.frag");

	
	// Rendering Loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw your scene here
		shader.activate();
		
		glBindVertexArray(object);
		glUniformMatrix4fv(1, 1, GL_TRUE, transfMatrix);
		glDrawElements(GL_TRIANGLES, indLen, GL_UNSIGNED_INT, 0);
		

		shader.deactivate();
		
		// Handle other events
		glfwPollEvents();
		handleKeyboardInput(window);

		// Flip buffers
		glfwSwapBuffers(window);
		
	}
	// destroy shader
	shader.destroy();
}
	


void handleKeyboardInput(GLFWwindow* window)
{
	// Use escape key for terminating the GLFW window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

unsigned int CreateVAO(float* vertices, unsigned int vertSize, int* indices, unsigned int indSize, float* colors, unsigned int colorSize) {
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int ColorVBO = 0;
	unsigned int IndexBuffer = 0;


	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// create VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3,GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	// create color buffer
	glGenBuffers(1, &ColorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, ColorVBO);
	glBufferData(GL_ARRAY_BUFFER, colorSize * sizeof(float), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// create index buffer
	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize * sizeof(int), indices, GL_STATIC_DRAW);

	return VAO;
}
