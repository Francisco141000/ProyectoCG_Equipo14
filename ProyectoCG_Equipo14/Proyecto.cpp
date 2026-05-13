// Proyecto final
// 316118732
// Fecha de entrega: 13 de mayo de 2026

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
//#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void Animation();

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

//float vertices[] = {
//	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//
//	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//
//	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//
//		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//
//	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//
//	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
//};

//glm::vec3 Light1 = glm::vec3(0);

//Anim
float rotBall = 0.0f;
float rotDog = 0.0f;
int dogAnim = 0;
float LeftFLegs = 0.0f;
float RightFLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;
float vuelta = 0.0f;

// Anim Camioneta
float carrPosX, carrPosY, carrPosZ;
float acelerar = 0.0f;
float girarLlanta = 0.0f;
float girarCarr = 0.0f;
float traslacionStandCamioneta = -10.0f;
bool animCamioneta = false;

// Animacion Stands Básicos
int anim_Stands = 0;
float traslacion_StandBasico = -5.0f;
float rotacion_StandBasico = 0.0f;
float escala_StandBasico = 0.01f;

// Animación Stands Opcionales
float traslacion_StandOpcional = -10.0f;
float rotacon_StandOpcional = 0.0f;
float escala_StandOpcional = 0.01f;

//KeyFrames
float dogPosX, dogPosY, dogPosZ;
#define MAX_FRAMES 400
int i_max_steps = 190;
int i_curr_steps = 0;



typedef struct _frame {
	float rotDog;
	float rotDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;
	float head;
	float headInc;
	float LeftFLegs;
	float LeftFLegsInc;
	float RightFLegs;
	float RightFLegsInc;
	float RLegs;
	float RLegsInc;
	float tail;
	float tailInc;
	float vuelta;
	float vueltaInc;

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

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;
	KeyFrame[FrameIndex].rotDog = rotDog;
	KeyFrame[FrameIndex].head = head;
	KeyFrame[FrameIndex].LeftFLegs = LeftFLegs;
	KeyFrame[FrameIndex].RightFLegs = RightFLegs;
	KeyFrame[FrameIndex].RLegs = RLegs;
	KeyFrame[FrameIndex].tail = tail;
	KeyFrame[FrameIndex].vuelta = vuelta;

	KeyFrame[FrameIndex].carrPosX = carrPosX;
	KeyFrame[FrameIndex].carrPosY = carrPosY;
	KeyFrame[FrameIndex].carrPosZ = carrPosZ;
	KeyFrame[FrameIndex].acelerar = acelerar;
	KeyFrame[FrameIndex].girarLlanta = girarLlanta;
	KeyFrame[FrameIndex].girarCarr = girarCarr;

	ofstream MyFile("animacion.txt", std::ios::app);

	MyFile << dogPosX << " "
		<< dogPosY << " "
		<< dogPosZ << " "
		<< rotDog << " "
		<< head << " "
		<< LeftFLegs << " "
		<< RightFLegs << " "
		<< RLegs << " "
		<< tail << " "
		<< vuelta << " "

		<< carrPosX << " "
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
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;
	rotDog = KeyFrame[0].rotDog;
	head = KeyFrame[0].head;
	LeftFLegs = KeyFrame[0].LeftFLegs;
	RightFLegs = KeyFrame[0].RightFLegs;
	RLegs = KeyFrame[0].RLegs;
	tail = KeyFrame[0].tail;
	vuelta = KeyFrame[0].vuelta;

	carrPosX = KeyFrame[0].carrPosX;
	carrPosY = KeyFrame[0].carrPosY;
	carrPosZ = KeyFrame[0].carrPosZ;
	acelerar = KeyFrame[0].acelerar;
	girarLlanta = KeyFrame[0].girarLlanta;
	girarCarr = KeyFrame[0].girarCarr;
}
void interpolation(void)
{
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;
	KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
	KeyFrame[playIndex].LeftFLegsInc = (KeyFrame[playIndex + 1].LeftFLegs - KeyFrame[playIndex].LeftFLegs) / i_max_steps;
	KeyFrame[playIndex].RightFLegsInc = (KeyFrame[playIndex + 1].RightFLegs - KeyFrame[playIndex].RightFLegs) / i_max_steps;
	KeyFrame[playIndex].RLegsInc = (KeyFrame[playIndex + 1].RLegs - KeyFrame[playIndex].RLegs) / i_max_steps;
	KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;
	KeyFrame[playIndex].vueltaInc = (KeyFrame[playIndex + 1].vuelta - KeyFrame[playIndex].vuelta) / i_max_steps;

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
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

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

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	// Carga Modelo RedDog
	Model RedDog((char*)"Models/RedDog/RedDog.obj");

	// Carga Modelo Perro Separado
	Model DogBody((char*)"Models/RedDogSeparated/DogBody.obj");
	Model HeadDog((char*)"Models/RedDogSeparated/HeadDog.obj");
	Model DogTail((char*)"Models/RedDogSeparated/TailDog.obj");
	Model F_RightLeg((char*)"Models/RedDogSeparated/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/RedDogSeparated/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/RedDogSeparated/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/RedDogSeparated/B_LeftLegDog.obj");

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


	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dogPosX = 0;
		KeyFrame[i].dogPosY = 0;
		KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].head = 0;
		KeyFrame[i].headInc = 0;
		KeyFrame[i].LeftFLegs = 0;
		KeyFrame[i].LeftFLegsInc = 0;
		KeyFrame[i].RightFLegs = 0;
		KeyFrame[i].RightFLegsInc = 0;
		KeyFrame[i].RLegs = 0;
		KeyFrame[i].RLegsInc = 0;
		KeyFrame[i].tail = 0;
		KeyFrame[i].tailInc = 0;
		KeyFrame[i].vuelta = 0;
		KeyFrame[i].vueltaInc = 0;

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
		>> KeyFrame[i].dogPosX
		>> KeyFrame[i].dogPosY
		>> KeyFrame[i].dogPosZ
		>> KeyFrame[i].rotDog
		>> KeyFrame[i].head
		>> KeyFrame[i].LeftFLegs
		>> KeyFrame[i].RightFLegs
		>> KeyFrame[i].RLegs
		>> KeyFrame[i].tail
		>> KeyFrame[i].vuelta
		>> KeyFrame[i].carrPosX
		>> KeyFrame[i].carrPosY
		>> KeyFrame[i].carrPosZ
		>> KeyFrame[i].acelerar
		>> KeyFrame[i].girarLlanta
		>> KeyFrame[i].girarCarr
		
		
	
		)
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

	// First, set the container's VAO (and VBO)
	//GLuint VBO, VAO, EBO;
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/*glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// Position attribute
	/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);*/

	// normal attribute
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/

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

		// Caso 1 (Claro)
		/*glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 0.0f, -1.0f, -0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.7f, 0.7f, 0.7f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.8f, 0.8f, 0.8f);*/

		// Caso 2
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

		//// Point light 1
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.8f, 0.8f, 0.8f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.5f, 0.5f, 0.5f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.4f, 0.4f, 0.4f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.14f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.07f);

		//// Point light 2
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.8f, 0.8f, 0.8f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.5f, 0.5f, 0.5f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.4f, 0.4f, 0.4f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.14f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.07f);

		//// Point light 3
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.8f, 0.8f, 0.8f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.5f, 0.5f, 0.5f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.4f, 0.4f, 0.4f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.14f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.07f);

		//// Point light 4
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.8f, 0.8f, 0.8f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.5f, 0.5f, 0.5f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.4f, 0.4f, 0.4f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.14f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.07f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), 0.45f, 0.0f, 32.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 0.45f, 6.0f, 50.0f);

		// Caso 1 (Tonos blancos)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), animCamioneta * 0.1f, animCamioneta * 0.1f, animCamioneta * 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), animCamioneta * 1.0f, animCamioneta * 0.9f, animCamioneta * 0.9f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), animCamioneta * 1.0f, animCamioneta * 1.0f, animCamioneta * 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), animCamioneta * 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), animCamioneta * 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(animCamioneta * 12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(animCamioneta * 17.5f)));

		// Desactivada
		/*glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(0.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(0.0f)));*/

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


		// Despliegue Modelo Puente - Sin transparencia

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

		// Esto se borra después

		/*model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RedDog.Draw(lightingShader);*/

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


		// Despliegue Modelo Humano

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cuerpo.Draw(lightingShader);

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Brazo_Izquierdo.Draw(lightingShader);

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Brazo_Derecho.Draw(lightingShader);

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pierna_Izquierda.Draw(lightingShader);

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pierna_Derecha.Draw(lightingShader);

		// Despliegue Modelo Lampara

		/*for (int i = 0; i < 14; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(pointLightPositions[i]));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Lampara.Draw(lightingShader);
		}*/


		// Despliegue Modelo Perro Separado

		////Body
		//model = glm::mat4(1);
		//modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY + 0.4f, dogPosZ));
		//modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(1.0f, 0.0f, 0.0f));
		//modelTemp = model = glm::rotate(model, glm::radians(vuelta), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//DogBody.Draw(lightingShader);

		////Head
		//model = modelTemp;
		//model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		//model = glm::rotate(model, glm::radians(head), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//HeadDog.Draw(lightingShader);

		////Tail 
		//model = modelTemp;
		//model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		//model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//DogTail.Draw(lightingShader);

		////Front Left Leg
		//model = modelTemp;
		//model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		//model = glm::rotate(model, glm::radians(LeftFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//F_LeftLeg.Draw(lightingShader);

		////Front Right Leg
		//model = modelTemp;
		//model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		//model = glm::rotate(model, glm::radians(RightFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//F_RightLeg.Draw(lightingShader);

		////Back Left Leg
		//model = modelTemp;
		//model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
		//model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//B_LeftLeg.Draw(lightingShader);

		////Back Right Leg
		//model = modelTemp;
		//model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		//model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//B_RightLeg.Draw(lightingShader);


		if (anim_Stands == 0 and animCamioneta == true)
		{
			// Despliegue Modelo Camioneta
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

		if (anim_Stands == 1)
		{

			// Depsliegue Modelos Stand Básico
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


		// Also draw the lamp object, again binding the appropriate shader
		//lampShader.Use();

		//// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		//modelLoc = glGetUniformLocation(lampShader.Program, "model");
		//viewLoc = glGetUniformLocation(lampShader.Program, "view");
		//projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//// Set matrices
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//model = glm::mat4(1);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//// Draw the light object (using light's vertex attributes)
		//for (GLuint i = 0; i < 14; i++)
		//{
		//	model = glm::mat4(1);
		//	model = glm::translate(model, pointLightPositions[i]);
		//	model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glBindVertexArray(VAO);
		//	//glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
		//glBindVertexArray(0);

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

	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);*/
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
	////Dog Controls

	//if (keys[GLFW_KEY_1])
	//{
	//	rotDog -= 0.01f;
	//	dogPosY -= 0.00001f;
	//	LeftFLegs += 0.001f;
	//	RightFLegs += 0.001f;
	//	RLegs -= 0.01f;
	//	head += 0.01f;
	//}

	//if (keys[GLFW_KEY_2])
	//{
	//	rotDog += 0.01f;
	//	dogPosY += 0.00001f;
	//	LeftFLegs -= 0.001f;
	//	RightFLegs -= 0.001f;
	//	RLegs += 0.01f;
	//	head -= 0.01f;
	//}

	//if (keys[GLFW_KEY_3])
	//{
	//	dogPosY -= 0.0001f;
	//}

	//if (keys[GLFW_KEY_4])
	//{
	//	LeftFLegs -= 0.065f;
	//	RightFLegs -= 0.065f;
	//	RLegs += 0.065f;
	//}

	//if (keys[GLFW_KEY_5])
	//{
	//	vuelta += 0.1f;
	//	dogPosX -= 0.0004f;
	//}

	//if (keys[GLFW_KEY_6])
	//{
	//	dogPosY += 0.0001f;
	//}

	//if (keys[GLFW_KEY_7])
	//{
	//	LeftFLegs += 0.065f;
	//	RightFLegs += 0.065f;
	//	RLegs -= 0.065f;
	//}

	//if (keys[GLFW_KEY_8])
	//{
	//	RightFLegs -= 0.065f;;
	//}

	//if (keys[GLFW_KEY_9])
	//{
	//	RightFLegs += 0.065f;;
	//}

	//if (keys[GLFW_KEY_O])
	//{
	//	tail += 1.0f;
	//}

	//if (keys[GLFW_KEY_P])
	//{
	//	tail -= 1.0f;
	//}

	//if (keys[GLFW_KEY_H])
	//{
	//	dogPosZ += 0.01;
	//}

	//if (keys[GLFW_KEY_Y])
	//{
	//	dogPosZ -= 0.01;
	//}

	//if (keys[GLFW_KEY_G])
	//{
	//	dogPosX -= 0.01;
	//}

	//if (keys[GLFW_KEY_J])
	//{
	//	dogPosX += 0.01;
	//}

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

	//if (keys[GLFW_KEY_T])
	//{
	//	pointLightPositions[0].x += 0.01f;
	//}
	//if (keys[GLFW_KEY_G])
	//{
	//	pointLightPositions[0].x -= 0.01f;
	//}

	//if (keys[GLFW_KEY_Y])
	//{
	//	pointLightPositions[0].y += 0.01f;
	//}

	//if (keys[GLFW_KEY_H])
	//{
	//	pointLightPositions[0].y -= 0.01f;
	//}
	//if (keys[GLFW_KEY_U])
	//{
	//	pointLightPositions[0].z -= 0.1f;
	//}
	//if (keys[GLFW_KEY_J])
	//{
	//	pointLightPositions[0].z += 0.01f;
	//}
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
		/*printf("\nPosX: %f", camera.GetPosition().x);
		printf("\nPosZ: %f", camera.GetPosition().z);*/
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
	//if (keys[GLFW_KEY_SPACE])
	//{
	//	active = !active;
	//	if (active)
	//	{
	//		Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
	//	}
	//	else
	//	{
	//		Light1 = glm::vec3(0); //Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
	//	}
	//}
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
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;
			rotDog += KeyFrame[playIndex].rotDogInc;
			head += KeyFrame[playIndex].headInc;
			LeftFLegs += KeyFrame[playIndex].LeftFLegsInc;
			RightFLegs += KeyFrame[playIndex].RightFLegsInc;
			RLegs += KeyFrame[playIndex].RLegsInc;
			tail += KeyFrame[playIndex].tailInc;
			vuelta += KeyFrame[playIndex].vueltaInc;

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

	if (animCamioneta)
	{
		if (traslacionStandCamioneta < 0)
		{
			traslacionStandCamioneta += 0.1;
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