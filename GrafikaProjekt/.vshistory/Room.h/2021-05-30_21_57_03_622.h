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
	std::vector <GLuint> tex;
	std::vector<mat4> matricies;
	float roomHeight;
	float roomWidth;
	ShaderProgram* sp;

	Room(mat4 M,float roomHeight, float roomWidth, ShaderProgram* sp, std::vector <GLuint> tex)
	{
		//this->sp = shaderProgram;
		this->roomHeight = roomHeight;
		this->roomWidth = roomWidth;
		this->sp = sp;
		
		this->tex = tex;
		for (int i = 0; i < 6; i++)
		{
			this->matricies.push_back(M);
		}
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
	void drawRoom()
	{
		for (int i = 0; i < 6; i++)
		{

			matricies[i] = glm::translate(matricies[i], translates[i]);
			matricies[i] = glm::rotate(matricies[i],rotates[i].angle, rotates[i].axis);
			if(i<2) matricies[i] = glm::scale(matricies[i], vec3(roomWidth, roomWidth, roomWidth));
			else matricies[i] = glm::scale(matricies[i], vec3(roomWidth, roomHeight, roomWidth));
			glUniformMatrix4fv(sp->u("M"), 1, false, value_ptr(matricies[i]));
			glEnableVertexAttribArray(sp->a("vertex"));
			glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, quads[i].verts.data());
			glEnableVertexAttribArray(sp->a("texCoord0"));
			glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, quads[i].texCoords.data());
			glEnableVertexAttribArray(sp->a("normal"));
			glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, quads[i].normals.data());

			glUniform1i(sp->u("textureMap0"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex[i]);

			glDrawArrays(GL_TRIANGLES, 0, quads[i].vertexCount); //Narysuj obiekt

			glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
			glDisableVertexAttribArray(sp->a("texCoord0"));
			glDisableVertexAttribArray(sp->a("normal"));
		}
	}

		
private:

};