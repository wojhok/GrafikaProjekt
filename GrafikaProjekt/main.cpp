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

ShaderProgram* sp;
ShaderProgram* sp1;

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

int liczba_obiektow = 0;
std::vector<glm::vec4> verts;
std::vector<glm::vec4> norms;
std::vector<glm::vec2> texCoords1;
std::vector<unsigned int> indices;

Model modelObraz = Model(string("fbxPainting.fbx"),0.11f);

GLuint tex0;
GLuint tex1;

glm::vec3 positionOfCubesArr[] = {glm::vec3(0.0f,0.0f,0.0f),
								  glm::vec3(-5.0f,-5.0f,0.0f),
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

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pami�ci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do kt�rych wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pami�ci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pami�ci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
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
	//loadModel(string("fbxPainting.fbx"));
	//mesh = Mesh(std::string("fbxPainting.fbx"));
	tex0 = readTexture("bricks.png");
	tex1 = readTexture("painting.png");
	sp = new ShaderProgram("VertexShader.glsl", NULL, "FragmenShader.glsl");
	//sp1 = new ShaderProgram("ModelVS.glsl", NULL, "ModelFS.glsl");
}


//Zwolnienie zasob�w zaj�tych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, kt�ry nale�y wykona� po zako�czeniu p�tli g��wnej************

	delete sp;
}



void drawScene(GLFWwindow* window, float position_z,float position_x) {
	//************Tutaj umieszczaj kod rysuj�cy obraz******************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec4 lp = glm::vec4(0, 3, -100, 1); // Ustalenie wsp�rz�dnyh �r�d�a �wiata�a
	glUniform4fv(sp->u("lp"), 1, glm::value_ptr(lp));
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
	//for (int i = 0; i < 6; i++) 
	//{
	//	glm::mat4 M1 = M;
	//	M1 = glm::translate(M, positionOfCubesArr[i]);
	//	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M1));
	//	glEnableVertexAttribArray(sp->a("vertex"));  
	//	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices); 
	//	glEnableVertexAttribArray(sp->a("color"));  
	//	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors); 
	//	glEnableVertexAttribArray(sp->a("texCoord0"));  
	//	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords); 
	//	glEnableVertexAttribArray(sp->a("normal"));  
	//	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals);
	//	
	//	glUniform1i(sp->u("textureMap0"), 0);
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, tex0);

	//	glDrawArrays(GL_TRIANGLES, 0, vertexCount); //Narysuj obiekt
	//}
	for (int i = 0; i < modelObraz.meshes.size(); i++)
	{
		glm::mat4 M1 = M;
		//M1 = glm::translate(M, glm::vec3(0.0f,0.0f,0.0f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M1));
		glEnableVertexAttribArray(sp->a("vertex"));  //W��cz przesy�anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, modelObraz.meshes[i].vertices.data()); //Wska� tablic� z danymi dla atrybutu vertex
		glEnableVertexAttribArray(sp->a("texCoord1"));  //W��cz przesy�anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("texCoord1"), 4, GL_FLOAT, false, 0, modelObraz.meshes[i].textures.data()); //Wska� tablic� z danymi dla atrybutu vertex
		glEnableVertexAttribArray(sp->a("normal"));  //W��cz przesy�anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, modelObraz.meshes[i].norms.data()); //Wska� tablic� z danymi dla atrybutu vertex


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex0);
		glUniform1i(sp->u("textureMap0"), 0);

		glDrawElements(GL_TRIANGLES, modelObraz.meshes[i].indices.size(), GL_UNSIGNED_INT, modelObraz.meshes[i].indices.data()); //Narysuj obiekt
	}
		
	
	


	glDisableVertexAttribArray(sp->a("vertex"));  //Wy��cz przesy�anie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("texCoord1"));
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
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak d�ugo jak okno nie powinno zosta� zamkni�te
	{
		cameraPos += float(positionSpeedHorizontal * glfwGetTime()) * glm::vec3(cameraLook.z, 0, -cameraLook.x); // przesuwanie w prawo lub lew wraz z uwzgl�dnieniem kieunku wzroku
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