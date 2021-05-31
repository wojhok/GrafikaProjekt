#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "shaderProgram.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Quad.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>


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
	float roomHeight;
	float roomWidth;
	//ShaderProgram* sp;

	Room(mat4 M,float roomHeight, float roomWidth)
	{
		//this->sp = shaderProgram;
		this->roomHeight = roomHeight;
		this->roomWidth = roomWidth;
		this->M = M;
		for (int i = 0; i < 6; i++)
		{
			this->quads.push_back(Quad());
		}
		this->translates = { vec3(0.0f,roomHeight-2.0f,0.0f),
			vec3(0.0f,-2.0f,0.0f),
			vec3(roomWidth,0.0f,0.0f),
			vec3(-roomWidth,0.0f,0.0f),
			vec3(0.0f,0.0f,roomWidth),
			vec3(0.0f,0.0f,-roomWidth),
		};
		this->rotates = {
			{90.0f*2.0*M_PI/360.0, vec3(1.0f,0.0f,0.0f)},
		{-90.0f * 2.0 * M_PI / 360.0, vec3(1.0f,0.0f,0.0f)},
		{-90.0f * 2.0 * M_PI / 360.0, vec3(0.0f,1.0f,0.0f)},
		{90.0f * 2.0 * M_PI / 360.0, vec3(0.0f,1.0f,0.0f) },
		{ 180.0f * 2.0 * M_PI / 360.0, vec3(0.0f,1.0f,0.0f) },
		{ 0.0f, vec3(0.0f, 0.0f, 1.0f)},
		};
	}


		
private:

};