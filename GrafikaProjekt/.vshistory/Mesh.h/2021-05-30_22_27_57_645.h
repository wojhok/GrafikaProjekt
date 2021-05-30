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

	void drawMesh(GLuint tex, glm::mat4 matrix)
	{
		matrix = glm::scale(matrix, glm::vec3(0.1f, 0.1f, 0.1f));
		//M1 = glm::translate(M, glm::vec3(0.0f,0.0f,0.0f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(matrix));
		glEnableVertexAttribArray(sp->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices.data()); //Wska¿ tablicê z danymi dla atrybutu vertex
		glEnableVertexAttribArray(sp->a("texCoord0"));  //W³¹cz przesy³anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, textures.data()); //testesttest
		
		//Wska¿ tablicê z danymi dla atrybutu vertex
		glEnableVertexAttribArray(sp->a("normal"));  //W³¹cz przesy³anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, norms.data()); //Wska¿ tablicê z danymi dla atrybutu vertex

		glUniform1i(sp->u("textureMap0"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data()); //Narysuj obiekt
		glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
		glDisableVertexAttribArray(sp->a("texCoord0"));
		glDisableVertexAttribArray(sp->a("normal"));
	}

	
};
