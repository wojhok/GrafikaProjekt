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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Quad.h"


using namespace std;
using namespace glm
class Room
{
public:
	std::vector <Quad> quads;
	std::vector <glm::vec3> translates;
	std::vector <glm::vec3> rotates;
	std::vector <glm::vec3> scales;
	glm::mat4 M;
	ShaderProgram shaderProgram;
	Room()
	{
		for (int i = 0; i < 6; i++)
		{
			this->quads.push_back(Quad());
		}

	}

	Room()
	{
		this->vertices = vertices;
		this->norms = norms;
		this->textures = textures;
		this->indices = indices;
	}
private:

};