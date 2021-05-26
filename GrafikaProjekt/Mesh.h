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
	std::string plik;

    // konstruktor
    Mesh(std::string plik)
    {
		this->plik = plik;

        // settowanie bufferów i pointerów.
        loadModel();
    }

    // renderowanie mesha
	void loadModel() {
		using namespace std;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(this->plik,
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
		cout << importer.GetErrorString() << endl;

		for (int k = 0; k < scene->mNumMeshes; k++)
		{
			aiMesh* mesh = scene->mMeshes[k];

			for (int i = 0; i < mesh->mNumVertices; i++) {
				aiVector3D vertex = mesh->mVertices[i]; //aiVector3D podobny do glm::vec3
				vertices.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));

				aiVector3D normal = mesh->mNormals[i]; //Wektory znormalizowane
				norms.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));

				//liczba zdefiniowanych zestawów wsp. teksturowania (zestawów jest max 8)
				//unsigned int liczba_zest = mesh->GetNumUVChannels();
				//Liczba sk³adowych wsp. teksturowania dla 0 zestawu.
				//unsigned int wymiar_wsp_tex = mesh->mNumUVComponents[0];
				//0 to numer zestawu wspó³rzêdnych teksturowania


				aiVector3D texCoord = mesh->mTextureCoords[0][i];
				textures.push_back(glm::vec2(texCoord.x, texCoord.y));
				//x,y,z wykorzystywane jako u,v,w. 0 je¿eli tekstura ma mniej wymiarów

			}

			//dla ka¿dego wielok¹ta sk³adowego
			for (int i = 0; i < mesh->mNumFaces; i++) {
				aiFace& face = mesh->mFaces[i]; //face to jeden z wielok¹tów siatki

				//dla ka¿dego indeksu->wierzcho³ka tworz¹cego wielok¹t
				//dla aiProcess_Triangulate to zawsze bêdzie 3
				for (int j = 0; j < face.mNumIndices; j++) {
					//cout << face.mIndices[j] << " ";
					indices.push_back(face.mIndices[j]);
				}
				//cout << "h";
			}
		}
	}
};


