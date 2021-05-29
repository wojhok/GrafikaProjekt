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


float* vertices = myCubeVertices;
float* normals = myCubeNormals;
float* texCoords = myCubeTexCoords;
float* colors = myCubeColors;
int vertexCount = myCubeVertexCount;

float* quadVerticies;

std::vector<glm::vec4> verts;
std::vector<glm::vec4> norms;
std::vector<glm::vec2> texCoords1;
std::vector<unsigned int> indices;

Model modelObraz = Model(string("fbxPainting.fbx"),1.0f);

GLuint tex0;
GLuint tex1;

glm::vec3 positionOfCubesArr[] = {glm::vec3(0.0f,0.0f,0.0f),
								  glm::vec3(-5.0f,-5.0f,0.0f),
								  glm::vec3(4.0f,0.0f,0.0f),
								  glm::vec3(0.0f,4.0f,5.0f),
								  glm::vec3(2.0f,5.0f,-2.0f),
								  glm::vec3(2.0f,-4.0f,4.0f)}; // Tablica przechowuj¹ca po³o¿enia kolejnych szeœcianów


//kamera
Camera camera = Camera(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));

float yaw_ = 90.0f;
float pitch_ = 0.0f;
// stan myszy
bool firstMouse = true; // czy mysz klikniêta
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
	if (leftButtonPressed) // Przesuniêcie kamery je¿eli lewy przycisk myszy jest wciœniêty  
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
		front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)); //wyliczenie wpó³rzêdnej X kamery
		front.y = sin(glm::radians(pitch_)); //wyliczenie wspó³rzêdnej Y kamery
		front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_)); //wyliczenie wspó³rzêdnej Z kamery
		camera.cameraLook = glm::normalize(front);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) { //Obs³uga klawiszy
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

//GLuint readTexture(const char* filename) {
//	GLuint tex;
//	glActiveTexture(GL_TEXTURE0);
//
//	std::vector<unsigned char> image;   
//	unsigned width, height;   
//	unsigned error = lodepng::decode(image, width, height, filename);
//
//	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
//	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
//	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
//		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	return tex;
//}


GLuint readTexture(const char* filename) {
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
	else
	{
		std::cout << "Failed to load texture" << std::endl;
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
	//************Tutaj umieszczaj kod, który nale¿y wykonaæ raz, na pocz¹tku programu************
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, key_callback);
	//loadModel(string("fbxPainting.fbx"));
	//mesh = Mesh(std::string("fbxPainting.fbx"));
	tex0 = readTexture("painting.png");
	//tex1 = readTexture("bricks.png");
	sp = new ShaderProgram("VertexShader.glsl", NULL, "FragmenShader.glsl");
	//sp1 = new ShaderProgram("ModelVS.glsl", NULL, "ModelFS.glsl");
}


//Zwolnienie zasobów zajêtych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który nale¿y wykonaæ po zakoñczeniu pêtli g³ównej************

	delete sp;
}



void drawScene(GLFWwindow* window, float position_z,float position_x, Camera camera) {
	//************Tutaj umieszczaj kod rysuj¹cy obraz******************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec4 lp = glm::vec4(0, 3, -5, 1); // Ustalenie wspó³rzêdnyh Ÿród³a œwiata³a
	glUniform3fv(sp->u("lightPosition"), 1, glm::value_ptr(camera.cameraPos));
	glUniform3fv(sp->u("viewPosition"), 1, glm::value_ptr(camera.cameraPos));
	glm::mat4 V = camera.cameraMatrix();

	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.2f, 50.0f);

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	glm::mat4 M = glm::mat4(1.0f);
	
	//M = glm::rotate(M, angle_y, glm::vec3(1.0f, 0.0f, 0.0f)); //obrót wokó³ osi X uk³adu
	//M = glm::rotate(M, angle_x, glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz modelu

	sp->use();//Aktywacja programu cieniuj¹cego
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
		M1 = glm::scale(M1, glm::vec3(0.1f,0.1f,0.1f));
		//M1 = glm::translate(M, glm::vec3(0.0f,0.0f,0.0f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M1));
		glEnableVertexAttribArray(sp->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, modelObraz.meshes[i].vertices.data()); //Wska¿ tablicê z danymi dla atrybutu vertex
		
		if (i == 1) {
			glEnableVertexAttribArray(sp->a("texCoord0"));  //W³¹cz przesy³anie danych do atrybutu vertex
			glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, modelObraz.meshes[i].textures.data());
		}
		//Wska¿ tablicê z danymi dla atrybutu vertex
		glEnableVertexAttribArray(sp->a("normal"));  //W³¹cz przesy³anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, modelObraz.meshes[i].norms.data()); //Wska¿ tablicê z danymi dla atrybutu vertex

		
		glUniform1i(sp->u("textureMap0"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex0);
		
		
		

		glDrawElements(GL_TRIANGLES, modelObraz.meshes[i].indices.size(), GL_UNSIGNED_INT, modelObraz.meshes[i].indices.data()); //Narysuj obiekt
		glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
		glDisableVertexAttribArray(sp->a("texCoord0"));
		glDisableVertexAttribArray(sp->a("normal"));
	}
		
	
	glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("texCoord0"));
	glDisableVertexAttribArray(sp->a("normal"));


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
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	//G³ówna pêtla
	float position_z = 0; //Aktualna pozycja kamery
	float position_x = 0; // Aktualna pozycja kamery
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
	{
		camera.cameraCalculateNewPos(positionSpeedVertical, positionSpeedHorizontal, glfwGetTime());
		glfwSetTime(0); //Zeruj timer
		drawScene(window, position_z, position_x, camera); //Wykonaj procedurê rysuj¹c¹
		glfwPollEvents(); //Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
	exit(EXIT_SUCCESS);
}