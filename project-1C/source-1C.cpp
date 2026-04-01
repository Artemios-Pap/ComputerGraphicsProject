//********************************
// 2023
//Αυτό το αρχείο θα το χρησιμοποιήσετε
// για να υλοποιήσετε την άσκηση 1C της OpenGL
//
//ΑΜ:  Μπιριντζης Αγγελος        Όνομα:		4741
//ΑΜ:  Παπαδοπουλος Αρτεμιος     Όνομα:		4762

//*********************************
#define _CRT_SECURE_NO_WARNINGS

#define STB_IMAGE_IMPLEMENTATION

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "stb_image.h"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;


glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

//************************************
// Η LoadShaders είναι black box για σας
//************************************
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


///****************************************************************
//  Εδω θα υλοποιήσετε την συνάρτηση της κάμερας
//****************************************************************


void camera_function()
{
	

}


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(950, 950, u8"Εργασία 1Γ – CSGBoolean Operations", NULL, NULL);


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; 
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
		
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	
	GLuint programID = LoadShaders("P1C.vertexshader", "P1C.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 40.0f),
		glm::vec3(0.0f, 0.0f, 2.5f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 ModelA = glm::mat4(1.0f);
	glm::mat4 ModelB = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVPA = Projection * View * ModelA; // Remember, matrix multiplication is the other way around
	glm::mat4 MVPB = Projection * View * ModelB;

	//SHAPE A dimensions
	float k = 10.0f;
	float h = 5.0f;
	float w = 5.0f;

	//SHAPE B dimensions
	float m = 5.0f;
	float n = 14.0f;
	float v = 5.0f;

	static const GLfloat shapeA[] =
	{
		//SHAPE A
		//5-1-2 triangle
		-(k / 2),-(h / 2),-(w / 2) + 2.5f,
		-(k / 2),-(h / 2),(w / 2) + 2.5f,
		-(k / 2),(h / 2),(w / 2) + 2.5f,

		//7-5-6
		(k / 2),(h / 2),-(w / 2) + 2.5f,
		-(k / 2),-(h / 2),-(w / 2) + 2.5f,
		-(k / 2),(h / 2),-(w / 2) + 2.5f,

		//4-5-8
		(k / 2),-(h / 2),(w / 2) + 2.5f,
		-(k / 2),-(h / 2),-(w / 2) + 2.5f,
		(k / 2),-(h / 2),-(w / 2) + 2.5f,

		//7-8-5
		(k / 2),(h / 2),-(w / 2) + 2.5f,
		(k / 2),-(h / 2),-(w / 2) + 2.5f,
		-(k / 2),-(h / 2),-(w / 2) + 2.5f,

		//5-2-6
		-(k / 2),-(h / 2),-(w / 2) + 2.5f,
		-(k / 2),(h / 2),(w / 2) + 2.5f,
		-(k / 2),(h / 2),-(w / 2) + 2.5f,

		//4-1-5
		(k / 2),-(h / 2),(w / 2) + 2.5f,
		-(k / 2),-(h / 2),(w / 2) + 2.5f,
		-(k / 2),-(h / 2),-(w / 2) + 2.5f,

		//2-1-4
		-(k / 2),(h / 2),(w / 2) + 2.5f,
		-(k / 2),-(h / 2),(w / 2) + 2.5f,
		(k / 2),-(h / 2),(w / 2) + 2.5f,

		//3-8-7
		(k / 2),(h / 2),(w / 2) + 2.5f,
		(k / 2),-(h / 2),-(w / 2) + 2.5f,
		(k / 2),(h / 2),-(w / 2) + 2.5f,

		//8-3-4
		(k / 2),-(h / 2),-(w / 2) + 2.5f,
		(k / 2),(h / 2),(w / 2) + 2.5f,
		(k / 2),-(h / 2),(w / 2) + 2.5f,

		//3-7-6
		(k / 2),(h / 2),(w / 2) + 2.5f,
		(k / 2),(h / 2),-(w / 2) + 2.5f,
		-(k / 2),(h / 2),-(w / 2) + 2.5f,

		//3-6-2
		(k / 2),(h / 2),(w / 2) + 2.5f,
		-(k / 2),(h / 2),-(w / 2) + 2.5f,
		-(k / 2),(h / 2),(w / 2) + 2.5f,

		//3-2-4
		(k / 2),(h / 2),(w / 2) + 2.5f,
		-(k / 2),(h / 2),(w / 2) + 2.5f,
		(k / 2),-(h / 2),(w / 2) + 2.5f
	};
	static const GLfloat shapeB[] =
	{
		//***************************************************************************************************************************************
		//SHAPE B
		//5-1-2 triangle
		-(m / 2),(n * 0),-(v / 2) + 2.5f,
		-(m / 2),(n * 0),(v / 2) + 2.5f,
		-(m / 2),n,(v / 2) + 2.5f,

		//7-5-6
		(m / 2),n,-(v / 2) + 2.5f ,
		-(m / 2),(n * 0),-(v / 2) + 2.5f,
		-(m / 2),n,-(v / 2) + 2.5f,

		//4-5-8
		(k / 2),(n * 0),(v / 2) + 2.5f,
		-(k / 2),(n * 0),-(v / 2) + 2.5f,
		(k / 2),(n * 0),-(v / 2) + 2.5f,

		//7-8-5
		(m / 2),n,-(v / 2) + 2.5f,
		(m / 2),(n * 0),-(v / 2) + 2.5f,
		-(m / 2),(n * 0),-(v / 2) + 2.5f,

		//5-2-6
		-(m / 2),(n * 0),-(v / 2) + 2.5f ,
		-(m / 2),n,(v / 2) + 2.5f,
		-(m / 2),n,-(v / 2) + 2.5f,

		//4-1-5
		(m / 2),(n * 0),(v / 2) + 2.5f,
		-(m / 2),(n * 0),(v / 2) + 2.5f,
		-(m / 2),(n * 0),-(v / 2) + 2.5f,

		//2-1-4
		-(m / 2),n,(v / 2) + 2.5f,
		-(m / 2),(n * 0),(v / 2) + 2.5f,
		(m / 2),(n * 0),(v / 2) + 2.5f,

		//3-8-7
		(m / 2),n,(v / 2) + 2.5f,
		(m / 2), (n * 0),-(v / 2) + 2.5f,
		(m / 2),n,-(v / 2) + 2.5f,

		//8-3-4
		(m / 2), (n * 0),-(v / 2) + 2.5f,
		(m / 2),n,(v / 2) + 2.5f,
		(m / 2), (n * 0),(v / 2) + 2.5f,

		//3-7-6
		(m / 2),n,(v / 2) + 2.5f,
		(m / 2),n,-(v / 2) + 2.5f,
		-(m / 2),n,-(v / 2) + 2.5f,

		//3-6-2
		(m / 2),n,(v / 2) + 2.5f,
		-(m / 2),n,-(v / 2) + 2.5f,
		-(m / 2),n,(v / 2) + 2.5f,

		//3-2-4
		(m / 2),n,(v / 2) + 2.5f,
		-(m / 2),n,(v / 2) + 2.5f,
		(m / 2), (n * 0),(v / 2) + 2.5f
	};

	static const GLfloat UVcords_A[]={
	0.5f,0.75f,
	0.25f,0.75f,
	0.25f,1.0f,

	0.75f,0.25f,
	0.5f,0.75f,
	0.75f,0.75f,

	0.25f,0.25f,
	0.5f,0.75f,
	0.5f,0.25f,

	0.75f,0.25f,
	0.5f,0.25f,
	0.5f,0.75f,

	0.5f,0.75f,
	0.25f,1.0f,
	0.5f,1.0f,

	0.25f,0.25f,
	0.25f,0.75f,
	0.5f,0.75f,

	0.0f,0.75f,
	0.25f,1.0f,
	0.25f,0.25f,

	0.25f,0.0f,
	0.5f,0.25f,
	0.5f,0.0f,

	0.5f,0.25f,
	0.25f,0.0f,
	0.25f,0.25f,

	1.0f,0.25f,
	0.75f,0.25f,
	0.75f,0.75f,

	1.0f,0.25f,
	0.75f,0.75f,
	1.0f,0.75f,

	0.0f,0.25f,
	0.0f,0.75f,
	0.25f,0.25f
	};

	GLuint vertexbufferA;
	glGenBuffers(1, &vertexbufferA);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shapeA), shapeA, GL_STATIC_DRAW);
	
	
	GLuint uvbufferA;
	glGenBuffers(1, &uvbufferA);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(UVcords_A), UVcords_A, GL_STATIC_DRAW);


	GLuint vertexbufferB;
	glGenBuffers(1, &vertexbufferB);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shapeB), shapeB, GL_STATIC_DRAW);
	//////////////////////////////////////////////////////
	// Load the textures here
	int width, height, channels;
	unsigned char* image_data = stbi_load("C:\GLP\GLFWx64-GLEWx64-GLM-0\texture-model-A.jpg", &width, &height, &channels, 0);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	/////////////////////////////////////////////////////

	//Assisting variables for camera movement
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 40.0f;

	
	
	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			if (X < 90.0f) {
				X += 0.1f;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			if (X > -90.0f) {
				X -= 0.1f;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			if (Y < 90.0f) {
				Y += 0.1f;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			if (Y > -90.0f) {
				Y -= 0.1f;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
			if (Z < 90.0f) {
				Z += 0.1f;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
			if (Z > -90.0f) {
				Z -= 0.1f;
			}
		}

		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(X, Y, Z),
			glm::vec3(0.0f, 0.0f, 2.5f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		
		glm::mat4 MVPA = Projection * View * ModelA;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPA[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferA);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferA);
		glVertexAttribPointer(
			1,                  
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 1 triangle

	/*	glm::mat4 MVPB = Projection * View * ModelB;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPB[0][0]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferB);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 1 triangle
		*/

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	// Cleanup VBO
	glDeleteBuffers(1, &vertexbufferA);
	glDeleteBuffers(1, &vertexbufferB);
	glDeleteBuffers(1, &uvbufferA);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);


	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;

}

