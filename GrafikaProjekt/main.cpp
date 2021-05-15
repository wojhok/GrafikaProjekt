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

float* vertices = myCubeVertices;
float* normals = myCubeNormals;
float* texCoords = myCubeTexCoords;
float* colors = myCubeColors;
int vertexCount = myCubeVertexCount;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed_x = -3.14 / 2;
		if (key == GLFW_KEY_RIGHT) speed_x = 3.14 / 2;
		if (key == GLFW_KEY_UP) speed_y = 3.14 / 2;
		if (key == GLFW_KEY_DOWN) speed_y = -3.14 / 2;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) speed_x = 0;
		if (key == GLFW_KEY_RIGHT) speed_x = 0;
		if (key == GLFW_KEY_UP) speed_y = 0;
		if (key == GLFW_KEY_DOWN) speed_y = 0;
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
	//************Tutaj umieszczaj kod, który nale¿y wykonaæ raz, na pocz¹tku programu************
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, key_callback);

	sp = new ShaderProgram("VertexShader.glsl", NULL, "FragmenShader.glsl");
}


//Zwolnienie zasobów zajêtych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który nale¿y wykonaæ po zakoñczeniu pêtli g³ównej************

	delete sp;
}



void drawScene(GLFWwindow* window, float angle_x, float angle_y) {
	//************Tutaj umieszczaj kod rysuj¹cy obraz******************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(
		glm::vec3(0, 0, -5),
		glm::vec3(0, 0, 0),
		glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku

	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.2f, 50.0f);

	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, angle_y, glm::vec3(1.0f, 0.0f, 0.0f)); //Wylicz macierz modelu
	M = glm::rotate(M, angle_x, glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz modelu

	sp->use();//Aktywacja programu cieniuj¹cego
	//Przeslij parametry programu cieniuj¹cego do karty graficznej
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Wska¿ tablicê z danymi dla atrybutu vertex

	glEnableVertexAttribArray(sp->a("color"));  //W³¹cz przesy³anie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors); //Wska¿ tablicê z danymi dla atrybutu vertex

	glDrawArrays(GL_TRIANGLES, 0, vertexCount); //Narysuj obiekt

	glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));		//Wy³¹cz przesy³anie danych do atrybutu color

	glfwSwapBuffers(window); //Przerzuæ tylny bufor na przedni

}

int main()
{
	GLFWwindow* window; //WskaŸnik na obiekt reprezentuj¹cy okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurê obs³ugi b³êdów

	if (!glfwInit()) { //Zainicjuj bibliotekê GLFW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Je¿eli okna nie uda³o siê utworzyæ, to zamknij program
	{
		fprintf(stderr, "Nie mo¿na utworzyæ okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje siê aktywny i polecenia OpenGL bêd¹ dotyczyæ w³aœnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekê GLEW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjuj¹ce
	glEnable(GL_DEPTH_TEST);
	glfwSetKeyCallback(window, key_callback);

	//G³ówna pêtla
	float angle_x = 0; //Aktualny k¹t obrotu obiektu
	float angle_y = 0; //Aktualny k¹t obrotu obiektu
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
	{
		angle_x += speed_x * glfwGetTime(); //Zwiêksz/zmniejsz k¹t obrotu na podstawie prêdkoœci i czasu jaki up³yna³ od poprzedniej klatki
		angle_y += speed_y * glfwGetTime(); //Zwiêksz/zmniejsz k¹t obrotu na podstawie prêdkoœci i czasu jaki up³yna³ od poprzedniej klatki
		glfwSetTime(0); //Zeruj timer
		drawScene(window, angle_x, angle_y); //Wykonaj procedurê rysuj¹c¹
		glfwPollEvents(); //Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
	exit(EXIT_SUCCESS);
}