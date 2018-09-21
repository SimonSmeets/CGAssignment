// Local headers

#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>





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
	float vertices[27] = { 0.0f, 1.0f,-20.0f,
						  -1.0f,-1.0f,-20.0f,
						   1.0f,-1.0f,-20.0f,

					  0.2f, 1.0f,-15.0f,
					 -0.8f,-1.0f,-15.0f,
					  1.2f,-1.0f,-15.0f,

					  0.4f, 1.0f,-10.0f,
					 -0.6f,-1.0f,-10.0f,
					  1.4f,-1.0f,-10.0f

	};
	
	
	
	/* { 0.6f,-0.8f,-0.0f,
		 0.0f, 0.4f,0.0f,
	    -0.8f,-0.2f,0.0f };*/

	int indices[9] = { 0,1,2, 
					   3,4,5, 
					   6,7,8};

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
	

	
	
	
	unsigned int vertLen = sizeof(vertices)/sizeof(float);
	unsigned int indLen = sizeof(indices)/sizeof(int);
	unsigned int colorLen = sizeof(colors) / sizeof(float);

	unsigned int object = 0;

	
	
	object = CreateVAO(vertices, vertLen, indices, indLen, colors, colorLen);

	Gloom::Shader shader;
	shader.makeBasicShader("..\\gloom\\shaders\\simple.vert",
						   "..\\gloom\\shaders\\simple.frag");

	/*glm::vec3 base = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec4 test = glm::vec4(0.0f, 1.0f, -10.0f, 1.0f);
	glm::mat4x4 transfMatrix = glm::scale(base);
	glm::mat4x4 proj = glm::perspective(45.0f, 1.0f, 1.0f, 100.0f);
	glm::mat4x4 look = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	transfMatrix = proj * look;

	std::cout << glm::to_string(transfMatrix *test) << std::endl;
	*/

	float xTrans = 0;
	float yTrans = 0;
	float zTrans = 0;
	float horRot = 0;
	float vertRot = 0;
	

	// Rendering Loop
	while (!glfwWindowShouldClose(window))
	{
		//clear Transformation matrix
		glm::vec3 base = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::mat4x4 transfMatrix = glm::scale(base);

		// calculate projection matrix
		glm::mat4x4 proj = glm::perspective(45.0f, 1.0f, 1.0f, 100.0f);
		glm::mat4x4 look = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		
		//std::cout << glm::to_string(proj) << std::endl;


		// calculate transformation matrix
		glm::vec3 transVec = glm::vec3(xTrans, yTrans, zTrans);
		glm::mat4x4 translation = glm::translate(transVec);
		glm::mat4x4 horRotMat = glm::rotate(horRot, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4x4 vertRotMat = glm::rotate(vertRot, glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4x4 totalTranfMat =  vertRotMat * horRotMat * translation  ;


		// apply and get final transfMatrix

		transfMatrix = proj * totalTranfMat * look * transfMatrix;

		// Clear colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw your scene here
		shader.activate();
		glBindVertexArray(object);
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(transfMatrix));
		glDrawElements(GL_TRIANGLES, indLen, GL_UNSIGNED_INT, 0);
		

		shader.deactivate();
		
		// Handle other events
		glfwPollEvents();
		float speedControlTrans = 180;
		float speedControlRot = 360;
		handleKeyboardInput(window);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			xTrans -= 1.0f/speedControlTrans;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			xTrans += 1.0f / speedControlTrans;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			zTrans += 1.0f / speedControlTrans;

		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			zTrans -= 1.0f / speedControlTrans;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
			yTrans += 1.0f / speedControlTrans;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
			yTrans -= 1.0f / speedControlTrans;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			horRot -= 1.0f / speedControlRot;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			horRot += 1.0f / speedControlRot;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			vertRot += 1.0f / speedControlRot;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			vertRot -= 1.0f / speedControlRot;
		}
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
