#include "Cube.h"
#include "Room.h"
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
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <vector>
using namespace std;
using namespace glm;



class Walls
{
public:
	std::vector<Cube> cubes;
	std::vector <glm::vec3> translates;
	std::vector <Rotate> rotates;
	std::vector <glm::vec3> scales;
	std::vector <glm::mat4> matricies;
	ShaderProgram* sp;
	GLuint tex;
	Walls(glm::mat4 M,  ShaderProgram* sp,GLuint tex,float roomWidth,float roomHeight)
	{
		this->sp = sp;
		this->tex = tex;
		for (int i = 0; i < 4; i++)
		{
			this->matricies.push_back(M);
		}
		for (int i = 0; i < 4; i++)
		{
			this->cubes.push_back(Cube(4.0f));
		}
		this->translates = { glm::vec3(roomWidth,roomHeight/2.0f-2.0f,0.0f),
			vec3(-roomWidth,roomHeight/2.0f-2.0f,0.0f),
			vec3(0.0f,roomHeight/2.0f -2.0f,roomWidth),
			vec3(0.0f,roomHeight/2.0f-2.0f,-roomWidth),
		};
		this->scales = {
			vec3(roomWidth/2.0f,roomHeight / 2.0f,1.0f),
			vec3(roomWidth / 2.0f,roomHeight / 2.0f,1.0f),
			vec3(1.0f,roomHeight / 2.0f,roomWidth / 2.0f),
			vec3(1.0f,roomHeight / 2.0f,roomWidth / 2.0f),
		};
	}
	
	void drawWalls()
	{
		for (int i = 0; i < 4; i++)
		{
			matricies[i] = glm::translate(matricies[i], this->translates[i]);
			//matricies[i] = glm::scale(matricies[i],this->scales[i] );
			glUniformMatrix4fv(sp->u("M"), 1, false, value_ptr(matricies[i]));
			glEnableVertexAttribArray(sp->a("vertex"));
			glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, this->cubes[i].cubeVertices.data());
			glEnableVertexAttribArray(sp->a("texCoord0"));
			glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, cubes[i].cubeTexCoords.data());
			glEnableVertexAttribArray(sp->a("normal"));
			glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, cubes[i].cubeNormals.data());

			glUniform1i(sp->u("textureMap0"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);

			glDrawArrays(GL_TRIANGLES, 0, cubes[i].vertexCount); //Narysuj obiekt

			glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
			glDisableVertexAttribArray(sp->a("texCoord0"));
			glDisableVertexAttribArray(sp->a("normal"));
		}
	}

private:

};
