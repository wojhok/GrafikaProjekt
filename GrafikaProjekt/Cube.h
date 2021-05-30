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
			//Œciana 1
			1.0f,-1.0f,-1.0f,1.0f,
			-1.0f, 1.0f,-1.0f,1.0f,
			-1.0f,-1.0f,-1.0f,1.0f,

			1.0f,-1.0f,-1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,
			-1.0f, 1.0f,-1.0f,1.0f,

			//Œciana 2
			-1.0f,-1.0f, 1.0f,1.0f,
			1.0f, 1.0f, 1.0f,1.0f,
			1.0f,-1.0f, 1.0f,1.0f,

			-1.0f,-1.0f, 1.0f,1.0f,
			-1.0f, 1.0f, 1.0f,1.0f,
			1.0f, 1.0f, 1.0f,1.0f,


			//Œciana 3
			-1.0f,-1.0f,-1.0f,1.0f,
			1.0f,-1.0f, 1.0f,1.0f,
			1.0f,-1.0f,-1.0f,1.0f,

			-1.0f,-1.0f,-1.0f,1.0f,
			-1.0f,-1.0f, 1.0f,1.0f,
			1.0f,-1.0f, 1.0f,1.0f,

			//Œciana 4
			-1.0f, 1.0f, 1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,
			1.0f, 1.0f, 1.0f,1.0f,

			-1.0f, 1.0f, 1.0f,1.0f,
			-1.0f, 1.0f,-1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,

			//Œciana 5
			-1.0f,-1.0f,-1.0f,1.0f,
			-1.0f, 1.0f, 1.0f,1.0f,
			-1.0f,-1.0f, 1.0f,1.0f,

			-1.0f,-1.0f,-1.0f,1.0f,
			-1.0f, 1.0f,-1.0f,1.0f,
			-1.0f, 1.0f, 1.0f,1.0f,

			//Œciana 6
			1.0f,-1.0f, 1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,
			1.0f,-1.0f,-1.0f,1.0f,

			1.0f,-1.0f, 1.0f,1.0f,
			1.0f, 1.0f, 1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,




		};


		 this->cubeColors = {
			//Œciana 1
			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,

			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,

			//Œciana 2
			0.0f,1.0f,0.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,

			0.0f,1.0f,0.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,

			//Œciana 3
			0.0f,0.0f,1.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,

			0.0f,0.0f,1.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,

			//Œciana 4
			1.0f,1.0f,0.0f,1.0f,
			1.0f,1.0f,0.0f,1.0f,
			1.0f,1.0f,0.0f,1.0f,

			1.0f,1.0f,0.0f,1.0f,
			1.0f,1.0f,0.0f,1.0f,
			1.0f,1.0f,0.0f,1.0f,

			//Œciana 5
			0.0f,1.0f,1.0f,1.0f,
			0.0f,1.0f,1.0f,1.0f,
			0.0f,1.0f,1.0f,1.0f,

			0.0f,1.0f,1.0f,1.0f,
			0.0f,1.0f,1.0f,1.0f,
			0.0f,1.0f,1.0f,1.0f,

			//Œciana 6
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,

			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
		};

		this->cubeNormals = {
			//Œciana 1
			0.0f, 0.0f,-1.0f,0.0f,
			0.0f, 0.0f,-1.0f,0.0f,
			0.0f, 0.0f,-1.0f,0.0f,

			0.0f, 0.0f,-1.0f,0.0f,
			0.0f, 0.0f,-1.0f,0.0f,
			0.0f, 0.0f,-1.0f,0.0f,

			//Œciana 2
			0.0f, 0.0f, 1.0f,0.0f,
			0.0f, 0.0f, 1.0f,0.0f,
			0.0f, 0.0f, 1.0f,0.0f,

			0.0f, 0.0f, 1.0f,0.0f,
			0.0f, 0.0f, 1.0f,0.0f,
			0.0f, 0.0f, 1.0f,0.0f,

			//Œciana 3
			0.0f,-1.0f, 0.0f,0.0f,
			0.0f,-1.0f, 0.0f,0.0f,
			0.0f,-1.0f, 0.0f,0.0f,

			0.0f,-1.0f, 0.0f,0.0f,
			0.0f,-1.0f, 0.0f,0.0f,
			0.0f,-1.0f, 0.0f,0.0f,

			//Œciana 4
			0.0f, 1.0f, 0.0f,0.0f,
			0.0f, 1.0f, 0.0f,0.0f,
			0.0f, 1.0f, 0.0f,0.0f,

			0.0f, 1.0f, 0.0f,0.0f,
			0.0f, 1.0f, 0.0f,0.0f,
			0.0f, 1.0f, 0.0f,0.0f,

			//Œciana 5
			-1.0f, 0.0f, 0.0f,0.0f,
			-1.0f, 0.0f, 0.0f,0.0f,
			-1.0f, 0.0f, 0.0f,0.0f,

			-1.0f, 0.0f, 0.0f,0.0f,
			-1.0f, 0.0f, 0.0f,0.0f,
			-1.0f, 0.0f, 0.0f,0.0f,

			//Œciana 6
			1.0f, 0.0f, 0.0f,0.0f,
			1.0f, 0.0f, 0.0f,0.0f,
			1.0f, 0.0f, 0.0f,0.0f,

			1.0f, 0.0f, 0.0f,0.0f,
			1.0f, 0.0f, 0.0f,0.0f,
			1.0f, 0.0f, 0.0f,0.0f,


		};

		this->cubeVertexNormals = {
			//Œciana 1
			1.0f,-1.0f,-1.0f,0.0f,
			-1.0f, 1.0f,-1.0f,0.0f,
			-1.0f,-1.0f,-1.0f,0.0f,

			1.0f,-1.0f,-1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,
			-1.0f, 1.0f,-1.0f,0.0f,

			//Œciana 2
			-1.0f,-1.0f, 1.0f,0.0f,
			1.0f, 1.0f, 1.0f,0.0f,
			1.0f,-1.0f, 1.0f,0.0f,

			-1.0f,-1.0f, 1.0f,0.0f,
			-1.0f, 1.0f, 1.0f,0.0f,
			1.0f, 1.0f, 1.0f,0.0f,


			//Œciana 3
			-1.0f,-1.0f,-1.0f,0.0f,
			1.0f,-1.0f, 1.0f,0.0f,
			1.0f,-1.0f,-1.0f,0.0f,

			-1.0f,-1.0f,-1.0f,0.0f,
			-1.0f,-1.0f, 1.0f,0.0f,
			1.0f,-1.0f, 1.0f,0.0f,

			//Œciana 4
			-1.0f, 1.0f, 1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,
			1.0f, 1.0f, 1.0f,0.0f,

			-1.0f, 1.0f, 1.0f,0.0f,
			-1.0f, 1.0f,-1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,

			//Œciana 5
			-1.0f,-1.0f,-1.0f,0.0f,
			-1.0f, 1.0f, 1.0f,0.0f,
			-1.0f,-1.0f, 1.0f,0.0f,

			-1.0f,-1.0f,-1.0f,0.0f,
			-1.0f, 1.0f,-1.0f,0.0f,
			-1.0f, 1.0f, 1.0f,0.0f,

			//Œciana 6
			1.0f,-1.0f, 1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,
			1.0f,-1.0f,-1.0f,0.0f,

			1.0f,-1.0f, 1.0f,0.0f,
			1.0f, 1.0f, 1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,
		};

		this->cubeTexCoords = {
			//Œciana 1
			1.0f*numTex,1.0f*numTex, 0.0f,0.0f, 0.0f,1.0f*numTex,
			1.0f*numTex,1.0f*numTex, 1.0f*numTex,0.0f, 0.0f,0.0f,

			//Œciana 2
			1.0f*numTex,1.0f*numTex, 0.0f,0.0f, 0.0f,1.0f*numTex,
			1.0f*numTex,1.0f*numTex, 1.0f*numTex,0.0f, 0.0f,0.0f,

			//Œciana 3
			1.0f* numTex,1.0f*numTex, 0.0f,0.0f, 0.0f,1.0f*numTex,
			1.0f*numTex,1.0f*numTex, 1.0f*numTex,0.0f, 0.0f,0.0f,

			//Œciana 4
			1.0f*numTex,1.0f*numTex, 0.0f,0.0f, 0.0f,1.0f*numTex,
			1.0f*numTex,1.0f*numTex, 1.0f*numTex,0.0f, 0.0f,0.0f,

			//Œciana 5
			1.0f*numTex,1.0f*numTex, 0.0f,0.0f, 0.0f,1.0f*numTex,
			1.0f*numTex,1.0f*numTex, 1.0f*numTex,0.0f, 0.0f,0.0f,

			//Œciana 6
			1.0f*numTex,1.0f*numTex, 0.0f,0.0f, 0.0f,1.0f*numTex,
			1.0f*numTex,1.0f*numTex, 1.0f*numTex,0.0f, 0.0f,0.0f,
		};
	
	}
};

