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
#include "keyboard.h"
#include "Quad.h"


using namespace std;
using namespace glm;

struct Rotate {
	float angle;
	vec3 axis;

};


class Room
{
public:
	std::vector <Quad> quads;
	std::vector <glm::vec3> translates;
	std::vector <Rotate> rotates;
	std::vector <glm::vec3> scales;
	glm::mat4 M;
	//ShaderProgram* sp;

	Room(mat4 M)
	{
		//this->sp = shaderProgram;
		this->M = M;
		for (int i = 0; i < 6; i++)
		{
			this->quads.push_back(Quad());
		}
		this->translates = { vec3(0.0f,5.0f,0.0f),
			vec3(0.0f,-15.0f,0.0f),
			vec3(15.0f,0.0f,0.0f),
			vec3(-15.0f,0.0f,0.0f),
			vec3(0.0f,0.0f,15.0f),
			vec3(0.0f,0.0f,-15.0f),
		};
		this->rotates = {
			{90.0f, vec3(0.0f,0.0f,1.0f)},
		{90.0f, vec3(0.0f,0.0f,1.0f)},
		{90.0f, vec3(0.0f,1.0f,0.0f)},
		{90.0f, vec3(0.0f,1.0f,0.0f) },
		{ 180.0f, vec3(0.0f,1.0f,0.0f) },
		{ 0.0f, vec3(0.0f, 0.0f, 1.0f)},
		};
	}


		
private:

};