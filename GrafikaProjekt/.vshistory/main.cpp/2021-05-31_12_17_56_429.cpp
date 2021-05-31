#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Model.h"
#include "Cube.h"
#include "shaderProgram.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "lodepng.h"
#include "Camera.h"
#include "keyboard.h"
#include "Quad.h"
#include "Room.h"
#include "Walls.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


ShaderProgram* sp;
ShaderProgram* sp1;

using namespace glm;
using namespace std;

float speed_x = 0;
float speed_y = 0;

float positionSpeedVertical = 0;
float positionSpeedHorizontal = 0;
Keyboard keyboard = Keyboard(positionSpeedVertical,positionSpeedHorizontal);

//Cube
std::vector<GLuint> texRoom;
std::vector<GLuint> texPainting;





//Model modelObraz = Model(string("fbxPainting.fbx"),1.0f);

GLuint tex0;
GLuint tex1;
GLuint texWalls;

glm::vec3 positionOfCubesArr[] = {glm::vec3(0.0f,0.0f,0.0f),
								  glm::vec3(-5.0f,-5.0f,0.0f),
								  glm::vec3(4.0f,0.0f,0.0f),
								  glm::vec3(0.0f,4.0f,5.0f),
								  glm::vec3(2.0f,5.0f,-2.0f),
								  glm::vec3(2.0f,-4.0f,4.0f)}; // Tablica przechowuj�ca po�o�enia kolejnych sze�cian�w


//kamera
Camera camera = Camera(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));

float yaw_ = 90.0f;
float pitch_ = 0.0f;
// stan myszy
bool firstMouse = true; // czy mysz klikni�ta
float lastX; 
float lastY;
bool leftButtonPressed = false;

void mouseButtonCallback(GLFWwindow* window, int button,int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		leftButtonPressed = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		leftButtonPressed = false;
		firstMouse = true;
	}
}
void cursor_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (leftButtonPressed) // Przesuni�cie kamery je�eli lewy przycisk myszy jest wci�ni�ty  
	{
		if (firstMouse)
		{
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}
		float xOffset = xPos - lastX;
		float yOffset = lastY - yPos;

		lastX = xPos;
		lastY = yPos;
		float sensitivity = 0.1f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;
		yaw_ += xOffset;
		pitch_ += yOffset;
		if (pitch_ > 89.0f)
		{
			pitch_ = 89.0f;
		}
		if (pitch_ < -89.0f)
		{
			pitch_ = -89.0f;
		}
		glm::vec3 front;
		front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)); //wyliczenie wp�rz�dnej X kamery
		front.y = sin(glm::radians(pitch_)); //wyliczenie wsp�rz�dnej Y kamery
		front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_)); //wyliczenie wsp�rz�dnej Z kamery
		camera.cameraLook = glm::normalize(front);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) { //Obs�uga klawiszy
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_W) positionSpeedVertical = 3.14 / 2;
		if (key == GLFW_KEY_S) positionSpeedVertical = -3.14 / 2;
		if (key == GLFW_KEY_A) positionSpeedHorizontal = 3.14 / 2;
		if (key == GLFW_KEY_D) positionSpeedHorizontal = -3.14 / 2;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_W) positionSpeedVertical = 0;
		if (key == GLFW_KEY_S) positionSpeedVertical = 0;
		if (key == GLFW_KEY_A) positionSpeedHorizontal = 0;
		if (key == GLFW_KEY_D) positionSpeedHorizontal = 0;
	}
}

GLuint readTexturePNG(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	std::vector<unsigned char> image;   
	unsigned width, height;   
	unsigned error = lodepng::decode(image, width, height, filename);

	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}


GLuint readTextureJPG(const char* filename) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);


	return texture;
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	glViewport(0, 0, width, height);
}


void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void initOpenGLProgram(GLFWwindow* window) {
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//************Tutaj umieszczaj kod, kt�ry nale�y wykona� raz, na pocz�tku programu************
	texRoom.push_back(readTextureJPG("ceil.jpg"));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	texRoom.push_back(readTextureJPG("floor.jpg"));
	for (int i = 0; i < 5; i++)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		texRoom.push_back(readTextureJPG("wall.jpg"));
	}
	texWalls = readTextureJPG("wall.jpg");
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, key_callback);
	
	texPainting.push_back(readTexturePNG("Black.PNG"));
	texPainting.push_back(readTextureJPG("painting.png"));
	texPainting.push_back(readTexturePNG("Black.PNG"));
	tex0 = readTextureJPG("painting.png");
	tex1 = readTexturePNG("bricks.png");
	sp = new ShaderProgram("VertexShader.glsl", NULL, "FragmenShader.glsl");
	//sp1 = new ShaderProgram("ModelVS.glsl", NULL, "ModelFS.glsl");
}


//Zwolnienie zasob�w zaj�tych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, kt�ry nale�y wykona� po zako�czeniu p�tli g��wnej************

	delete sp;
}



void drawScene(GLFWwindow* window, Camera camera, Walls walls,Room room, std::vector<Model> paintings) {
	//************Tutaj umieszczaj kod rysuj�cy obraz******************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::vec4 lp = glm::vec4(0, 3, -5, 1); // Ustalenie wsp�rz�dnyh �r�d�a �wiata�a
	glUniform3fv(sp->u("lightPosition"), 1, glm::value_ptr(camera.cameraPos));
	glUniform3fv(sp->u("viewPosition"), 1, glm::value_ptr(camera.cameraPos));
	glm::mat4 V = camera.cameraMatrix();

	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.2f, 50.0f);

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	glm::mat4 M = glm::mat4(1.0f);
	sp->use();//Aktywacja programu cieniuj�cego	
	
	room.drawRoom();
	walls.drawWalls();
	for (int i = 0; i < paintings.size(); i++)
	{
		paintings[i].drawModel();	
	}
		
	
	glDisableVertexAttribArray(sp->a("vertex"));  //Wy��cz przesy�anie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("texCoord0"));
	glDisableVertexAttribArray(sp->a("normal"));


	glfwSwapBuffers(window); //Przerzu� tylny bufor na przedni

}

int main()
{
	
	GLFWwindow* window; //Wska�nik na obiekt reprezentuj�cy okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedur� obs�ugi b��d�w

	if (!glfwInit()) { //Zainicjuj bibliotek� GLFW
		fprintf(stderr, "Nie mo�na zainicjowa� GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utw�rz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Je�eli okna nie uda�o si� utworzy�, to zamknij program
	{
		fprintf(stderr, "Nie mo�na utworzy� okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje si� aktywny i polecenia OpenGL b�d� dotyczy� w�a�nie jego.
	glfwSwapInterval(1); //Czekaj na 1 powr�t plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotek� GLEW
		fprintf(stderr, "Nie mo�na zainicjowa� GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjuj�ce
	glEnable(GL_DEPTH_TEST);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	//G��wna p�tla
	float position_z = 0; //Aktualna pozycja kamery
	float position_x = 0; // Aktualna pozycja kamery
	glm::mat4 M = glm::mat4(1.0f);
	Room room = Room(M, 7.0f, 15.f, sp, texRoom);
	Walls walls = Walls(M, sp, texWalls,room.roomWidth,room.roomHeight);
	std::vector<Model> paintings = { Model("fbxPainting.fbx",0.1f,texPainting,room.matricies[2],sp,glm::vec3(0,0,0),180, glm::vec3(0,1,0))
	
	};
	/*for (int i = 0; i < 4; i++)
	{
		paintings.push_back(Model("fbxPainting.fbx", 0.1f, texPainting, walls.matricies[i], sp, glm::vec3(0.0f, 0.0f, -1.0f)));
	}*/
	//Model painting = Model("fbxPainting.fbx", 0.1f, texPainting, walls.matricies[0], sp, glm::vec3(0.0f,0.0f,-1.0f));
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak d�ugo jak okno nie powinno zosta� zamkni�te
	{
		camera.cameraCalculateNewPos(positionSpeedVertical, positionSpeedHorizontal, glfwGetTime());
		glfwSetTime(0); //Zeruj timer
		drawScene(window, camera, walls,room,paintings); //Wykonaj procedur� rysuj�c�
		glfwPollEvents(); //Wykonaj procedury callback w zalezno�ci od zdarze� jakie zasz�y.
	}
	
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usu� kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zaj�te przez GLFW
	exit(EXIT_SUCCESS);
}