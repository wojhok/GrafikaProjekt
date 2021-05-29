#pragma once
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

class Camera
{
public:
	glm::vec3 cameraPos; // po³o¿enie kamery
	glm::vec3 cameraLook; // gdzie patrzymy
	glm::vec3 cameraUp;
	Camera(glm::vec3 cameraPos, glm::vec3 cameraLook, glm::vec3 cameraUp)
	{
		this->cameraPos = cameraPos;
		this->cameraLook = cameraLook;
		this->cameraUp = cameraUp;
	}
	glm::mat4 cameraMatrix()
	{
		return glm::lookAt(
			cameraPos,
			cameraPos + cameraLook,
			glm::vec3(0, 1.0f, 0)); //Wylicz macierz widoku
	}
	void cameraCalculateNewPos(float positionSpeedVertical, float positionSpeedHorizontal, double time)
	{
		this->cameraPos += float(positionSpeedHorizontal * time) * glm::vec3(cameraLook.z, 0, -cameraLook.x); // przesuwanie w prawo lub lew wraz z uwzglêdnieniem kieunku wzroku
		this->cameraPos += float(positionSpeedVertical * time) * glm::vec3(cameraLook.x, 0, cameraLook.z); // przesuwanie w przód lub w ty³ wraz z uwzglêdnieniem kieunku wzroku
	}
	

private:

};

