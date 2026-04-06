// Práctica 8
// García Hernández Jesús Francisco
// 316118732
// Fecha de entrega: 29 de marzo de 2026

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 50.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Light attributes (position)
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 lightPos2(0.0f, 0.0f, 0.0f);

//float movelightPos = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;
bool OnOff = true;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Garcia Hernandez Jesus Francisco - Practica 8. Materiales e Iluminacion", nullptr, nullptr);

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
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

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

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    // Load models

    Model campfire((char*)"Models/CampFire/CampFire.obj");

    Model doghouse((char*)"Models/DogHouse/DogHouse.obj");

    Model fence_block((char*)"Models/Fence/Fence_block/Fence_block.obj");

    Model forest((char*)"Models/Forest/Forest.obj");

    Model house((char*)"Models/House/House.obj");

    Model lamp_rotated((char*)"Models/Lamp/Lamp_rotated/Lamp_rotated.obj");

    Model red_dog((char*)"Models/RedDog/RedDog.obj");

    Model moon((char*)"Models/Moon/Moon.obj");

    Model sun((char*)"Models/Sun/Sun.obj");


    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    //float vertices[] = {

    //    // Posición (x, y, z)       // Vector normal
    //    -0.5f, -0.5f, -0.5f,        0.0f,  0.0f, -1.0f,
    //     0.5f, -0.5f, -0.5f,        0.0f,  0.0f, -1.0f,
    //     0.5f,  0.5f, -0.5f,        0.0f,  0.0f, -1.0f,
    //     0.5f,  0.5f, -0.5f,        0.0f,  0.0f, -1.0f,
    //    -0.5f,  0.5f, -0.5f,        0.0f,  0.0f, -1.0f,
    //    -0.5f, -0.5f, -0.5f,        0.0f,  0.0f, -1.0f,

    //    -0.5f, -0.5f,  0.5f,        0.0f,  0.0f,  1.0f,
    //     0.5f, -0.5f,  0.5f,        0.0f,  0.0f,  1.0f,
    //     0.5f,  0.5f,  0.5f,        0.0f,  0.0f,  1.0f,
    //     0.5f,  0.5f,  0.5f,        0.0f,  0.0f,  1.0f,
    //    -0.5f,  0.5f,  0.5f,        0.0f,  0.0f,  1.0f,
    //    -0.5f, -0.5f,  0.5f,        0.0f,  0.0f,  1.0f,

    //    -0.5f,  0.5f,  0.5f,        -1.0f,  0.0f,  0.0f,
    //    -0.5f,  0.5f, -0.5f,        -1.0f,  0.0f,  0.0f,
    //    -0.5f, -0.5f, -0.5f,        -1.0f,  0.0f,  0.0f,
    //    -0.5f, -0.5f, -0.5f,        -1.0f,  0.0f,  0.0f,
    //    -0.5f, -0.5f,  0.5f,        -1.0f,  0.0f,  0.0f,
    //    -0.5f,  0.5f,  0.5f,        -1.0f,  0.0f,  0.0f,

    //     0.5f,  0.5f,  0.5f,        1.0f,  0.0f,  0.0f,
    //     0.5f,  0.5f, -0.5f,        1.0f,  0.0f,  0.0f,
    //     0.5f, -0.5f, -0.5f,        1.0f,  0.0f,  0.0f,
    //     0.5f, -0.5f, -0.5f,        1.0f,  0.0f,  0.0f,
    //     0.5f, -0.5f,  0.5f,        1.0f,  0.0f,  0.0f,
    //     0.5f,  0.5f,  0.5f,        1.0f,  0.0f,  0.0f,

    //    -0.5f, -0.5f, -0.5f,        0.0f, -1.0f,  0.0f,
    //     0.5f, -0.5f, -0.5f,        0.0f, -1.0f,  0.0f,
    //     0.5f, -0.5f,  0.5f,        0.0f, -1.0f,  0.0f,
    //     0.5f, -0.5f,  0.5f,        0.0f, -1.0f,  0.0f,
    //    -0.5f, -0.5f,  0.5f,        0.0f, -1.0f,  0.0f,
    //    -0.5f, -0.5f, -0.5f,        0.0f, -1.0f,  0.0f,

    //    -0.5f,  0.5f, -0.5f,        0.0f,  1.0f,  0.0f,
    //     0.5f,  0.5f, -0.5f,        0.0f,  1.0f,  0.0f,
    //     0.5f,  0.5f,  0.5f,        0.0f,  1.0f,  0.0f,
    //     0.5f,  0.5f,  0.5f,        0.0f,  1.0f,  0.0f,
    //    -0.5f,  0.5f,  0.5f,        0.0f,  1.0f,  0.0f,
    //    -0.5f,  0.5f, -0.5f,        0.0f,  1.0f,  0.0f
    //};

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    //glEnableVertexAttribArray(0);
    //// normal attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    // Load textures
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    //int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    //unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    /*image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);*/


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightPos.x = 20.0f * cos(rot);
        lightPos.y = 20.0f * sin(rot);

        lightPos2.x = 20.0f * cos(rot + 3.1415f);
        lightPos2.y = 20.0f * sin(rot + 3.1415f);

        // Configuración luces
        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint lightPosLoc2 = glGetUniformLocation(lightingShader.Program, "light2.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(lightPosLoc2, lightPos2.x, lightPos2.y, lightPos2.z);


        // Propiedades primera luz (Sol)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), OnOff * 0.45f, OnOff * 0.3f, OnOff * 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), OnOff * 1.0f, OnOff * 0.8f, OnOff * 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), OnOff * 1.0f, OnOff * 0.9f, OnOff * 0.6f);

        // Propiedades segunda luz (Luna)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), !OnOff * 0.12f, !OnOff * 0.15f, !OnOff * 0.28f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), !OnOff * 0.4f, !OnOff * 0.5f, !OnOff * 0.85f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), !OnOff * 0.3f, !OnOff * 0.35f, !OnOff * 0.6f);


        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.18f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.55f, 0.45f, 0.25f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.05f, 0.05f, 0.03f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 8.0f);


        // Draw the loaded model

        // Modelo house
        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        house.Draw(lightingShader);

        // Modelo perro
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.4f, 10.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        red_dog.Draw(lightingShader);

        // Modelo fence
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 11.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        fence_block.Draw(lightingShader);

        // Modelo lamp
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 13.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        lamp_rotated.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 13.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        lamp_rotated.Draw(lightingShader);

        // Modelo campfire
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, -4.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        campfire.Draw(lightingShader);

        // Modelo doghouse
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.0f, 0.0f, -5.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        doghouse.Draw(lightingShader);

        // Modelo forest
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        forest.Draw(lightingShader);

        glBindVertexArray(0);

        // Desplegar Sol / Luna
        shader.Use();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        if (OnOff == true)
        {
            // Sol / Primera Luz
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(3.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            sun.Draw(shader);
        }
        else
        {
            // Luna / Segunda Luz
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos2);
            model = glm::scale(model, glm::vec3(2.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            moon.Draw(shader);
        }


        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.001f;
    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
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

    if (keys[GLFW_KEY_O])
    {
        activanim = !activanim;
    }

    if (keys[GLFW_KEY_L])
    {
        OnOff = !OnOff;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
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