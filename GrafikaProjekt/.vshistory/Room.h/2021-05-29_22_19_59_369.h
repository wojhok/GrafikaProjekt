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
	ShaderProgram* sp;

	Room(mat4 M, ShaderProgram* shaderProgram)
	{
		this->sp = shaderProgram;
		this->M = M;
		for (int i = 0; i < 6; i++)
		{
			this->quads.push_back(Quad());
		}
		this->translates = { vec3(0.0f,15.0f,0.0f),
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
		//this->tex = tex;
	}


	void drawRoom(){
		//sp->use();//Aktywacja programu cieniuj¹cego
		for (int i = 0; i < 6; i++)
		{
			mat4 M1 = M;
			M1 = translate(M, translates[i]);
			M1 = rotate(M, rotates[i].angle, rotates[i].axis);
			glUniformMatrix4fv(sp->u("M"), 1, false, value_ptr(M1));
			glEnableVertexAttribArray(sp->a("vertex"));
			glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, quads[i].verts.data());
			/*glEnableVertexAttribArray(sp->a("texCoord0"));
			glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, quads[i].texCoords.data());*/
			glEnableVertexAttribArray(sp->a("normal"));
			glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, quads[i].normals.data());
	
			/*glUniform1i(sp->u("textureMap0"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);*/

			glDrawArrays(GL_TRIANGLES, 0, quads[i].vertexCount); //Narysuj obiekt

			glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
			//glDisableVertexAttribArray(sp->a("texCoord0"));
			glDisableVertexAttribArray(sp->a("normal"));
		}
	}
private:

};