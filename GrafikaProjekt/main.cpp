#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, kt�ry nale�y wykona� raz, na pocz�tku programu************
}


//Zwolnienie zasob�w zaj�tych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, kt�ry nale�y wykona� po zako�czeniu p�tli g��wnej************
}

void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		//if (key == GLFW_KEY_LEFT) speed = -3.14;
		if (key == GLFW_KEY_RIGHT)
		{
			//speed = 3.14;
			
		}
	}
	if (action == GLFW_RELEASE) {
		//if (key == GLFW_KEY_LEFT) speed = 0;
		//if (key == GLFW_KEY_RIGHT)  speed = 0;
	}
}

void drawScene(GLFWwindow* window, float angle) {
	//************Tutaj umieszczaj kod rysuj�cy obraz******************
	glClearColor(0.15, 0.54, 0.45, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 V = glm::lookAt(
		glm::vec3(0.0f, 0.0f, -5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.2f, 50.0f);
	glm::vec3 translationB(1, 0, 0);
	glm::mat4 M = glm::translate(glm::mat4(1.0f), translationB);
	M = glm::rotate(M, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 translationA(-1, 0, 0);
	M = glm::translate(glm::mat4(1.0f), translationA);
	M = glm::rotate(M, angle, glm::vec3(0.0f, -1.2f, 0.0f));
	glfwSwapBuffers(window);

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

	//G��wna p�tla
	float angle = 0;
	glfwSetTime(0);
	while (!glfwWindowShouldClose(window)) //Tak d�ugo jak okno nie powinno zosta� zamkni�te
	{
		//angle += speed * glfwGetTime();
		glfwSetTime(0);
		drawScene(window, angle); //Wykonaj procedur� rysuj�c�
		glfwPollEvents(); //Wykonaj procedury callback w zalezno�ci od zdarze� jakie zasz�y.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usu� kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zaj�te przez GLFW
	exit(EXIT_SUCCESS);
}