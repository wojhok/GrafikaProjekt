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
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "lodepng.h"

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

int liczba_obiektow = 0;
std::vector<glm::vec4> verts;
std::vector<glm::vec4> norms;
std::vector<glm::vec2> texCoords1;
std::vector<unsigned int> indices;

Mesh mesh = Mesh(string("jack.fbx"));

GLuint tex0;

glm::vec3 positionOfCubesArr[] = {glm::vec3(0.0f,0.0f,0.0f),
								  glm::vec3(-5.0f,-5.0f,0.0f),
								  glm::vec3(4.0f,0.0f,0.0f),
								  glm::vec3(0.0f,4.0f,5.0f),
								  glm::vec3(2.0f,5.0f,-2.0f),
								  glm::vec3(2.0f,-4.0f,4.0f)}; // Tablica przechowuj¹ca po³o¿enia kolejnych szeœcianów


//kamera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -5.0f); // po³o¿enie kamery
glm::vec3 cameraLook = glm::vec3(0.0f, 0.0f, 2.0f); // gdzie patrzymy
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw_ = 90.0f;
float pitch_ = 0.0f;
// stan myszy
bool firstMouse = true; // czy mysz klikniêta
float lastX; 
float lastY;
bool leftButtonPressed = false;



//void loadModel(std::string plik) {
//	using namespace std;
//	Assimp::Importer importer;
//	const aiScene* scene = importer.ReadFile(plik,
//		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
//	cout << importer.GetErrorString() << endl;
//
//	for (int k = 0; k < scene->mNumMeshes; k++)
//	{
//		aiMesh* mesh = scene->mMeshes[k];
//
//		for (int i = 0; i < mesh->mNumVertices; i++) {
//			aiVector3D vertex = mesh->mVertices[i]; //aiVector3D podobny do glm::vec3
//			verts.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));
//
//			aiVector3D normal = mesh->mNormals[i]; //Wektory znormalizowane
//			norms.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));
//
//			//liczba zdefiniowanych zestawów wsp. teksturowania (zestawów jest max 8)
//			//unsigned int liczba_zest = mesh->GetNumUVChannels();
//			//Liczba sk³adowych wsp. teksturowania dla 0 zestawu.
//			//unsigned int wymiar_wsp_tex = mesh->mNumUVComponents[0];
//			//0 to numer zestawu wspó³rzêdnych teksturowania
//
//
//			aiVector3D texCoord = mesh->mTextureCoords[0][i];
//			texCoords1.push_back(glm::vec2(texCoord.x, texCoord.y));
//			//x,y,z wykorzystywane jako u,v,w. 0 je¿eli tekstura ma mniej wymiarów
//
//		}
//
//		//dla ka¿dego wielok¹ta sk³adowego
//		for (int i = 0; i < mesh->mNumFaces; i++) {
//			aiFace& face = mesh->mFaces[i]; //face to jeden z wielok¹tów siatki
//
//			//dla ka¿dego indeksu->wierzcho³ka tworz¹cego wielok¹t
//			//dla aiProcess_Triangulate to zawsze bêdzie 3
//			for (int j = 0; j < face.mNumIndices; j++) {
//				//cout << face.mIndices[j] << " ";
//				indices.push_back(face.mIndices[j]);
//			}
//			//cout << endl;
//		}
//	}
//
//}



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
		cameraLook = glm::normalize(front);
	}
	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) { //Obs³uga klawiszy
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

	//Wczytanie do pamiêci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamiêci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
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
	//************Tutaj umieszczaj kod, który nale¿y wykonaæ raz, na pocz¹tku programu************
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, key_callback);
	//loadModel(string("fbxPainting.fbx"));
	//mesh = Mesh(std::string("fbxPainting.fbx"));
	tex0 = readTexture("bricks.png");
	sp = new ShaderProgram("VertexShader.glsl", NULL, "FragmenShader.glsl");
}


//Zwolnienie zasobów zajêtych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który nale¿y wykonaæ po zakoñczeniu pêtli g³ównej************

	delete sp;
}



void drawScene(GLFWwindow* window, float position_z,float position_x) {
	//************Tutaj umieszczaj kod rysuj¹cy obraz******************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec4 lp = glm::vec4(0, 20, -5, 1); // Ustalenie wspó³rzêdnyh Ÿród³a œwiata³a
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
	
	//M = glm::rotate(M, angle_y, glm::vec3(1.0f, 0.0f, 0.0f)); //obrót wokó³ osi X uk³adu
	//M = glm::rotate(M, angle_x, glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz modelu

	sp->use();//Aktywacja programu cieniuj¹cego
	//for (int i = 0; i < 6; i++) //Rysowanie 6 szeœcianów
	//{
	//	glm::mat4 M1 = M;
	//	M1 = glm::translate(M, positionOfCubesArr[i]);
	//	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M1));
	//	glEnableVertexAttribArray(sp->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
	//	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Wska¿ tablicê z danymi dla atrybutu vertex
	//	glEnableVertexAttribArray(sp->a("color"));  //W³¹cz przesy³anie danych do atrybutu color
	//	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors); //Wska¿ tablicê z danymi dla atrybutu color
	//	glEnableVertexAttribArray(sp->a("texCoord0"));  //W³¹cz przesy³anie danych do atrybutu texCoord0
	//	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords); //Wska¿ tablicê z danym dla atrybutu texCoords0
	//	glEnableVertexAttribArray(sp->a("normal"));  //W³¹cz przesy³anie danych do atrybutu normal do atrybutu normal
	//	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals);//Wska¿ tablicê z danymi dla atrybutu normal
	//	
	//	glUniform1i(sp->u("textureMap0"), 0);
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, tex0);

	//	glDrawArrays(GL_TRIANGLES, 0, vertexCount); //Narysuj obiekt
	//}
	
		glm::mat4 M1 = M;
		//M1 = glm::translate(M, glm::vec3(0.0f,0.0f,0.0f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M1));
		glEnableVertexAttribArray(sp->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, mesh.vertices.data()); //Wska¿ tablicê z danymi dla atrybutu vertex
		glEnableVertexAttribArray(sp->a("texCoord1"));  //W³¹cz przesy³anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("texCoord1"), 4, GL_FLOAT, false, 0, mesh.textures.data()); //Wska¿ tablicê z danymi dla atrybutu vertex
		glEnableVertexAttribArray(sp->a("normal"));  //W³¹cz przesy³anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, mesh.textures.data()); //Wska¿ tablicê z danymi dla atrybutu vertex


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex0);
		glUniform1i(sp->u("textureMap0"), 0);

		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, mesh.indices.data()); //Narysuj obiekt
	
	


	glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("texCoord1"));
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
		cameraPos += float(positionSpeedHorizontal * glfwGetTime()) * glm::vec3(cameraLook.z, 0, -cameraLook.x); // przesuwanie w prawo lub lew wraz z uwzglêdnieniem kieunku wzroku
		cameraPos += float(positionSpeedVertical*glfwGetTime())*glm::vec3(cameraLook.x,0, cameraLook.z); // przesuwanie w przód lub w ty³ wraz z uwzglêdnieniem kieunku wzroku
		glfwSetTime(0); //Zeruj timer
		drawScene(window, position_z, position_x); //Wykonaj procedurê rysuj¹c¹
		glfwPollEvents(); //Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
	exit(EXIT_SUCCESS);
}