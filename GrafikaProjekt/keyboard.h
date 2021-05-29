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
#include "Camera.h"
class Keyboard
{
public:
	float positionSpeedVertical;
	float positionSpeedHorizontal;
	Keyboard(float positionSpeedVertical, float postionSpeedHorizontal)
	{
		this->positionSpeedHorizontal = positionSpeedHorizontal;
		this->positionSpeedVertical = positionSpeedVertical;
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

private:

};