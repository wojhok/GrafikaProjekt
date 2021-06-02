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

std::vector<GLuint> texRoom;
std::vector<GLuint> texPainting;
std::vector<GLuint> paint1;
std::vector<GLuint> paint2;
std::vector<GLuint> paint3;
std::vector<GLuint> paint4;
std::vector<GLuint> paint5;
std::vector<GLuint> paint6;
std::vector<GLuint> paint7;
std::vector<GLuint> paint8;
std::vector<GLuint> paint9;
std::vector<GLuint> paint10;
std::vector<GLuint> paint11;
std::vector<GLuint> paint12;
std::vector<GLuint> paint13;
std::vector<GLuint> paint14;
std::vector<GLuint> paint15;






//Model modelObraz = Model(string("fbxPainting.fbx"),1.0f);

GLuint tex0;
GLuint tex1;
GLuint texWalls;

glm::vec3 positionOfCubesArr[] = {glm::vec3(0.0f,0.0f,0.0f),
								  glm::vec3(-5.0f,-5.0f,0.0f),
								  glm::vec3(4.0f,0.0f,0.0f),
								  glm::vec3(0.0f,4.0f,5.0f),
								  glm::vec3(2.0f,5.0f,-2.0f),
								  glm::vec3(2.0f,-4.0f,4.0f)}; // Tablica przechowuj¹ca po³o¿enia kolejnych szeœcianów


//kamera
Camera camera = Camera(glm::vec3(5.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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


GLuint readTexturePNG(const char* filename, int choice) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);

	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	if (choice == 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else if (choice == 2) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}
	else if (choice == 3) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}


GLuint readTextureJPG(const char* filename, int choice) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	if (choice == 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else if (choice == 2) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}
	else if (choice == 3) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
	
	//************Tutaj umieszczaj kod, który nale¿y wykonaæ raz, na pocz¹tku programu************
	//glEnable(GL_FRAMEBUFFER_SRGB);
	texRoom.push_back(readTextureJPG("sufit.jpg",2));
	texRoom.push_back(readTextureJPG("floor.jpg",2));
	for (int i = 0; i < 5; i++)
	{
		texRoom.push_back(readTextureJPG("wall.jpg",2));
	}
	texWalls = readTextureJPG("wall.jpg",2);
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, key_callback);
	
	texPainting.push_back(readTexturePNG("wood.png",1));
	texPainting.push_back(readTextureJPG("painting.png",2));
	texPainting.push_back(readTexturePNG("wood.png",1));

	paint1.push_back(readTexturePNG("wood.png", 1));
	paint1.push_back(readTextureJPG("obraz4.jpg", 2));
	paint1.push_back(readTexturePNG("wood.png", 1));

	paint2.push_back(readTexturePNG("wood.png", 1));
	paint2.push_back(readTextureJPG("obraz5.jpg", 2));
	paint2.push_back(readTexturePNG("wood.png", 1));

	paint3.push_back(readTexturePNG("wood.png", 1));
	paint3.push_back(readTexturePNG("kostki.png", 2));
	paint3.push_back(readTexturePNG("wood.png", 1));

	paint4.push_back(readTexturePNG("wood.png", 1));
	paint4.push_back(readTexturePNG("test.png", 2));
	paint4.push_back(readTexturePNG("wood.png", 1));

	paint5.push_back(readTexturePNG("wood.png", 1));
	paint5.push_back(readTexturePNG("moon.png", 2));
	paint5.push_back(readTexturePNG("wood.png", 1));

	paint6.push_back(readTexturePNG("wood.png", 1));
	paint6.push_back(readTextureJPG("chiny.jpg", 2));
	paint6.push_back(readTexturePNG("wood.png", 1));

	paint7.push_back(readTexturePNG("wood.png", 1));
	paint7.push_back(readTextureJPG("chiny2.jpg", 2));
	paint7.push_back(readTexturePNG("wood.png", 1));

	paint8.push_back(readTexturePNG("wood.png", 1));
	paint8.push_back(readTextureJPG("zdj1.jpg", 2));
	paint8.push_back(readTexturePNG("wood.png", 1));

	paint9.push_back(readTexturePNG("wood.png", 1));
	paint9.push_back(readTextureJPG("zdj2.jpg", 2));
	paint9.push_back(readTexturePNG("wood.png", 1));

	paint10.push_back(readTexturePNG("wood.png", 1));
	paint10.push_back(readTextureJPG("zdj3.jpg", 2));
	paint10.push_back(readTexturePNG("wood.png", 1));

	paint11.push_back(readTexturePNG("wood.png", 1));
	paint11.push_back(readTextureJPG("zdj5.jpg", 2));
	paint11.push_back(readTexturePNG("wood.png", 1));

	paint12.push_back(readTexturePNG("wood.png", 1));
	paint12.push_back(readTextureJPG("zdj6.jpg", 2));
	paint12.push_back(readTexturePNG("wood.png", 1));

	paint13.push_back(readTexturePNG("wood.png", 1));
	paint13.push_back(readTexturePNG("zdj7.png", 2));
	paint13.push_back(readTexturePNG("wood.png", 1));

	paint14.push_back(readTexturePNG("wood.png", 1));
	paint14.push_back(readTexturePNG("zdj8.png", 2));
	paint14.push_back(readTexturePNG("wood.png", 1));

	paint15.push_back(readTexturePNG("wood.png", 1));
	paint15.push_back(readTextureJPG("zdj9.jpg", 2));
	paint15.push_back(readTexturePNG("wood.png", 1));
	
	sp = new ShaderProgram("VertexShader.glsl", NULL, "FragmenShader.glsl");
	//sp1 = new ShaderProgram("ModelVS.glsl", NULL, "ModelFS.glsl");
}


//Zwolnienie zasobów zajêtych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który nale¿y wykonaæ po zakoñczeniu pêtli g³ównej************

	delete sp;
}



void drawScene(GLFWwindow* window, Camera camera, Walls walls,Room room, std::vector<Model> paintings, std::vector<Model> humans, std::vector<Model> statues, std::vector<Model> lamps) {
	//************Tutaj umieszczaj kod rysuj¹cy obraz******************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::vec4 lp = glm::vec4(0, 3, -5, 1); // Ustalenie wspó³rzêdnyh Ÿród³a œwiata³a
	std::vector<glm::vec3> lightPositions = {
		glm::vec3(7.5, 4, -7.5),
		glm::vec3(-7.5, 4,- 7.5),
		glm::vec3(7.5, 4, 7.5),
		glm::vec3(-7.5, 4, 7.5),
		camera.cameraPos
	};
	std::vector<glm::vec3> lightColors = {
		glm::vec3(1,0,0),
		glm::vec3(0,1,0),
		glm::vec3(0,0,1),
		glm::vec3(1,1,1),
		glm::vec3(1,1,1)
	};
	for (int i = 0; i < 5; i++) {
		string number = to_string(i);
		//glUniform3f(sp->u(("lightPositions["+ number+"]").c_str()), lightPositions[i].x, lightPositions[i].y, lightPositions[i].z);
		glUniform3fv(sp->u(("lightColors[" + number + "]").c_str()), 1, &lightColors[i][0]);
		
	}
	glUniform3fv(sp->u("lightPositions"), lightPositions.size(), &lightPositions[0][0] );
	//glUniform3fv(sp->u("lightPositions"), 1, glm::value_ptr(camera.cameraPos));
	//glUniform3fv(sp->u("lightPosition"), 1, glm::value_ptr(camera.cameraPos));
	glUniform3fv(sp->u("viewPosition"), 1, glm::value_ptr(camera.cameraPos));
	glm::mat4 V = camera.cameraMatrix();

	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.2f, 50.0f);

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	glm::mat4 M = glm::mat4(1.0f);
	sp->use();//Aktywacja programu cieniuj¹cego	
	
	room.drawRoom();
	walls.drawWalls();
	for (int i = 0; i < humans.size(); i++)
	{
		humans[i].drawModel();
	}
	for (int i = 0; i < 4; i++)
	{
		statues[i].drawModel();
	}
	for (int i = 0; i < paintings.size(); i++)
	{
		paintings[i].drawModel();	
	}
	for (int i = 0; i < lamps.size(); i++)
	{
		lamps[i].drawModel();
	}

	
		
	
	glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("texCoord0"));
	glDisableVertexAttribArray(sp->a("normal"));


	glfwSwapBuffers(window); //Przerzuæ tylny bufor na przedni

}

void calcDirHumans(std::vector<float>  &dirHumans,std::vector<Model> const& humans, Room const& room)
{
	if (humans[0].translate.x >= -1.0f - 0.5f)
	{
		dirHumans[0] = -1;
	}
	else if (humans[0].translate.x <= -room.roomWidth + 0.7f)
	{
		dirHumans[0] = 1;
	}
	if (humans[1].translate.z >= room.roomWidth - 0.7f)
	{
		dirHumans[1] = -1;
	}
	else if (humans[1].translate.z <= -room.roomWidth + 0.7f)
	{
		dirHumans[1] = 1;
	}
	if (humans[2].translate.z >= room.roomWidth - 0.7f)
	{
		dirHumans[2] = -1;
	}
	else if (humans[2].translate.z <= 3.0f)
	{
		dirHumans[2] = 1;
	}
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
	glm::mat4 M = glm::mat4(1.0f);
	Room room = Room(M, 7.0f, 15.f, sp, texRoom);
	Walls walls = Walls(M, sp, texWalls,room.roomWidth,room.roomHeight);
	std::vector<Model> paintings = { Model("fbxPainting.fbx",0.1f,texPainting,room.matricies[2],sp,glm::vec3(room.roomWidth / 2.0,0.0f,0.05f),180.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint15,room.matricies[2],sp,glm::vec3(-room.roomWidth / 2.0,0.0f,0.05f),180.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint1,room.matricies[3],sp,glm::vec3(-room.roomWidth / 2.0,0.0f,0.05f),180.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint2,room.matricies[3],sp,glm::vec3(room.roomWidth / 2.0,0.0f,0.05f),180.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint3,room.matricies[4],sp,glm::vec3(room.roomWidth / 2.0,0.0f,0.05f),180.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint4,room.matricies[4],sp,glm::vec3(-room.roomWidth / 2.0,0.0f,0.05f),180.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint5,room.matricies[5],sp,glm::vec3(room.roomWidth / 2.0,0.0f,0.05f),180.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint6,room.matricies[5],sp,glm::vec3(-room.roomWidth / 2.0,0.0f,0.05f),180.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint7,walls.matricies[0],sp,glm::vec3(0.0f,0.0f,1.05f),180.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint8,walls.matricies[0],sp,glm::vec3(0.0f,0.0f,-1.05f),0.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint9,walls.matricies[1],sp,glm::vec3(0.0f,0.0f,1.05f),180.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint10,walls.matricies[1],sp,glm::vec3(0.0f,0.0f,-1.05f),0.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint11,walls.matricies[2],sp,glm::vec3(1.05f,0.0f,0.0f),-90.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint12,walls.matricies[2],sp,glm::vec3(-1.05f,0.0f,0.0f),90.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint13,walls.matricies[3],sp,glm::vec3(1.05f,0.0f,0.0f),-90.0f, glm::vec3(0.0f,1.0f,0.0f)),
	Model("fbxPainting.fbx",0.1f,paint14,walls.matricies[3],sp,glm::vec3(-1.05f,0.0f,0.0f),90.0f, glm::vec3(0.0f,1.0f,0.0f)),
	
	};
	std::vector<GLuint> texStatue1 = { readTexturePNG("statue1.png", 1) };
	std::vector<GLuint> texStatue2 = { readTextureJPG("statue2.jpg",1) };
	std::vector<GLuint> texHuman = { readTexturePNG("jess.png", 1) };
	std::vector<Model> humans = {
		Model("human1.obj", 0.002f, texHuman, M, sp, glm::vec3(-6.0f, -2.0f, -6.0f), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
		Model("human1.obj", 0.002f, texHuman, M, sp, glm::vec3(13.0f, -2.0f, -13.0f), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
		Model("human1.obj", 0.002f, texHuman, M, sp, glm::vec3(-3.0f, -2.0f, 3.0f), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
	};
	std::vector <float> dirHumans = { 1.0f,1.0f,1.0f,1.0f };
	vector<Model> statues = {
	Model("Statue1.fbx",7.0f,texStatue2,M,sp,vec3(2.0f,-2.0f,-2.0f),180.0f, glm::vec3(0.0f,1.0f,1.0f)),
	Model("Statue2.obj",0.01f,texStatue1,M,sp,vec3(2.0f,-2.0f,2.0f),0.0f, glm::vec3(0.0f,0.0f,1.0f)), 
	Model("Statue1.fbx",7.0f,texStatue2,M,sp,vec3(-2.0f,-2.0f,-2.0f),180.0f, glm::vec3(0.0f,1.0f,1.0f)),
	Model("Statue2.obj",0.01f,texStatue1,M,sp,vec3(-2.0f,-2.0f,2.0f),0.0f, glm::vec3(1.0f,0.0f,0.0f)), 
	};
	std::vector<GLuint> texLamp = { readTextureJPG("sufit.jpg", 1), readTexturePNG("whit.png", 1) };
	vector<Model> lamps = { 
		Model("lamp.obj", 0.01f, texLamp,M,sp,vec3(-7.5f, 4.5f, 7.5f),0.0f, glm::vec3(0.0f,0.0f,1.0f)),
		Model("lamp.obj", 0.01f, texLamp,M,sp,vec3(-7.5f, 4.5f, -7.5f),0.0f, glm::vec3(0.0f,0.0f,1.0f)),
		Model("lamp.obj", 0.01f, texLamp,M,sp,vec3(7.5f, 4.5f, 7.5f),0.0f, glm::vec3(0.0f,0.0f,1.0f)),
		Model("lamp.obj", 0.01f, texLamp,M,sp,vec3(7.5f, 4.5f, -7.5f),0.0f, glm::vec3(0.0f,0.0f,1.0f)),
	};


	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
	{
		calcDirHumans(dirHumans,humans,room);
		humans[0].translate.x += dirHumans[0]*0.01f;
		humans[1].translate.z += dirHumans[1] * 0.01f;
		humans[2].translate.x += dirHumans[2] * -0.01f;
		humans[2].translate.z += dirHumans[2] * 0.01f;
		camera.cameraCalculateNewPos(positionSpeedVertical, positionSpeedHorizontal, glfwGetTime());
		glfwSetTime(0); //Zeruj timer
		drawScene(window, camera, walls,room,paintings, humans,statues, lamps); //Wykonaj procedurê rysuj¹c¹
		glfwPollEvents(); //Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.
	}
	
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
	exit(EXIT_SUCCESS);
}