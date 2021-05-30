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
#include "shaderProgram.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "lodepng.h"
using namespace glm;

//Klasa Mesh - jeden element wczytywanego obiektu np noga broñ itp itd


class Mesh {
public:
	// mesh dane
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> norms;
	std::vector<glm::vec2> textures;
	std::vector<unsigned int> indices;

	// konstruktor
	Mesh(std::vector<glm::vec4> vertices, std::vector<glm::vec4> norms, std::vector<glm::vec2> textures, std::vector<unsigned int> indices)
	{
		this->vertices = vertices;
		this->norms = norms;
		this->textures = textures;
		this->indices = indices;
	}

	

	
};
