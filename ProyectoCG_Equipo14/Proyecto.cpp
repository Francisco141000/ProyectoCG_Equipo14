// Proyecto final
// 316118732
// Fecha de entrega: 20 de mayo de 2026

#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void Animation();
void CaminarHumano();
void MoverMotor();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 2.0f, 6.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

 //Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 5.9f,  20.0f),
	glm::vec3(0.0f, 5.9f,  6.66f),
	glm::vec3(0.0f, 5.9f, -6.66f),
	glm::vec3(0.0f, 5.9f, -20.0f),
	glm::vec3(-18.0f, 5.9f, -31.0f),
	glm::vec3(-9.0f,5.9f, -31.0f),
	glm::vec3(0.0f, 5.9f, -31.0f),
	glm::vec3(9.0f, 5.9f, -31.0f),
	glm::vec3(0.0f, 5.9f, -40.0f),
	glm::vec3(9.0f, 5.9f, -40.0f),
	glm::vec3(0.0f, 5.9f, 38.0f),
	glm::vec3(0.0f, 5.9f, 31.0f),
	glm::vec3(6.5f, 5.9f, 31.0f),
	glm::vec3(6.5f, 5.9f, 38.0f),
};

// Anim Camioneta
float carrPosX, carrPosY, carrPosZ;
float acelerar = 0.0f;
float girarLlanta = 0.0f;
float girarCarr = 0.0f;
float traslacionStandCamioneta = -10.0f;
bool animCamioneta = false;

// Anim Stands Básicos
int anim_Stands = 0;
float traslacion_StandBasico = -5.0f;
float rotacion_StandBasico = 0.0f;
float escala_StandBasico = 0.01f;

// Anim Stands Opcionales
float traslacion_StandOpcional = -10.0f;
float rotacon_StandOpcional = 0.0f;
float escala_StandOpcional = 0.01f;

// Anim Humano (Flujo de personas)
int animFlujo = 0;
float humPosX = 6.5f;
float humPosY = 1.921f;
float humPosZ = 31.0f;
float rotCuerpo = -90.0f;
float rotBrazos = 0.0f;
float rotPiernas = 0.0f;
float escalaHumano = 0.0f;
bool step = false;

// Anim Brazo Robotico
int animBrazo = -1;
float mesaPosX = 6.5f;
float mesaPosY = 0.77f;
float mesaPosZ = 31.0f;
float rotMesa = 90.0f;
float rotBase = 0.0f;
float rotParte1 = 0.0f;
float rotParte2 = 0.0f;
float rotParte3 = 0.0f;

// Anim Dron
int animDron = 0;
float dronPosX = 0.0f;
float dronPosY = 0.3f;
float dronPosZ = 0.0f;
float rotMotor = 0.0f;

//KeyFrames
float dogPosX, dogPosY, dogPosZ;
#define MAX_FRAMES 400
int i_max_steps = 40;
int i_curr_steps = 0;

typedef struct _frame {
	float carrPosX;
	float carrPosY;
	float carrPosZ;
	float incCarrX;
	float incCarrY;
	float incCarrZ;
	float acelerar;
	float incAcelerar;
	float girarLlanta;
	float incGirarLlanta;
	float girarCarr;
	float incGirarCarr;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].carrPosX = carrPosX;
	KeyFrame[FrameIndex].carrPosY = carrPosY;
	KeyFrame[FrameIndex].carrPosZ = carrPosZ;
	KeyFrame[FrameIndex].acelerar = acelerar;
	KeyFrame[FrameIndex].girarLlanta = girarLlanta;
	KeyFrame[FrameIndex].girarCarr = girarCarr;

	ofstream MyFile("animacion.txt", std::ios::app);

	MyFile << carrPosX << " "
		<< carrPosY << " "
		<< carrPosZ << " "
		<< acelerar << " "
		<< girarLlanta << " "
		<< girarCarr << "\n";

	MyFile.close();

	FrameIndex++;
}

void resetElements(void)
{
	carrPosX = KeyFrame[0].carrPosX;
	carrPosY = KeyFrame[0].carrPosY;
	carrPosZ = KeyFrame[0].carrPosZ;
	acelerar = KeyFrame[0].acelerar;
	girarLlanta = KeyFrame[0].girarLlanta;
	girarCarr = KeyFrame[0].girarCarr;
}
void interpolation(void)
{
	KeyFrame[playIndex].incCarrX = (KeyFrame[playIndex + 1].carrPosX - KeyFrame[playIndex].carrPosX) / i_max_steps;
	KeyFrame[playIndex].incCarrY = (KeyFrame[playIndex + 1].carrPosY - KeyFrame[playIndex].carrPosY) / i_max_steps;
	KeyFrame[playIndex].incCarrZ = (KeyFrame[playIndex + 1].carrPosZ - KeyFrame[playIndex].carrPosZ) / i_max_steps;
	KeyFrame[playIndex].incAcelerar = (KeyFrame[playIndex + 1].acelerar - KeyFrame[playIndex].acelerar) / i_max_steps;
	KeyFrame[playIndex].incGirarLlanta = (KeyFrame[playIndex + 1].girarLlanta - KeyFrame[playIndex].girarLlanta) / i_max_steps;
	KeyFrame[playIndex].incGirarCarr = (KeyFrame[playIndex + 1].girarCarr - KeyFrame[playIndex].girarCarr) / i_max_steps;
}

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	
	// Create a GLFWwindow object that we can use for GLFW's functions
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Equipo 14 - Proyecto final", nullptr, nullptr);
	glfwSetCursorPos(window, 1920 / 2, 1080 / 2);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader skyboxshader("Shader/SkyBox.vs", "Shader/SkyBox.frag");

	// Carga Modelo Puente
	Model Piso((char*)"Models/Piso/Piso.obj");
	Model Columnas((char*)"Models/Columnas/Columnas.obj");
	Model Techo((char*)"Models/Techo/Techo.obj");
	Model Laterales((char*)"Models/Laterales/Laterales.obj");

	// Carga Modelo Stand 1
	Model Banco_Stand1((char*)"Models/Stand_1/Banco_Stand1.obj");
	Model Escritorio_Stand1((char*)"Models/Stand_1/Escritorio_Stand1.obj");
	Model Logo_Stand1((char*)"Models/Stand_1/Logo_Stand1.obj");
	Model Mesa_Stand1((char*)"Models/Stand_1/Mesa_Stand1.obj");
	Model Paredes_Stand1((char*)"Models/Stand_1/Paredes_Stand1.obj");
	Model Piso_Stand1((char*)"Models/Stand_1/Piso_Stand1.obj");
	Model Silla_Stand1((char*)"Models/Stand_1/Silla_Stand1.obj");
	
	// Carga Modelo Stand 2
	Model Caja_Stand2((char*)"Models/Stand_2/Caja_Stand2.obj");
	Model Escritorio_Stand2((char*)"Models/Stand_2/Escritorio_Stand2.obj");
	Model Logo_Stand2((char*)"Models/Stand_2/Logo_Stand2.obj");
	Model Mesa_Stand2((char*)"Models/Stand_2/Mesa_Stand2.obj");
	Model Plataforma_Stand2((char*)"Models/Stand_2/Plataforma_Stand2.obj");

	// Carga Modelo Stand 3
	Model Mesa_Stand3((char*)"Models/Stand_3/Mesa_Stand3.obj");
	Model Paredes_Stand3((char*)"Models/Stand_3/Paredes_Stand3.obj");
	Model Piso_Stand3((char*)"Models/Stand_3/Piso_Stand3.obj");

	// Carga Modelo Stand 4
	Model Caja_Stand4((char*)"Models/Stand_4/Caja_Stand4.obj");
	Model Logos_Stand4((char*)"Models/Stand_4/Logos_Stand4.obj");
	Model Paredes_Stand4((char*)"Models/Stand_4/Paredes_Stand4.obj");
	Model Piso_Stand4((char*)"Models/Stand_4/Piso_Stand4.obj");
	Model Repisas_Stand4((char*)"Models/Stand_4/Repisas_Stand4.obj");

	// Carga Modelo Stand Básico
	Model Stand_Basico((char*)"Models/Stand_Basico/Stand_Basico.obj");
	Model Cartel1((char*)"Models/Carteles/Cartel1.obj");
	Model Cartel2((char*)"Models/Carteles/Cartel2.obj");
	Model Cartel3((char*)"Models/Carteles/Cartel3.obj");
	Model Cartel4((char*)"Models/Carteles/Cartel4.obj");
	Model Cartel5((char*)"Models/Carteles/Cartel5.obj");
	Model Cartel6((char*)"Models/Carteles/Cartel6.obj");
	Model Cartel7((char*)"Models/Carteles/Cartel7.obj");
	Model Cartel8((char*)"Models/Carteles/Cartel8.obj");

	// Carga Modelo Entorno
	Model Techo_Entorno((char*)"Models/Entorno/Techo_Entorno.obj");
	Model Reja_Entorno((char*)"Models/Entorno/Reja_Entorno.obj");
	Model Piso_Entorno((char*)"Models/Entorno/Piso_Entorno.obj");
	Model Paredes_Entorno((char*)"Models/Entorno/Paredes_Entorno.obj");
	Model Pantallas_Entorno((char*)"Models/Entorno/Pantallas_Entorno.obj");
	Model Mural_Entorno((char*)"Models/Entorno/Mural_Entorno.obj");
	Model Laterales_Entorno((char*)"Models/Entorno/Laterales_Entorno.obj");
	Model Columnas_Entorno((char*)"Models/Entorno/Columnas_Entorno.obj");
	Model Stand_Camioneta((char*)"Models/Stand_Camioneta/Stand_Camioneta.obj");

	// Carga Modelo Lampara
	Model Lampara((char*)"Models/Lampara/Lampara.obj");

	// Carga Modelo Camioneta 
	Model Carroceria((char*)"Models/Camioneta/Carroceria.obj");
	Model LTI((char*)"Models/Camioneta/LTI.obj");
	Model LTD((char*)"Models/Camioneta/LTD.obj");
	Model LDI((char*)"Models/Camioneta/LDI.obj");
	Model LDD((char*)"Models/Camioneta/LDD.obj");

	// Carga Modelo Humano
	Model Cuerpo((char*)"Models/Humano/Cuerpo.obj");
	Model Brazo_Izquierdo((char*)"Models/Humano/Brazo_Izquierdo.obj");
	Model Brazo_Derecho((char*)"Models/Humano/Brazo_Derecho.obj");
	Model Pierna_Izquierda((char*)"Models/Humano/Pierna_Izquierda.obj");
	Model Pierna_Derecha((char*)"Models/Humano/Pierna_Derecha.obj");

	// Carga Modelo Brazo Robotico
	Model Mesa_Brazo((char*)"Models/Brazo_Robotico/Mesa_Brazo.obj");
	Model Base_Brazo((char*)"Models/Brazo_Robotico/Base_Brazo.obj");
	Model Parte1_Brazo((char*)"Models/Brazo_Robotico/Parte1_Brazo.obj");
	Model Parte2_Brazo((char*)"Models/Brazo_Robotico/Parte2_Brazo.obj");
	Model Parte3_Brazo((char*)"Models/Brazo_Robotico/Parte3_Brazo.obj");

	// Carga Modelo Dron
	Model Cuerpo_Dron((char*)"Models/Dron/Cuerpo_Dron.obj");
	Model Motor1_Dron((char*)"Models/Dron/Motor1_Dron.obj");
	Model Motor2_Dron((char*)"Models/Dron/Motor2_Dron.obj");
	Model Motor3_Dron((char*)"Models/Dron/Motor3_Dron.obj");
	Model Motor4_Dron((char*)"Models/Dron/Motor4_Dron.obj");

	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].carrPosX = 0;
		KeyFrame[i].carrPosY = 0;
		KeyFrame[i].carrPosZ = 0;
		KeyFrame[i].incCarrX = 0;
		KeyFrame[i].incCarrY = 0;
		KeyFrame[i].incCarrZ = 0;
		KeyFrame[i].acelerar = 0;
		KeyFrame[i].incAcelerar = 0;
		KeyFrame[i].girarLlanta = 0;
		KeyFrame[i].incGirarLlanta = 0;
		KeyFrame[i].girarCarr = 0;
		KeyFrame[i].incGirarCarr = 0;
	}


	// Leer contenido del archivo
	ifstream MyReadFile("animacion.txt");
	int i = 0;

	// Leer los datos mientras el archivo tenga contenido y no superemos MAX_FRAMES
	while (i < MAX_FRAMES && MyReadFile
		>> KeyFrame[i].carrPosX
		>> KeyFrame[i].carrPosY
		>> KeyFrame[i].carrPosZ
		>> KeyFrame[i].acelerar
		>> KeyFrame[i].girarLlanta
		>> KeyFrame[i].girarCarr)
	{
		i++;
	}

	FrameIndex = i;
	if (FrameIndex > 0)
	{
		resetElements();
	}

	MyReadFile.close();

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	// Skybox
	GLuint skyboxVBO, skyboxVAO, EBO;
	
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);

	// Load Textures (Skybox)
	vector <const GLchar*> faces;
	faces.push_back("SkyBox/right.jpg");
	faces.push_back("SkyBox/left.jpg");
	faces.push_back("SkyBox/top.jpg");
	faces.push_back("SkyBox/bottom.jpg");
	faces.push_back("SkyBox/back.jpg");
	faces.push_back("SkyBox/front.jpg");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		
		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 0.0f, -1.0f, -0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.15f, 0.15f, 0.15f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.8f, 0.8f, 0.8f);

		// Point light
		for (int i = 0; i < 14; i++) {
			string baseName = "pointLights[" + to_string(i) + "].";
			glUniform3f(glGetUniformLocation(lightingShader.Program, (baseName + "position").c_str()), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (baseName + "ambient").c_str()), 0.05f, 0.05f, 0.05f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (baseName + "diffuse").c_str()), 0.8f, 0.8f, 0.8f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (baseName + "specular").c_str()), 1.0f, 1.0f, 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (baseName + "constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (baseName + "linear").c_str()), 0.09f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (baseName + "quadratic").c_str()), 0.032f);
		}

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), 0.45f, 0.0f, 32.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 0.45f, 6.0f, 50.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), animCamioneta * 0.1f, animCamioneta * 0.1f, animCamioneta * 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), animCamioneta * 1.0f, animCamioneta * 0.9f, animCamioneta * 0.9f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), animCamioneta * 1.0f, animCamioneta * 1.0f, animCamioneta * 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), animCamioneta * 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), animCamioneta * 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(animCamioneta * 12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(animCamioneta * 17.5f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 1.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);
		
		//Carga de modelo 
        view = camera.GetViewMatrix();


		// Despliegue Modelo Puente

		model = glm::mat4(1);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Piso.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.001f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Columnas.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.002f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Techo.Draw(lightingShader);

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Laterales.Draw(lightingShader);

		
		// Despliegue Modelo Entorno

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Techo_Entorno.Draw(lightingShader);

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Reja_Entorno.Draw(lightingShader);

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso_Entorno.Draw(lightingShader);

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Paredes_Entorno.Draw(lightingShader);

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pantallas_Entorno.Draw(lightingShader);

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mural_Entorno.Draw(lightingShader);

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Laterales_Entorno.Draw(lightingShader);

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Columnas_Entorno.Draw(lightingShader);


		// Despliegue Modelo Dron

		if (animDron > 0)
		{
			MoverMotor();

			model = glm::mat4(1.0f);
			modelTemp = model = glm::translate(model, glm::vec3(dronPosX, dronPosY, dronPosZ));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Cuerpo_Dron.Draw(lightingShader);

			model = modelTemp;
			model = glm::translate(model, glm::vec3(-0.21f, 0.02f, 0.21f));
			model = glm::rotate(model, glm::radians(rotMotor), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Motor1_Dron.Draw(lightingShader);

			model = modelTemp;
			model = glm::translate(model, glm::vec3(0.21f, 0.025f, 0.21f));
			model = glm::rotate(model, glm::radians(rotMotor), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Motor2_Dron.Draw(lightingShader);

			model = modelTemp;
			model = glm::translate(model, glm::vec3(-0.21f, 0.02f, -0.21f));
			model = glm::rotate(model, glm::radians(rotMotor), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Motor3_Dron.Draw(lightingShader);

			model = modelTemp;
			model = glm::translate(model, glm::vec3(0.21f, 0.02f, -0.21f));
			model = glm::rotate(model, glm::radians(rotMotor), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Motor4_Dron.Draw(lightingShader);
		}


		// Despliegue Modelo Brazo Robotico

		if (anim_Stands == 2 and animBrazo > -1)
		{
			model = glm::mat4(1.0f);
			modelTemp = model = glm::translate(model, glm::vec3(mesaPosX, mesaPosY, mesaPosZ));
			modelTemp = model = glm::rotate(model, glm::radians(rotMesa), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Mesa_Brazo.Draw(lightingShader);

			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotBase), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Base_Brazo.Draw(lightingShader);

			model = glm::translate(model, glm::vec3(0.0f, 0.24f, 0.0f));
			model = glm::rotate(model, glm::radians(rotParte1), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Parte1_Brazo.Draw(lightingShader);

			model = glm::translate(model, glm::vec3(-0.02f, 0.33f, -0.38f));
			model = glm::rotate(model, glm::radians(rotParte2), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Parte2_Brazo.Draw(lightingShader);

			model = glm::translate(model, glm::vec3(0.0f, -0.4f, -0.65f));
			model = glm::rotate(model, glm::radians(rotParte3), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Parte3_Brazo.Draw(lightingShader);
		}


		// Despliegue Modelo Humano

		if (anim_Stands == 0 and animFlujo != 0)
		{
			CaminarHumano();
			
			model = glm::mat4(1.0f);
			modelTemp = model = glm::translate(model, glm::vec3(humPosX, humPosY, humPosZ));
			modelTemp = model = glm::rotate(model, glm::radians(rotCuerpo), glm::vec3(0.0f, 1.0f, 0.0f));
			modelTemp = model = glm::scale(model, glm::vec3(escalaHumano));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Cuerpo.Draw(lightingShader);

			model = modelTemp;
			model = glm::translate(model, glm::vec3(0.24f, 0.14f, 0.02f));
			model = glm::rotate(model, glm::radians(rotBrazos), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Brazo_Izquierdo.Draw(lightingShader);

			model = modelTemp;
			model = glm::translate(model, glm::vec3(-0.26f, 0.15f, 0.02f));
			model = glm::rotate(model, glm::radians(rotBrazos), glm::vec3(-1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Brazo_Derecho.Draw(lightingShader);

			model = modelTemp;
			model = glm::translate(model, glm::vec3(0.108f, -0.85f, 0.02f));
			model = glm::rotate(model, glm::radians(rotPiernas), glm::vec3(-1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Pierna_Izquierda.Draw(lightingShader);

			model = modelTemp;
			model = glm::translate(model, glm::vec3(-0.109f, -0.85f, 0.026f));
			model = glm::rotate(model, glm::radians(rotPiernas), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Pierna_Derecha.Draw(lightingShader);
		}


		// Despliegue Modelo Lampara

		/*for (int i = 0; i < 14; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(pointLightPositions[i]));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Lampara.Draw(lightingShader);
		}*/


		// Despliegue Modelo Camioneta

		if (anim_Stands == 0 and animCamioneta == true)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacionStandCamioneta, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Stand_Camioneta.Draw(lightingShader);

			model = glm::mat4(1.0f);
			modelTemp = model = glm::translate(model, glm::vec3(carrPosX, carrPosY, carrPosZ));
			modelTemp = model = glm::rotate(model, glm::radians(girarCarr), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Carroceria.Draw(lightingShader);

			model = modelTemp;
			model = glm::translate(model, glm::vec3(1.174f, 0.543f, 2.45f));
			model = glm::rotate(model, glm::radians(girarLlanta), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(acelerar), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			LTI.Draw(lightingShader);

			model = modelTemp;
			model = glm::translate(model, glm::vec3(-1.174f, 0.543f, 2.45f));
			model = glm::rotate(model, glm::radians(girarLlanta), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(acelerar), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			LTD.Draw(lightingShader);

			model = modelTemp;
			model = glm::translate(model, glm::vec3(1.174f, 0.543f, -2.143f));
			model = glm::rotate(model, glm::radians(acelerar), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			LDI.Draw(lightingShader);

			model = modelTemp;
			model = glm::translate(model, glm::vec3(-1.174f, 0.543f, -2.143f));
			model = glm::rotate(model, glm::radians(acelerar), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			LDD.Draw(lightingShader);
		}


		// Depsliegue Modelos Stand Básico

		if (anim_Stands == 1)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandBasico, 0.0f));
			model = glm::rotate(model, glm::radians(rotacion_StandBasico), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandBasico));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Stand_Basico.Draw(lightingShader);
			Cartel8.Draw(lightingShader);
			
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandBasico, -41.4f));
			model = glm::rotate(model, glm::radians(rotacion_StandBasico + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandBasico));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Stand_Basico.Draw(lightingShader);
			Cartel7.Draw(lightingShader);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandBasico, 13.8f));
			model = glm::rotate(model, glm::radians(rotacion_StandBasico), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandBasico));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Stand_Basico.Draw(lightingShader);
			Cartel6.Draw(lightingShader);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandBasico, -27.6f));
			model = glm::rotate(model, glm::radians(rotacion_StandBasico + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandBasico));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Stand_Basico.Draw(lightingShader);
			Cartel5.Draw(lightingShader);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandBasico, 27.6f));
			model = glm::rotate(model, glm::radians(rotacion_StandBasico), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandBasico));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Stand_Basico.Draw(lightingShader);
			Cartel4.Draw(lightingShader);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandBasico, -13.8f));
			model = glm::rotate(model, glm::radians(rotacion_StandBasico + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandBasico));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Stand_Basico.Draw(lightingShader);
			Cartel3.Draw(lightingShader);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandBasico, 41.4f));
			model = glm::rotate(model, glm::radians(rotacion_StandBasico), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandBasico));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Stand_Basico.Draw(lightingShader);
			Cartel2.Draw(lightingShader);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandBasico, 0.0f));
			model = glm::rotate(model, glm::radians(rotacion_StandBasico + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandBasico));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Stand_Basico.Draw(lightingShader);
			Cartel1.Draw(lightingShader);
		}


		// Despliegue Modelos Stands Opcionales

		if (anim_Stands == 2)
		{
			// Despliegue Modelo Stand 1

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandOpcional, 0.0f));
			model = glm::rotate(model, glm::radians(rotacon_StandOpcional), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandOpcional));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Piso_Stand1.Draw(lightingShader);
			Paredes_Stand1.Draw(lightingShader);

			float offsetMuebles = glm::min(0.0f, traslacion_StandOpcional * 2.0f);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, offsetMuebles, 0.0f));
			model = glm::rotate(model, glm::radians(rotacon_StandOpcional), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandOpcional));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Escritorio_Stand1.Draw(lightingShader);
			Mesa_Stand1.Draw(lightingShader);
			Banco_Stand1.Draw(lightingShader);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, offsetMuebles, 0.0f));
			model = glm::rotate(model, glm::radians(rotacon_StandOpcional * 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandOpcional));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla_Stand1.Draw(lightingShader);

			float logoScale = glm::max(0.0f, (escala_StandOpcional - 0.5f) * 2.0f);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandOpcional, 0.0f));
			model = glm::scale(model, glm::vec3(logoScale));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Logo_Stand1.Draw(lightingShader);


			// Despliegue Modelo Stand 2

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandOpcional, 0.0f));
			model = glm::rotate(model, glm::radians(rotacon_StandOpcional), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandOpcional));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Plataforma_Stand2.Draw(lightingShader);

			float traslacionEmergente = glm::min(0.0f, traslacion_StandOpcional * 1.5f);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacionEmergente, 0.0f));
			model = glm::rotate(model, glm::radians(rotacon_StandOpcional), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandOpcional));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Caja_Stand2.Draw(lightingShader);
			Escritorio_Stand2.Draw(lightingShader);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacionEmergente, 0.0f));
			model = glm::rotate(model, glm::radians(-rotacon_StandOpcional), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandOpcional));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Mesa_Stand2.Draw(lightingShader);

			float escalaLogo2 = glm::max(0.0f, (escala_StandOpcional - 0.5f) * 2.0f);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandOpcional, 0.0f));
			model = glm::scale(model, glm::vec3(escalaLogo2));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Logo_Stand2.Draw(lightingShader);


			// Despliegue Modelo Stand 3

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandOpcional, 0.0f));
			model = glm::rotate(model, glm::radians(rotacon_StandOpcional), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandOpcional));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Piso_Stand3.Draw(lightingShader);

			float traslacionParedes = glm::min(0.0f, traslacion_StandOpcional * 1.2f);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacionParedes, 0.0f));
			model = glm::rotate(model, glm::radians(rotacon_StandOpcional), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandOpcional));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Paredes_Stand3.Draw(lightingShader);

			float escalaMesa = glm::pow(escala_StandOpcional, 2.0f);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandOpcional, 0.0f));
			model = glm::rotate(model, glm::radians(-rotacon_StandOpcional), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escalaMesa));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Mesa_Stand3.Draw(lightingShader);


			// Despliegue Modelo Stand 4

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandOpcional, 0.0f));
			model = glm::rotate(model, glm::radians(rotacon_StandOpcional), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandOpcional));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Piso_Stand4.Draw(lightingShader);

			traslacionParedes = glm::min(0.0f, traslacion_StandOpcional * 1.2f);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacionParedes, 0.0f));
			model = glm::rotate(model, glm::radians(rotacon_StandOpcional), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escala_StandOpcional));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Paredes_Stand4.Draw(lightingShader);

			float escalaDetalles = escala_StandOpcional * escala_StandOpcional;
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandOpcional, 0.0f));
			model = glm::rotate(model, glm::radians(rotacon_StandOpcional), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escalaDetalles));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Caja_Stand4.Draw(lightingShader);
			Repisas_Stand4.Draw(lightingShader);

			float escalaLogos = glm::max(0.0f, (escala_StandOpcional - 0.6f) * 2.5f);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, traslacion_StandOpcional, 0.0f));
			model = glm::rotate(model, glm::radians(-rotacon_StandOpcional), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escalaLogos));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Logos_Stand4.Draw(lightingShader);
		}

		glBindVertexArray(0);

		// Draw SkyBox
		glDepthFunc(GL_LEQUAL);
		skyboxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVAO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{	
	// Camera controls

	if (keys[GLFW_KEY_W])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	// Camioneta controls

	if (keys[GLFW_KEY_UP])
	{
		carrPosZ += 0.1f;
		acelerar += 0.8f;
	}

	if (keys[GLFW_KEY_DOWN])
	{
		carrPosZ -= 0.1f;
		acelerar -= 0.8f;
	}

	if (keys[GLFW_KEY_LEFT])
	{
		carrPosX += 0.1f;
	}

	if (keys[GLFW_KEY_RIGHT])
	{
		carrPosX -= 0.1f;
	}

	if (keys[GLFW_KEY_Q])
	{
		if (girarLlanta < 35.0f)
		{
			girarLlanta += 0.5f;
		}
		girarCarr += 0.9;
		acelerar = 0;
	}

	if (keys[GLFW_KEY_E])
	{
		if (girarLlanta > -35.0f)
		{
			girarLlanta -= 0.5f;
		}
		girarCarr -= 0.9;
		acelerar = 0;
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (keys[GLFW_KEY_Z])
	{
		anim_Stands += 1;
		if (anim_Stands > 2) anim_Stands = 0;
	}

	if (keys[GLFW_KEY_X])
	{
		animCamioneta = !animCamioneta;
	}

	if (keys[GLFW_KEY_C])
	{
		animFlujo++;
	}

	if (keys[GLFW_KEY_V])
	{
		animBrazo++;
	}

	if (keys[GLFW_KEY_B])
	{
		animDron++;
	}

	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{
			resetElements();

			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}
	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void Animation() {

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter

				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			carrPosX += KeyFrame[playIndex].incCarrX;
			carrPosY += KeyFrame[playIndex].incCarrY;
			carrPosZ += KeyFrame[playIndex].incCarrZ;
			acelerar += KeyFrame[playIndex].incAcelerar;
			girarLlanta += KeyFrame[playIndex].incGirarLlanta;
			girarCarr += KeyFrame[playIndex].incGirarCarr;

			i_curr_steps++;
		}
	}

	// Máquina de estados animación stand básico
	if (anim_Stands == 1)
	{
		if (escala_StandBasico < 1.0f) escala_StandBasico += 0.005f;
		if (rotacion_StandBasico < 360.0f) rotacion_StandBasico += 1.5f;
		if (traslacion_StandBasico < 0.0f) traslacion_StandBasico += 0.025f;
	}
	else
	{
		traslacion_StandBasico = -5.0f;
		rotacion_StandBasico = 0.0f;
		escala_StandBasico = 0.01f;
	}

	if (anim_Stands == 2)
	{
		if (traslacion_StandOpcional < 0.0f) traslacion_StandOpcional += 0.05f;
		if (rotacon_StandOpcional < 360.0f) rotacon_StandOpcional += 1.5f;
		if (escala_StandOpcional < 1.0f) escala_StandOpcional += 0.005f;
	}
	else
	{
		traslacion_StandOpcional = -10.0f;
		rotacon_StandOpcional = 0.0f;
		escala_StandOpcional = 0.01f;
	}

	// Máquina de estados animación stand camioneta
	if (animCamioneta)
	{
		if (traslacionStandCamioneta < 0) traslacionStandCamioneta += 0.1;
	}

	//Máquina de estados animación flujo de humanos
	if (animFlujo == 0)
	{
		humPosX = 6.5f;
		humPosY = 1.921f;
		humPosZ = 31.0f;
		rotCuerpo = -90.0f;
		rotBrazos = 0.0f;
		rotPiernas = 0.0f;
		escalaHumano = 0.0f;
		step = false;
	}

	if (animFlujo == 1)
	{
		escalaHumano += 0.01f;
		if (escalaHumano > 0.9) animFlujo = 2;
	}

	if (animFlujo == 2)
	{
		humPosX -= 0.01f;
		if (humPosX < 0.0f) animFlujo = 3;
	}

	if (animFlujo == 3)
	{
		rotCuerpo -= 0.5f;
		if (rotCuerpo < -180.0f) animFlujo = 4;
	}

	if (animFlujo == 4)
	{
		humPosZ -= 0.01f;
		if (humPosZ < -32.0f) animFlujo = 5;
	}

	if (animFlujo == 5)
	{
		rotCuerpo -= 0.5f;
		if (rotCuerpo < -270.0f) animFlujo = 6;
	}

	if (animFlujo == 6)
	{
		humPosX += 0.01f;
		if (humPosX > 8.0f) animFlujo = 7;
	}

	if (animFlujo == 7)
	{
		escalaHumano -= 0.01f;
		if (escalaHumano < 0) animFlujo = 8;
	}

	if (animFlujo == 8)
	{
		rotCuerpo = 90.0f;
		humPosX = -18.0f;
		humPosZ = -31.0f;
		escalaHumano += 0.01f;
		if (escalaHumano > 0.9) animFlujo = 9;
	}

	if (animFlujo == 9)
	{
		humPosX += 0.01;
		if (humPosX > 0) animFlujo = 10;
	}

	if (animFlujo == 10)
	{
		rotCuerpo -= 0.5f;
		if (rotCuerpo < 0.0f) animFlujo = 11;
	}

	if (animFlujo == 11)
	{
		humPosZ += 0.01f;
		if (humPosZ > 38.0f) animFlujo = 12;
	}

	if (animFlujo == 12)
	{
		rotCuerpo += 0.5f;
		if (rotCuerpo > 90.0f) animFlujo = 13;
	}

	if (animFlujo == 13)
	{
		humPosX += 0.01f;
		if (humPosX > 6.5f) animFlujo = 14;
	}

	if (animFlujo == 14)
	{
		escalaHumano -= 0.01f;
		if (escalaHumano < 0) animFlujo = 0;
	}

	//Máquina de estados animación Brazo Robótico
	if (animBrazo == 0)
	{
		mesaPosX = 6.5f;
		mesaPosY = 0.77f;
		mesaPosZ = 31.0f;
		rotMesa = 90.0f;
		rotBase = 0.0f;
		rotParte1 = 0.0f;
		rotParte2 = 0.0f;
		rotParte3 = 0.0f;
	}

	if (animBrazo == 1)
	{
		mesaPosX -= 0.01f;
		if (mesaPosX < 0.0f) animBrazo = 2;
	}

	if (animBrazo == 2)
	{
		rotMesa -= 0.5f;
		if (rotMesa < 0) animBrazo = 3;
	}

	if (animBrazo == 3)
	{
		mesaPosZ -= 0.01f;
		if (mesaPosZ < 17.0f) animBrazo = 4;
	}

	if (animBrazo == 4)
	{
		rotBase += 0.5f;
		if (rotBase > 90) animBrazo = 5;
	}

	if (animBrazo == 5)
	{
		rotParte1 += 0.5f;
		if (rotParte1 > 20.0f)
		{
			rotParte2 += 0.5f;
			if (rotParte2 > 30.0f)
			{
				animBrazo = 6;
			}
		}
	}

	if (animBrazo == 6)
	{
		rotParte3 += 0.5f;
		if (rotParte3 > 70.0f) animBrazo = 7;
	}

	if (animBrazo == 7)
	{
		rotParte3 -= 0.5f;
		if (rotParte3 < -30.0f) animBrazo = 8;
	}

	if (animBrazo == 8)
	{
		rotParte1 -= 0.5f;
		if (rotParte1 < 0.0f)
		{
			rotParte2 -= 0.5f;
			if (rotParte2 < 0.0f)
			{
				animBrazo = 9;
			}
		}
	}

	if (animBrazo == 9)
	{
		rotBase -= 0.5f;
		if (rotBase < 0.0f) animBrazo = 10;
	}

	if (animBrazo == 10)
	{
		mesaPosZ -= 0.01f;
		if (mesaPosZ < 3.0f) animBrazo = 11;
	}

	if (animBrazo == 11)
	{
		rotBase -= 0.5f;
		if (rotBase < -90.0f) animBrazo = 12;
	}

	if (animBrazo == 12)
	{
		rotParte1 += 0.5f;
		if (rotParte1 > 20.0f)
		{
			rotParte2 += 0.5f;
			if (rotParte2 > 30.0f)
			{
				animBrazo = 13;
			}
		}
	}

	if (animBrazo == 13)
	{
		rotParte3 += 0.5f;
		if (rotParte3 > 70.0f) animBrazo = 14;
	}

	if (animBrazo == 14)
	{
		rotParte3 -= 0.5f;
		if (rotParte3 < -30.0f) animBrazo = 15;
	}

	if (animBrazo == 15)
	{
		rotParte1 -= 0.5f;
		if (rotParte1 < 0.0f)
		{
			rotParte2 -= 0.5f;
			if (rotParte2 < 0.0f)
			{
				animBrazo = 16;
			}
		}
	}

	if (animBrazo == 16)
	{
		rotBase += 0.5f;
		if (rotBase > 0.0f) animBrazo = 17;
	}

	if (animBrazo == 17)
	{
		mesaPosZ -= 0.01f;
		if (mesaPosZ < -11.0f) animBrazo = 18;
	}

	if (animBrazo == 18)
	{
		rotBase += 0.5f;
		if (rotBase > 90.0f) animBrazo = 19;
	}

	if (animBrazo == 19)
	{
		rotParte1 += 0.5f;
		if (rotParte1 > 20.0f)
		{
			rotParte2 += 0.5f;
			if (rotParte2 > 30.0f)
			{
				animBrazo = 20;
			}
		}
	}

	if (animBrazo == 20)
	{
		rotParte3 += 0.5f;
		if (rotParte3 > 70.0f) animBrazo = 21;
	}

	if (animBrazo == 21)
	{
		rotParte3 -= 0.5f;
		if (rotParte3 < -30.0f) animBrazo = 22;
	}

	if (animBrazo == 22)
	{
		rotParte1 -= 0.5f;
		if (rotParte1 < 0.0f)
		{
			rotParte2 -= 0.5f;
			if (rotParte2 < 0.0f)
			{
				animBrazo = 23;
			}
		}
	}

	if (animBrazo == 23)
	{
		rotBase -= 0.5f;
		if (rotBase < 0.0f) animBrazo = 24;
	}

	if (animBrazo == 24)
	{
		mesaPosZ -= 0.01f;
		if (mesaPosZ < -23.0f) animBrazo = 25;
	}

	if (animBrazo == 25)
	{
		rotBase -= 0.5f;
		if (rotBase < -90.0f) animBrazo = 26;
	}

	if (animBrazo == 26)
	{
		rotParte1 += 0.5f;
		if (rotParte1 > 20.0f)
		{
			rotParte2 += 0.5f;
			if (rotParte2 > 30.0f)
			{
				animBrazo = 27;
			}
		}
	}

	if (animBrazo == 27)
	{
		rotParte3 += 0.5f;
		if (rotParte3 > 70.0f) animBrazo = 28;
	}

	if (animBrazo == 28)
	{
		rotParte3 -= 0.5f;
		if (rotParte3 < -30.0f) animBrazo = 29;
	}

	if (animBrazo == 29)
	{
		rotParte1 -= 0.5f;
		if (rotParte1 < 0.0f)
		{
			rotParte2 -= 0.5f;
			if (rotParte2 < 0.0f)
			{
				animBrazo = 30;
			}
		}
	}

	if (animBrazo == 30)
	{
		rotBase += 0.5f;
		if (rotBase > 0.0f) animBrazo = 31;
	}

	if (animBrazo == 31)
	{
		mesaPosZ -= 0.01f;
		if (mesaPosZ < -31.0f) animBrazo = 32;
	}

	if (animBrazo == 32)
	{
		rotBase -= 0.5f;
		if (rotBase < -180.0f) animBrazo = 33;
	}

	if (animBrazo == 33)
	{
		rotParte1 += 0.5f;
		if (rotParte1 > 20.0f)
		{
			rotParte2 += 0.5f;
			if (rotParte2 > 30.0f)
			{
				animBrazo = 34;
			}
		}
	}

	if (animBrazo == 34)
	{
		rotParte3 += 0.5f;
		if (rotParte3 > 70.0f) animBrazo = 35;
	}

	if (animBrazo == 35)
	{
		rotParte3 -= 0.5f;
		if (rotParte3 < -30.0f) animBrazo = 36;
	}

	if (animBrazo == 36)
	{
		rotParte1 -= 0.5f;
		if (rotParte1 < 0.0f)
		{
			rotParte2 -= 0.5f;
			if (rotParte2 < 0.0f)
			{
				animBrazo = -1;
			}
		}
	}

	//Máquina de estados animación Dron
	if (animDron == 1)
	{
		dronPosY += 0.02f;
		if (dronPosY >= 4.5f) animDron = 2;
	}

	if (animDron == 2)
	{
		if (dronPosX > -18.0f) dronPosX -= 0.1f;
		if (dronPosZ < 35.0f)  dronPosZ += 0.1f;

		if (dronPosX <= -18.0f and dronPosZ >= 35.0f) animDron = 3;
	}

	if (animDron == 3)
	{
		dronPosZ -= 0.15f;
		if (dronPosZ <= -30.0f) animDron = 4;
	}

	if (animDron == 4)
	{
		dronPosX += 0.15f;
		if (dronPosX >= 9.0f) animDron = 5;
	}

	if (animDron == 5)
	{
		dronPosZ += 0.15f;
		if (dronPosZ >= 25.0f) animDron = 6;
	}

	if (animDron == 6)
	{
		dronPosX -= 0.1f;
		dronPosZ -= 0.27f;

		if (dronPosZ <= -30.0f) animDron = 7;
	}

	if (animDron == 7)
	{
		if (dronPosX < 0.0f) dronPosX += 0.1f;
		if (dronPosZ < 0.0f) dronPosZ += 0.2f;

		if (dronPosX >= 0.0f and dronPosZ >= 0.0f)
		{
			dronPosX = 0.0f;
			dronPosZ = 0.0f;
			animDron = 8;
		}
	}

	if (animDron == 8)
	{
		dronPosY -= 0.02f;
		if (dronPosY <= 0.3f)
		{
			dronPosY = 0.3f;
			animDron = 0;
		}
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void CaminarHumano()
{
	if (!step)
	{
		rotBrazos += 0.5f;
		rotPiernas += 0.5f;

		if (rotBrazos > 15.0f) step = true;
	}
	else
	{
		rotBrazos -= 0.5f;
		rotPiernas -= 0.5f;

		if (rotBrazos < -15.0f) step = false;
	}
}

void MoverMotor()
{
	rotMotor += 2.0f;
	if (rotMotor > 10000.0f) rotMotor = 0;
}