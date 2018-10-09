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
#include <gloom/handout/sourceFiles/OBJLoader.hpp>
#include <gloom/handout/sourceFiles/toolbox.hpp>
#include <gloom/handout/sourceFiles/sceneGraph.hpp>




unsigned int CreateVAO(float* vertices, unsigned int vertSize, unsigned int* indices, unsigned int indSize, float* colors, unsigned int colorSize);
unsigned int meshToVAO(Mesh mesh);
SceneNode* constructSceneGraph(unsigned int groundVAO, unsigned int groundIndexCount, unsigned int characterVAO[], unsigned int characterIndexCount[]);
void visitSceneNode(SceneNode* node, glm::mat4 transformationThusFar, float time, float timeDelta, Path path);

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
	/*float vertices[27] = { 0.0f, 1.0f,-20.0f,
						  -1.0f,-1.0f,-20.0f,
						   1.0f,-1.0f,-20.0f,

					  0.2f, 1.0f,-15.0f,
					 -0.8f,-1.0f,-15.0f,
					  1.2f,-1.0f,-15.0f,

					  0.4f, 1.0f,-10.0f,
					 -0.6f,-1.0f,-10.0f,
					  1.4f,-1.0f,-10.0f

	};
	*/
	
	
	/* { 0.6f,-0.8f,-0.0f,
		 0.0f, 0.4f,0.0f,
	    -0.8f,-0.2f,0.0f };*/

	/*int indices[9] = { 0,1,2, 
					   3,4,5, 
					   6,7,8};
	*/
	//{ 0,1,2};
	/*float colors[36] = { 0.0f, 0.0f, 1.0f, 0.3f
						,0.0f, 0.0f, 1.0f, 0.3f 
						,0.0f, 0.0f, 1.0f, 0.3f 
						
						,1.0f, 0.0f, 0.0f, 0.3f
						,1.0f, 0.0f, 0.0f, 0.3f
						,1.0f, 0.0f, 0.0f, 0.3f
						
						,0.0f, 1.0f, 0.0f, 0.3f
						,0.0f, 1.0f, 0.0f, 0.3f
						,0.0f, 1.0f, 0.0f, 0.3f
	};
	*/

	
	

	MinecraftCharacter Character;
	std::string const model = "C:\\Users\\Simon Smeets\\Documents\\Unief\\Erasmus\\TDT4195\\gloom\\gloom\\handout\\res\\steve.obj";
	Character = loadMinecraftCharacterModel(model);


	Mesh meshes[6] = { Character.torso , Character.head,Character.leftArm ,Character.leftLeg ,Character.rightArm ,Character.rightLeg};
	unsigned int object[] = { 0,0,0,0,0,0 };
	unsigned int indlen[] = { 0,0,0,0,0,0 };
	for (int i = 0; i < 6; i++) {
		indlen[i] = meshes[i].indices.size();
		object[i] = meshToVAO(meshes[i]);

	}



	Mesh groundMesh = generateChessboard(10, 10, 10, float4(1, 0, 0, 1), float4(0, 1, 0, 1));
	unsigned int ground = meshToVAO(groundMesh);
	unsigned int groundindLen = groundMesh.indices.size();

	SceneNode* Scenegraph;
	Scenegraph = constructSceneGraph(ground, groundindLen, object, indlen);

	Gloom::Shader shader;
	shader.makeBasicShader("..\\gloom\\shaders\\simple.vert",
						   "..\\gloom\\shaders\\simple.frag");



	float xTrans = 0;
	float yTrans = 0;
	float zTrans = 0;
	float horRot = 0;
	float vertRot = 0;
	float time = 0;
	Path path = Path("..\\coordinates_0.txt");

	// Rendering Loop
	while (!glfwWindowShouldClose(window))
	{
		//update time
		float timeDelta = getTimeDeltaSeconds();
		time += timeDelta;

		//clear Transformation matrix
		glm::vec3 base = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::mat4x4 transfMatrix = glm::scale(base);


		// calculate projection matrix
		glm::mat4x4 proj = glm::perspective(45.0f, 1.0f, 1.0f, 500.0f);
		glm::mat4x4 look = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		


		// calculate transformation matrix
		glm::vec3 transVec = glm::vec3(xTrans, yTrans, zTrans);
		glm::mat4x4 translation = glm::translate(transVec);
		glm::mat4x4 horRotMat = glm::rotate(horRot, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4x4 vertRotMat = glm::rotate(vertRot, glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4x4 totalTranfMat =  vertRotMat  * translation * horRotMat;


		// apply and get final transfMatrix

		transfMatrix = proj * totalTranfMat * look * transfMatrix;

		// Clear colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw your scene here
		shader.activate();

		visitSceneNode(Scenegraph, transfMatrix, time, timeDelta, path);
		if (path.hasWaypointBeenReached(float2(Scenegraph->children[1]->position.x, Scenegraph->children[1]->position.z), 10.0)) {
			path.advanceToNextWaypoint();
			std::cout << Scenegraph->children[1]->rotation.y << "\n";
		}

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

unsigned int CreateVAO(float* vertices, unsigned int vertSize, unsigned int* indices, unsigned int indSize, float* colors, unsigned int colorSize) {
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

unsigned int meshToVAO(Mesh mesh) {
	std::vector<float3> vert;
	vert.resize(0);
	for (int j = 0; j < mesh.vertices.size(); j++) {
		vert.push_back(mesh.vertices[j].toFloat3());
	}


	unsigned int vertLen = vert.size() * 3;
	unsigned int indLen = mesh.indices.size();

	unsigned int colorLen = mesh.colours.size() * 4;

	return CreateVAO(&vert[0].x, vertLen, mesh.indices.data(), indLen, &mesh.colours[0].x, colorLen);

}

SceneNode* constructSceneGraph(unsigned int groundVAO, unsigned int groundIndexCount, unsigned int characterVAO[], unsigned int characterIndexCount[]) {

	//// hierarchy setup
	// root node of graph
	SceneNode* rootNode = createSceneNode();
	// children of root node
	SceneNode* GroundNode = createSceneNode();
	SceneNode* TorsoNode = createSceneNode();
	addChild(rootNode, GroundNode);
	addChild(rootNode, TorsoNode);
	// children of Torso node
	SceneNode* HeadNode = createSceneNode();
	SceneNode* LeftArmNode = createSceneNode();
	SceneNode* LeftLegNode = createSceneNode();
	SceneNode* RightArmNode = createSceneNode();
	SceneNode* RightLegNode = createSceneNode();

	addChild(TorsoNode, HeadNode);
	addChild(TorsoNode, LeftArmNode);
	addChild(TorsoNode, LeftLegNode);
	addChild(TorsoNode, RightArmNode);
	addChild(TorsoNode, RightLegNode);

	//// Initialization

	//start without translation or rotation, just set VAO's and indexcounts

	GroundNode->VAOIndexCount = groundIndexCount;
	GroundNode->vertexArrayObjectID = groundVAO;

	TorsoNode->VAOIndexCount = characterIndexCount[0];
	TorsoNode->vertexArrayObjectID = characterVAO[0];

	HeadNode->VAOIndexCount = characterIndexCount[1];
	HeadNode->vertexArrayObjectID = characterVAO[1];

	LeftArmNode->vertexArrayObjectID = characterVAO[2];
	LeftArmNode->VAOIndexCount = characterIndexCount[2];

	LeftLegNode->VAOIndexCount = characterIndexCount[3];
	LeftLegNode->vertexArrayObjectID = characterVAO[3];

	RightArmNode->VAOIndexCount = characterIndexCount[4];
	RightArmNode->vertexArrayObjectID = characterVAO[4];

	RightLegNode->VAOIndexCount = characterIndexCount[5];
	RightLegNode->vertexArrayObjectID = characterVAO[5];


	//set reference points
	TorsoNode->referencePoint = float3(0, 12, 0);
	HeadNode->referencePoint = float3(0, 24, 0);
	LeftArmNode->referencePoint = float3(-4, 24, 0);
	LeftLegNode->referencePoint = float3(-2, 12, 0);
	RightArmNode->referencePoint = float3(4, 24, 0);
	RightLegNode->referencePoint = float3(2, 12, 0);

	return rootNode;
}


void visitSceneNode(SceneNode* node, glm::mat4 transformationThusFar, float time , float timeDelta, Path path) {
	
	// Do transformations here
	glm::mat4 transl = glm::translate(glm::vec3(-node->referencePoint.x, -node->referencePoint.y, -node->referencePoint.z));
	glm::mat4 rot = glm::rotate(node->rotation.z, glm::vec3(0, 0, 1)) * glm::rotate(node->rotation.x, glm::vec3(1, 0, 0)) * glm::rotate(node->rotation.y, glm::vec3(0, 1, 0));
	glm::mat4 translInv = glm::translate(glm::vec3(node->referencePoint.x, node->referencePoint.y, node->referencePoint.z));
	glm::mat4 totalRot = translInv * rot * transl;

	glm::mat4 translation = glm::translate(glm::vec3(node->position.x, node->position.y, node->position.z));

	glm::mat4 combinedTransformation =  transformationThusFar * translation * totalRot ;
	// Do rendering here

	if (node->vertexArrayObjectID == 1) {
		
		
		
		
		

		float2 nextTarget = path.getCurrentWaypoint(10.0);
		float2 vector = nextTarget - float2(node->position.x, node->position.z);
		glm::vec2 norm =glm::normalize( glm::vec2(vector.x, vector.y));
		node->position = float3(node->position.x + norm.x * timeDelta*2,node->position.y , node->position.z + norm.y * timeDelta*2);
	
	
		glm::vec2 vecx = glm::vec2(1.0, 0);
		float angle = asin(dot(norm, vecx));

		node->rotation.y = angle;


		node->rotation.y += sin(time) / 8;
	
	}



		

	if (node->vertexArrayObjectID == 3) {
		node->rotation.x = sin(time) /2;
	}
	if (node->vertexArrayObjectID == 5) {
		node->rotation.x = -sin(time) / 2;
	}
	if (node->vertexArrayObjectID == 6) {
		node->rotation.x = sin(time) / 2;
	}
	if (node->vertexArrayObjectID == 4) {
		node->rotation.x = -sin(time) / 2;
	}
	



	glBindVertexArray(node->vertexArrayObjectID);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(combinedTransformation));
	glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, 0);





	for (SceneNode* child : node->children) {
		visitSceneNode(child, combinedTransformation, time, timeDelta, path);
	}
}


