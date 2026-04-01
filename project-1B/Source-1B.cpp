//********************************
//Αυτό το αρχείο θα το χρησιμοποιήσετε
// για να υλοποιήσετε την άσκηση 1B της OpenGL
//
//ΑΜ:    4741     Όνομα: Μπιριντζης Αγγελος
//ΑΜ:    4762     Όνομα: Παπαδοπουλος Αρτεμιος

//*********************************

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>



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

/// <summary>
/// //////////////////////////////////////////////
/// </summary>
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;


glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}



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
///////////////////////////////////////////////////



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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(950, 950, u8"Εργασία 1Β", NULL, NULL);


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark green background
	glClearColor(0.3f, 0.3f, 0.3f, 0.0f); //Window background color set to dark gray 
	glEnable(GL_DEPTH_TEST);
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders

	GLuint programID = LoadShaders("P1BVertexShader.vertexshader", "P1BFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 30.0f), 
		glm::vec3(0.0f, 0.0f, 2.5f), 
		glm::vec3(0.0f, 1.0f, 0.0f)  
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 ModelAB = glm::mat4(1.0f);
	glm::mat4 ModelC = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVPAB = Projection * View * ModelAB; // Remember, matrix multiplication is the other way around
	glm::mat4 MVPC = Projection * View * ModelC;
	
	//half values 
	GLfloat len = 5.0f, wid=2.5f, heig=2.5f;
	
	//SHAPE A dimensions
	float k = 10.0f ;
	float h = 5.0f ;
	float w = 5.0f ;

	//SHAPE B dimensions
	float m = 5.0f;
	float n = 14.0f;
	float v = 5.0f;

	//SHAPE C dimensions
	static const float f = 2.0f;

	
	static const GLfloat shapesA_B[] =
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
		(k / 2),-(h / 2),(w / 2) + 2.5f,

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
	static const GLfloat shapeC[] =
	{
		//**************************************************************************************************************************8
		//SHAPE C
		//5-1-2 triangle
		-(f / 2), -(f / 2)-5.0f, -(f / 2) -4.0f,
		-(f / 2), -(f / 2)-5.0f, (f / 2) - 4.0f,
		-(f / 2), (f / 2)-5.0f, (f / 2) - 4.0f,

		//7-5-6
		(f / 2), (f / 2)-5.0f, -(f / 2) - 4.0f,
		-(f / 2), -(f / 2)-5.0f, -(f / 2) - 4.0f,
		-(f / 2), (f / 2)-5.0f, -(f / 2) - 4.0f,

		//4-5-8
		(f / 2), -(f / 2)-5.0f, (f / 2) - 4.0f,
		-(f / 2), -(f / 2)-5.0f, -(f / 2) - 4.0f,
		(f / 2), -(f / 2)-5.0f, -(f / 2) - 4.0f,

		//7-8-5
		(f / 2), (f / 2)-5.0f, -(f / 2) - 4.0f,
		(f / 2), -(f / 2)-5.0f, -(f / 2) - 4.0f,
		-(f / 2), -(f / 2)-5.0f, -(f / 2) - 4.0f,

		//5-2-6
		-(f / 2), -(f / 2)-5.0f, -(f / 2) - 4.0f,
		-(f / 2), (f / 2)-5.0f, (f / 2) - 4.0f,
		-(f / 2), (f / 2)-5.0f, -(f / 2) - 4.0f,

		//4-1-5
		(f / 2), -(f / 2)-5.0f, (f / 2) - 4.0f,
		-(f / 2), -(f / 2)-5.0f, (f / 2) - 4.0f,
		-(f / 2), -(f / 2)-5.0f, -(f / 2) - 4.0f,

		//2-1-4
		-(f / 2), (f / 2)-5.0f, (f / 2) - 4.0f,
		-(f / 2), -(f / 2)-5.0f, (f / 2) - 4.0f,
		(f / 2), -(f / 2)-5.0f, (f / 2) - 4.0f,

		//3-8-7
		(f / 2), (f / 2)-5.0f, (f / 2) - 4.0f,
		(f / 2), -(f / 2)-5.0f, -(f / 2) - 4.0f,
		(f / 2), (f / 2)-5.0f, -(f / 2) - 4.0f,

		//8-3-4
		(f / 2), -(f / 2)-5.0f, -(f / 2) - 4.0f,
		(f / 2), (f / 2)-5.0f, (f / 2) - 4.0f,
		(f / 2), -(f / 2)-5.0f, (f / 2) - 4.0f,

		//3-7-6
		(f / 2), (f / 2)-5.0f, (f / 2) - 4.0f,
		(f / 2), (f / 2)-5.0f, -(f / 2) - 4.0f,
		-(f / 2), (f / 2)-5.0f, -(f / 2) - 4.0f,

		//3-6-2
		(f / 2), (f / 2)-5.0f, (f / 2) - 4.0f,
		-(f / 2), (f / 2)-5.0f, -(f / 2) - 4.0f,
		-(f / 2), (f / 2)-5.0f, (f / 2) - 4.0f,

		//3-2-4
		(f / 2), (f / 2)-5.0f, (f / 2) - 4.0f,
		-(f / 2), (f / 2)-5.0f, (f / 2) - 4.0f,
		(f / 2), -(f / 2)-5.0f, (f / 2) - 4.0f

	};

	GLfloat a=0.5f;
	//Gia ta sxhmata A kai B dinoume tuxaies times gia ton xrwmatismo kai dinoume sto sxhma A diafaneia (a=0.5f)
	static const GLfloat colorA_B[] = {
		//SHAPE A COLOUR
		0.583f,  0.771f,  0.014f,a,
		0.609f,  0.115f,  0.436f,a,
		0.327f,  0.483f,  0.844f,a,
		0.822f,  0.569f,  0.201f,a,
		0.435f,  0.602f,  0.223f,a,
		0.310f,  0.747f,  0.185f,a,
		0.597f,  0.770f,  0.761f,a,
		0.559f,  0.436f,  0.730f,a,
		0.359f,  0.583f,  0.152f,a,
		0.483f,  0.596f,  0.789f,a,
		0.559f,  0.861f,  0.639f,a,
		0.195f,  0.548f,  0.859f,a,
		0.014f,  0.184f,  0.576f,a,
		0.771f,  0.328f,  0.970f,a,
		0.406f,  0.615f,  0.116f,a,
		0.676f,  0.977f,  0.133f,a,
		0.971f,  0.572f,  0.833f,a,
		0.140f,  0.616f,  0.489f,a,
		0.997f,  0.513f,  0.064f,a,
		0.945f,  0.719f,  0.592f,a,
		0.543f,  0.021f,  0.978f,a,
		0.279f,  0.317f,  0.505f,a,
		0.167f,  0.620f,  0.077f,a,
		0.347f,  0.857f,  0.137f,a,
		0.055f,  0.953f,  0.042f,a,
		0.714f,  0.505f,  0.345f,a,
		0.783f,  0.290f,  0.734f,a,
		0.722f,  0.645f,  0.174f,a,
		0.302f,  0.455f,  0.848f,a,
		0.225f,  0.587f,  0.040f,a,
		0.517f,  0.713f,  0.338f,a,
		0.053f,  0.959f,  0.120f,a,
		0.393f,  0.621f,  0.362f,a,
		0.673f,  0.211f,  0.457f,a,
		0.820f,  0.883f,  0.371f,a,
		0.982f,  0.099f,  0.879f,a,
		//SHAPE B COLOUR ********************************************************************************************
		0.875f, 0.244f, 0.748f, 1.0f,
		0.738f, 0.101f, 0.555f, 1.0f,
		0.326f, 0.421f, 0.628f, 1.0f,
		0.459f, 0.013f, 0.237f, 1.0f,
		0.809f, 0.313f, 0.583f, 1.0f,
		0.712f, 0.799f, 0.220f, 1.0f,
		0.635f, 0.725f, 0.300f, 1.0f,
		0.487f, 0.525f, 0.857f, 1.0f,
		0.914f, 0.993f, 0.024f, 1.0f,
		0.615f, 0.741f, 0.491f, 1.0f,
		0.114f, 0.632f, 0.889f, 1.0f,
		0.892f, 0.118f, 0.609f, 1.0f,
		0.389f, 0.420f, 0.465f, 1.0f,
		0.866f, 0.225f, 0.301f, 1.0f,
		0.693f, 0.583f, 0.041f, 1.0f,
		0.247f, 0.909f, 0.780f, 1.0f,
		0.175f, 0.738f, 0.986f, 1.0f,
		0.654f, 0.568f, 0.125f, 1.0f,
		0.805f, 0.337f, 0.932f, 1.0f,
		0.981f, 0.571f, 0.268f, 1.0f,
		0.500f, 0.400f, 0.089f, 1.0f,
		0.035f, 0.844f, 0.049f, 1.0f,
		0.326f, 0.779f, 0.113f, 1.0f,
		0.487f, 0.129f, 0.472f, 1.0f,
		0.947f, 0.672f, 0.222f, 1.0f,
		0.112f, 0.765f, 0.948f, 1.0f,
		0.551f, 0.185f, 0.308f, 1.0f,
		0.369f, 0.602f, 0.757f, 1.0f,
		0.806f, 0.715f, 0.776f, 1.0f,
		0.034f, 0.919f, 0.402f, 1.0f,
		0.660f, 0.489f, 0.714f, 1.0f,
		0.925f, 0.841f, 0.379f, 1.0f,
		0.849f, 0.861f, 0.616f, 1.0f,
		0.487f, 0.666f, 0.542f, 1.0f,
		0.904f, 0.755f, 0.298f, 1.0f,
		0.171f, 0.961f, 0.977f, 1.0f
	};

	//To sxhma C to xrwmatizoume aspro
	static const GLfloat colorC[] = {
		//SHAPE C COLOUR ************************************************************************************
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f,
		1.0f,  1.0f,  1.0f,1.0f
	}; 
	
	//Buffer for shapes A and B
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shapesA_B), shapesA_B, GL_STATIC_DRAW);
	//Buffer for shape C
	GLuint vertexbuffer2;
	glGenBuffers(1, &vertexbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shapeC), shapeC, GL_STATIC_DRAW);

	//Color buffer for shapes A and B
	GLuint colorbufferA_B;
	glGenBuffers(1, &colorbufferA_B);
	glBindBuffer(GL_ARRAY_BUFFER, colorbufferA_B);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorA_B), colorA_B, GL_STATIC_DRAW);
	//Color buffer for shape C
	GLuint colorbufferC;
	glGenBuffers(1, &colorbufferC);
	glBindBuffer(GL_ARRAY_BUFFER, colorbufferC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorC), colorC, GL_STATIC_DRAW);

	//Assisting variables for camera movement
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 30.0f;

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		// Use our shader
		glUseProgram(programID);


		//Scale up on X axis
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			ModelAB = glm::scale(ModelAB, glm::vec3(1.01f, 1.0f, 1.0f));
		}

		// Scale down on X axis
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			ModelAB = glm::scale(ModelAB, glm::vec3(0.99f, 1.0f, 1.0f));
		}

		//Scale up on Y axis
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			ModelAB = glm::scale(ModelAB, glm::vec3(1.0f, 1.01f, 1.0f));
		}

		//Scale down on Y axis
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
			ModelAB = glm::scale(ModelAB, glm::vec3(1.0f, 0.99f, 1.0f));
		}
		//Scale up on Z axis
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			ModelAB = glm::scale(ModelAB, glm::vec3(1.0f, 1.0f, 1.01f));
		}

		//Scale down on Z axis
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
			ModelAB = glm::scale(ModelAB, glm::vec3(1.0f, 1.0f, 0.99f));
		}
		
		//Me tis parakatw is allazoume thn thesi ths kameras
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
			if(Z < 90.0f) {
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
		
		//MVP gia ta sxhmata A kai B wste na exoun metablhto megethos xwris na epireazetai to sxhma C 
		glm::mat4 MVPAB = Projection * View * ModelAB; 

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPAB[0][0]);
		//Drawing and coloring for shapes A and B
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbufferA_B);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0,72); // 3 indices starting at 0 -> 1 triangle

		//MVP gia to sxhma C
		glm::mat4 MVPC = Projection * View * ModelC;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPC[0][0]);

		//Drawing and coloring of shape C
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbufferC);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);
		//glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the SPACE key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &vertexbuffer2);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

