// Práctica 2
// García Hernández Jesús Francisco
// 316118732
// Fecha de entrega: 15 de febrero de 2026

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "García Hernández Jesús Francisco - Práctica 2. Dibujo de Primitivas en 2D", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers

	float vertices[] = {

		// Cabeza
		-0.25f,	0.7f,	0.0f,		0.0f,0.0f,0.0f,
		0.25f,	0.7f,	0.0f,		0.0f,0.0f,0.0f,
		0.25f,	0.2f,	0.0f,		0.0f,0.0f,0.0f,
		-0.25f,	0.2f,	0.0f,		0.0f,0.0f,0.0f,

		// Oreja izquierda
		-0.15f,	0.7f,	0.0f,		0.7f,0.6f,0.38f,
		-0.05f,	0.7f,	0.0f,		0.7f,0.6f,0.38f,
		-0.10f,	0.9f,	0.0f,		0.7f,0.6f,0.38f,

		// Oreja derecha
		0.05f,	0.7f,	0.0f,		0.7f,0.6f,0.38f,
		0.15f,	0.7f,	0.0f,		0.7f,0.6f,0.38f,
		0.1f,	0.9f,	0.0f,		0.7f,0.6f,0.38f,

		// Ojo izquierdo
		-0.15f,	0.6f,	0.0f,		1.0f,1.0f,1.0f,
		-0.05f,	0.6f,	0.0f,		1.0f,1.0f,1.0f,
		-0.05f,	0.4f,	0.0f,		1.0f,1.0f,1.0f,
		-0.15f,	0.4f,	0.0f,		1.0f,1.0f,1.0f,

		// Ojo dereccho
		0.05f,	0.6f,	0.0f,		1.0f,1.0f,1.0f,
		0.15f,	0.6f,	0.0f,		1.0f,1.0f,1.0f,
		0.15f,	0.4f,	0.0f,		1.0f,1.0f,1.0f,
		0.05f,	0.4f,	0.0f,		1.0f,1.0f,1.0f,

		// Nariz
		-0.05f,	0.35f,	0.0f,		0.9f,0.3f,0.6f,
		0.05f,	0.35f,	0.0f,		0.7f,0.1f,0.4f,
		0.05f,	0.3f,	0.0f,		0.9f,0.3f,0.6f,
		-0.05f,	0.3f,	0.0f,		0.7f,0.1f,0.4f,

		// Torzo
		0.15f,	0.4f,	0.0f,		0.43f,0.32f,0.2f,
		0.15f,	-0.3f,	0.0f,		0.43f,0.32f,0.2f,
		0.3f,	0.05f,	0.0f,		0.43f,0.32f,0.2f,

		-0.15f,	0.4f,	0.0f,		0.43f,0.32f,0.2f,
		-0.15f,	-0.3f,	0.0f,		0.43f,0.32f,0.2f,
		-0.3f,	0.05f,	0.0f,		0.43f,0.32f,0.2f,

		-0.15f,	0.4f,	0.0f,		0.0f,0.0f,0.0f,
		0.15f,	0.4f,	0.0f,		0.0f,0.0f,0.0f,
		0.15f,	-0.3f,	0.0f,		0.0f,0.0f,0.0f,
		-0.15f,	-0.3f,	0.0f,		0.0f,0.0f,0.0f,

		// Piernas
		
		0.15f,	0.0f,	0.0f,		0.35f,0.22f,0.16f,
		0.15f,	-0.7f,	0.0f,		0.35f,0.22f,0.16f,
		0.35f,	-0.35f,	0.0f,		0.35f,0.22f,0.16f,

		-0.15f,	0.0f,	0.0f,		0.35f,0.22f,0.16f,
		-0.15f,	-0.7f,	0.0f,		0.35f,0.22f,0.16f,
		-0.35f,	-0.35f,	0.0f,		0.35f,0.22f,0.16f,

		-0.15f,	0.0f,	0.0f,		0.0f,0.0f,0.0f,
		0.15f,	0.0f,	0.0f,		0.0f,0.0f,0.0f,
		0.15f,	-0.7f,	0.0f,		0.0f,0.0f,0.0f,
		-0.15f,	-0.7f,	0.0f,		0.0f,0.0f,0.0f,

		// Patas
		0.15f,	-0.7f,	0.0f,		1.0f,1.0f,1.0f,
		0.21f,	-0.6f,	0.0f,		1.0f,1.0f,1.0f,
		0.3f,	-0.7f,	0.0f,		1.0f,1.0f,1.0f,

		-0.15f,	-0.7f,	0.0f,		1.0f,1.0f,1.0f,
		-0.21f,	-0.6f,	0.0f,		1.0f,1.0f,1.0f,
		-0.3f,	-0.7f,	0.0f,		1.0f,1.0f,1.0f,

		// Cola
		-0.35f,	-0.35f,	0.0f,		0.0f,0.0f,0.0f,
		-0.35f,	0.05f,	0.0f,		0.0f,0.0f,0.0f,
		-0.6f,	0.2f,	0.0f,		0.0f,0.0f,0.0f,

		-0.35f,	0.05f,	0.0f,		1.0f,1.0f,1.0f,
		-0.6f,	0.2f,	0.0f,		1.0f,1.0f,1.0f,
		-0.4f,	0.4f,	0.0f,		1.0f,1.0f,1.0f,

		-0.6f,	0.2f,	0.0f,		0.0f,0.0f,0.0f,
		-0.4f,	0.4f,	0.0f,		0.0f,0.0f,0.0f,
		-0.8f,	0.4f,	0.0f,		0.0f,0.0f,0.0f,

		-0.6f,	0.2f,	0.0f,		1.0f,1.0f,1.0f,
		-0.8f,	0.4f,	0.0f,		1.0f,1.0f,1.0f,
		-0.95f,	0.2f,	0.0f,		1.0f,1.0f,1.0f,

		// Bigotes lado izquierdo
		-0.2f,	0.25f,	0.0f,		1.0f,1.0f,1.0f,
		-0.35f,	0.2f,	0.0f,		1.0f,1.0f,1.0f,
		-0.2f,	0.3f,	0.0f,		1.0f,1.0f,1.0f,
		-0.35f,	0.3f,	0.0f,		1.0f,1.0f,1.0f,
		-0.2f,	0.35f,	0.0f,		1.0f,1.0f,1.0f,
		-0.35f,	0.4f,	0.0f,		1.0f,1.0f,1.0f,

		// Bigotes lado derecho
		0.2f,	0.25f,	0.0f,		1.0f,1.0f,1.0f,
		0.35f,	0.2f,	0.0f,		1.0f,1.0f,1.0f,
		0.2f,	0.3f,	0.0f,		1.0f,1.0f,1.0f,
		0.35f,	0.3f,	0.0f,		1.0f,1.0f,1.0f,
		0.2f,	0.35f,	0.0f,		1.0f,1.0f,1.0f,
		0.35f,	0.4f,	0.0f,		1.0f,1.0f,1.0f,

		// Pupilas
		-0.1f,	0.5f,	0.0f,		0.4f,0.3f,0.2f,
		0.1f,	0.5f,	0.0f,		0.4f,0.3f,0.2f,

	};

	unsigned int indices[] = {  // note that we start from 0!
		0,1,2,
		0,2,3,
		4,5,6,
		7,8,9,
		10,11,12,
		10,12,13,
		14,15,16,
		14,16,17,
		18,19,20,
		18,20,21,
		22,23,24,
		25,26,27,
		28,29,30,
		28,30,31,
		32,33,34,
		35,36,37,
		38,39,40,
		38,40,41,
		42,43,44,
		45,46,47,
		48,49,50,
		51,52,53,
		54,55,56,
		57,58,59,
	};

	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        ourShader.Use();
        glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(42 * sizeof(GLuint)));			// Piernas
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(48 * sizeof(GLuint)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(30 * sizeof(GLuint)));			// Torzo
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(36 * sizeof(GLuint)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);									// Cabeza
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));			// Oreja izquierda
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(9 * sizeof(GLuint)));			// Oreja derecha
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(GLuint)));			// Ojo izquierdo
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(GLuint)));			// Ojo derecho
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(54 * sizeof(GLuint)));			// Patas
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(60 * sizeof(GLuint)));		// Cola
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24 * sizeof(GLuint)));			// Nariz
		glDrawArrays(GL_LINES, 60, 2);															// Bigotes lado izq.
		glDrawArrays(GL_LINES, 62, 2);
		glDrawArrays(GL_LINES, 64, 2);
		glDrawArrays(GL_LINES, 66, 2);															// Bigotes lado der.
		glDrawArrays(GL_LINES, 68, 2);
		glDrawArrays(GL_LINES, 70, 2);
		glPointSize(15);																		// Ojos (Pupilas)
		glDrawArrays(GL_POINTS,72,1);
		glDrawArrays(GL_POINTS, 73, 1);
		
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}