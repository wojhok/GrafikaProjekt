#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Model.h"
#include "shaderProgram.h"

ShaderProgram* sp;

using namespace glm;
using namespace std;

float speed_x = 0;
float speed_y = 0;
float positionSpeedVertical = 0;
float positionSpeedHorizontal = 0;

float* vertices = myCubeVertices;
float* normals = myCubeNormals;
float* texCoords = myCubeTexCoords;
float* colors = myCubeColors;
int vertexCount = myCubeVertexCount;

glm::vec3 positionOfCubesArr[] = {glm::vec3(0.0f,0.0f,0.0f),
								  glm::vec3(-4.0f,-5.0f,0.0f),
								  glm::vec3(4.0f,0.0f,0.0f),
								  glm::vec3(0.0f,4.0f,5.0f),
								  glm::vec3(2.0f,5.0f,-2.0f),
								  glm::vec3(2.0f,-4.0f,4.0f)}; // Tablica przechowuj�ca po�o�enia kolejnych sze�cian�w


//kamera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -5.0f); // po�o�enie kamery
glm::vec3 cameraLook = glm::vec3(0.0f, 0.0f, 2.0f); // gdzie patrzymy
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
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
		cameraLook = glm::normalize(front);
	}
	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) { //Obs�uga klawiszy
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed_x = -3.14 / 2;
		if (key == GLFW_KEY_RIGHT) speed_x = 3.14 / 2;
		if (key == GLFW_KEY_UP) speed_y = 3.14 / 2;
		if (key == GLFW_KEY_DOWN) speed_y = -3.14 / 2;
		if (key == GLFW_KEY_W) positionSpeedVertical = 3.14 / 2;
		if (key == GLFW_KEY_S) positionSpeedVertical = -3.14 / 2;
		if (key == GLFW_KEY_A) positionSpeedHorizontal = 3.14 / 2;
		if (key == GLFW_KEY_D) positionSpeedHorizontal = -3.14 / 2;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) speed_x = 0;
		if (key == GLFW_KEY_RIGHT) speed_x = 0;
		if (key == GLFW_KEY_UP) speed_y = 0;
		if (key == GLFW_KEY_DOWN) speed_y = 0;
		if (key == GLFW_KEY_W) positionSpeedVertical = 0;
		if (key == GLFW_KEY_S) positionSpeedVertical = 0;
		if (key == GLFW_KEY_A) positionSpeedHorizontal = 0;
		if (key == GLFW_KEY_D) positionSpeedHorizontal = 0;
	}
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	glViewport(0, 0, width, height);
}


void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, kt�ry nale�y wykona� raz, na pocz�tku programu************
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, key_callback);

	sp = new ShaderProgram("VertexShader.glsl", NULL, "FragmenShader.glsl");
}


//Zwolnienie zasob�w zaj�tych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, kt�ry nale�y wykona� po zako�czeniu p�tli g��wnej************

	delete sp;
}



void drawScene(GLFWwindow* window, float position_z,float position_x) {
	//************Tutaj umieszczaj kod rysuj�cy obraz******************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(
		cameraPos,
		cameraPos+cameraLook,
		//cameraPos + calcDir(angle_x),
		glm::vec3(0, 1.0f, 0)); //Wylicz macierz widoku

	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.2f, 50.0f);

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	glm::mat4 M = glm::mat4(1.0f);
	
	//M = glm::rotate(M, angle_y, glm::vec3(1.0f, 0.0f, 0.0f)); //obr�t wok� osi X uk�adu
	//M = glm::rotate(M, angle_x, glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz modelu

	sp->use();//Aktywacja programu cieniuj�cego
	for (int i = 0; i < 6; i++) //Rysowanie 6 sze�cian�w
	{
		glm::mat4 M1 = M;
		M1 = glm::translate(M, positionOfCubesArr[i]);
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M1));
		glEnableVertexAttribArray(sp->a("vertex"));  //W��cz przesy�anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Wska� tablic� z danymi dla atrybutu vertex
		glEnableVertexAttribArray(sp->a("color"));  //W��cz przesy�anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors); //Wska� tablic� z danymi dla atrybutu vertex
		glDrawArrays(GL_TRIANGLES, 0, vertexCount); //Narysuj obiekt
	}
	

	glDisableVertexAttribArray(sp->a("vertex"));  //Wy��cz przesy�anie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));		//Wy��cz przesy�anie danych do atrybutu color

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
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak d�ugo jak okno nie powinno zosta� zamkni�te
	{
		cameraPos += float(positionSpeedHorizontal * glfwGetTime()) * glm::vec3(cameraLook.z, 0, -cameraLook.x); // przesuwanie w prawo lub w lewo wraz z uwzgl�dnieniem kieunku wzroku
		cameraPos += float(positionSpeedVertical*glfwGetTime())*glm::vec3(cameraLook.x,0, cameraLook.z); // przesuwanie w prz�d lub w ty� wraz z uwzgl�dnieniem kieunku wzroku
		glfwSetTime(0); //Zeruj timer
		drawScene(window, position_z, position_x); //Wykonaj procedur� rysuj�c�
		glfwPollEvents(); //Wykonaj procedury callback w zalezno�ci od zdarze� jakie zasz�y.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usu� kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zaj�te przez GLFW
	exit(EXIT_SUCCESS);
}