#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
class Cube
{
public:
	std::vector<float>  cubeVertices;
	std::vector<float>  cubeColors;
	std::vector<float>  cubeTexCoords;
	std::vector<float>  cubeNormals;
	std::vector<float>  cubeVertexNormals;
	int vertexCount;

	Cube(float numTex)
	{
		this->vertexCount = 36;

		this->cubeVertices= {
			//�ciana 1
			1.0f,-1.0f,-1.0f,1.0f,
			-1.0f, 1.0f,-1.0f,1.0f,
			-1.0f,-1.0f,-1.0f,1.0f,

			1.0f,-1.0f,-1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,
			-1.0f, 1.0f,-1.0f,1.0f,

			//�ciana 2
			-1.0f,-1.0f, 1.0f,1.0f,
			1.0f, 1.0f, 1.0f,1.0f,
			1.0f,-1.0f, 1.0f,1.0f,

			-1.0f,-1.0f, 1.0f,1.0f,
			-1.0f, 1.0f, 1.0f,1.0f,
			1.0f, 1.0f, 1.0f,1.0f,


			//�ciana 3
			-1.0f,-1.0f,-1.0f,1.0f,
			1.0f,-1.0f, 1.0f,1.0f,
			1.0f,-1.0f,-1.0f,1.0f,

			-1.0f,-1.0f,-1.0f,1.0f,
			-1.0f,-1.0f, 1.0f,1.0f,
			1.0f,-1.0f, 1.0f,1.0f,

			//�ciana 4
			-1.0f, 1.0f, 1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,
			1.0f, 1.0f, 1.0f,1.0f,

			-1.0f, 1.0f, 1.0f,1.0f,
			-1.0f, 1.0f,-1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,

			//�ciana 5
			-1.0f,-1.0f,-1.0f,1.0f,
			-1.0f, 1.0f, 1.0f,1.0f,
			-1.0f,-1.0f, 1.0f,1.0f,

			-1.0f,-1.0f,-1.0f,1.0f,
			-1.0f, 1.0f,-1.0f,1.0f,
			-1.0f, 1.0f, 1.0f,1.0f,

			//�ciana 6
			1.0f,-1.0f, 1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,
			1.0f,-1.0f,-1.0f,1.0f,

			1.0f,-1.0f, 1.0f,1.0f,
			1.0f, 1.0f, 1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,




		};


		 this->cubeColors = {
			//�ciana 1
			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,

			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,

			//�ciana 2
			0.0f,1.0f,0.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,

			0.0f,1.0f,0.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,

			//�ciana 3
			0.0f,0.0f,1.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,

			0.0f,0.0f,1.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,

			//�ciana 4
			1.0f,1.0f,0.0f,1.0f,
			1.0f,1.0f,0.0f,1.0f,
			1.0f,1.0f,0.0f,1.0f,

			1.0f,1.0f,0.0f,1.0f,
			1.0f,1.0f,0.0f,1.0f,
			1.0f,1.0f,0.0f,1.0f,

			//�ciana 5
			0.0f,1.0f,1.0f,1.0f,
			0.0f,1.0f,1.0f,1.0f,
			0.0f,1.0f,1.0f,1.0f,

			0.0f,1.0f,1.0f,1.0f,
			0.0f,1.0f,1.0f,1.0f,
			0.0f,1.0f,1.0f,1.0f,

			//�ciana 6
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,

			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
		};

		this->cubeNormals = {
			//�ciana 1
			0.0f, 0.0f,-1.0f,0.0f,
			0.0f, 0.0f,-1.0f,0.0f,
			0.0f, 0.0f,-1.0f,0.0f,

			0.0f, 0.0f,-1.0f,0.0f,
			0.0f, 0.0f,-1.0f,0.0f,
			0.0f, 0.0f,-1.0f,0.0f,

			//�ciana 2
			0.0f, 0.0f, 1.0f,0.0f,
			0.0f, 0.0f, 1.0f,0.0f,
			0.0f, 0.0f, 1.0f,0.0f,

			0.0f, 0.0f, 1.0f,0.0f,
			0.0f, 0.0f, 1.0f,0.0f,
			0.0f, 0.0f, 1.0f,0.0f,

			//�ciana 3
			0.0f,-1.0f, 0.0f,0.0f,
			0.0f,-1.0f, 0.0f,0.0f,
			0.0f,-1.0f, 0.0f,0.0f,

			0.0f,-1.0f, 0.0f,0.0f,
			0.0f,-1.0f, 0.0f,0.0f,
			0.0f,-1.0f, 0.0f,0.0f,

			//�ciana 4
			0.0f, 1.0f, 0.0f,0.0f,
			0.0f, 1.0f, 0.0f,0.0f,
			0.0f, 1.0f, 0.0f,0.0f,

			0.0f, 1.0f, 0.0f,0.0f,
			0.0f, 1.0f, 0.0f,0.0f,
			0.0f, 1.0f, 0.0f,0.0f,

			//�ciana 5
			-1.0f, 0.0f, 0.0f,0.0f,
			-1.0f, 0.0f, 0.0f,0.0f,
			-1.0f, 0.0f, 0.0f,0.0f,

			-1.0f, 0.0f, 0.0f,0.0f,
			-1.0f, 0.0f, 0.0f,0.0f,
			-1.0f, 0.0f, 0.0f,0.0f,

			//�ciana 6
			1.0f, 0.0f, 0.0f,0.0f,
			1.0f, 0.0f, 0.0f,0.0f,
			1.0f, 0.0f, 0.0f,0.0f,

			1.0f, 0.0f, 0.0f,0.0f,
			1.0f, 0.0f, 0.0f,0.0f,
			1.0f, 0.0f, 0.0f,0.0f,


		};

		this->cubeVertexNormals = {
			//�ciana 1
			1.0f,-1.0f,-1.0f,0.0f,
			-1.0f, 1.0f,-1.0f,0.0f,
			-1.0f,-1.0f,-1.0f,0.0f,

			1.0f,-1.0f,-1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,
			-1.0f, 1.0f,-1.0f,0.0f,

			//�ciana 2
			-1.0f,-1.0f, 1.0f,0.0f,
			1.0f, 1.0f, 1.0f,0.0f,
			1.0f,-1.0f, 1.0f,0.0f,

			-1.0f,-1.0f, 1.0f,0.0f,
			-1.0f, 1.0f, 1.0f,0.0f,
			1.0f, 1.0f, 1.0f,0.0f,


			//�ciana 3
			-1.0f,-1.0f,-1.0f,0.0f,
			1.0f,-1.0f, 1.0f,0.0f,
			1.0f,-1.0f,-1.0f,0.0f,

			-1.0f,-1.0f,-1.0f,0.0f,
			-1.0f,-1.0f, 1.0f,0.0f,
			1.0f,-1.0f, 1.0f,0.0f,

			//�ciana 4
			-1.0f, 1.0f, 1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,
			1.0f, 1.0f, 1.0f,0.0f,

			-1.0f, 1.0f, 1.0f,0.0f,
			-1.0f, 1.0f,-1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,

			//�ciana 5
			-1.0f,-1.0f,-1.0f,0.0f,
			-1.0f, 1.0f, 1.0f,0.0f,
			-1.0f,-1.0f, 1.0f,0.0f,

			-1.0f,-1.0f,-1.0f,0.0f,
			-1.0f, 1.0f,-1.0f,0.0f,
			-1.0f, 1.0f, 1.0f,0.0f,

			//�ciana 6
			1.0f,-1.0f, 1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,
			1.0f,-1.0f,-1.0f,0.0f,

			1.0f,-1.0f, 1.0f,0.0f,
			1.0f, 1.0f, 1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,
		};

		this->cubeTexCoords = {
			//�ciana 1
			1.0f*numTex,1.0f*numTex, 0.0f,0.0f, 0.0f,1.0f*numTex,
			1.0f*numTex,1.0f*numTex, 1.0f*numTex,0.0f, 0.0f,0.0f,

			//�ciana 2
			1.0f*numTex,1.0f*numTex, 0.0f,0.0f, 0.0f,1.0f*numTex,
			1.0f*numTex,1.0f*numTex, 1.0f*numTex,0.0f, 0.0f,0.0f,

			//�ciana 3
			1.0f* numTex,1.0f*numTex, 0.0f,0.0f, 0.0f,1.0f*numTex,
			1.0f*numTex,1.0f*numTex, 1.0f*numTex,0.0f, 0.0f,0.0f,

			//�ciana 4
			1.0f*numTex,1.0f*numTex, 0.0f,0.0f, 0.0f,1.0f*numTex,
			1.0f*numTex,1.0f*numTex, 1.0f*numTex,0.0f, 0.0f,0.0f,

			//�ciana 5
			1.0f*numTex,1.0f*numTex, 0.0f,0.0f, 0.0f,1.0f*numTex,
			1.0f*numTex,1.0f*numTex, 1.0f*numTex,0.0f, 0.0f,0.0f,

			//�ciana 6
			1.0f*numTex,1.0f*numTex, 0.0f,0.0f, 0.0f,1.0f*numTex,
			1.0f*numTex,1.0f*numTex, 1.0f*numTex,0.0f, 0.0f,0.0f,
		};
	
	}
};

