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
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "lodepng.h"
#include "Mesh.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>


class Model
{
public:
	std::string plik;
	std::vector<Mesh> meshes;
	std::vector<GLuint> tex;
	glm::mat4 matrix;
	glm::vec3 translate;
	ShaderProgram* sp;
	float angle;
	glm::vec3 rotateVec;
	Model(std::string plik, float resize, std::vector<GLuint> tex, glm::mat4 matrix, ShaderProgram* sp, glm::vec3 translate,float angle,glm::vec3 rotateVec)
	{
		this->resize = resize;
		this->plik = plik;
		loadModel(plik);
		this->tex = tex;
		this->matrix = matrix;
		this->sp = sp;
		this->translate = translate;
		this->angle = angle * M_PI / 180.0f;
		this->rotateVec = rotateVec;
	}

	void drawModel()
	{
		for (int i = 0; i < meshes.size(); i++)
		{	
			if (plik == "lamp.obj")
			{
				std::cout << meshes.size() << std::endl;

			}
			meshes[i].drawMesh(tex[i],matrix,sp,resize, translate,angle , rotateVec);
		}
	}

private:
	float resize;
	void loadModel(std::string plik) {
		using namespace std;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(plik,
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
		cout << importer.GetErrorString() << endl;
		if (scene)
		{
			this->processNode(scene->mRootNode, scene);
		}
	}
	void processNode(aiNode* node, const aiScene* scene)
	{
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			//std::cout << node->mNumMeshes << std::endl;
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene));
		}

		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene);
		}

	}
	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<glm::vec4> vertices;
		std::vector<glm::vec4> norms;
		std::vector<glm::vec2> textures;
		std::vector<unsigned int> indices;
		for (int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D vertex = mesh->mVertices[i]; //aiVector3D podobny do glm::vec3
			//vertex *= resize ;
			vertices.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));

			if (mesh->HasNormals())
			{
				aiVector3D normal = mesh->mNormals[i]; //Wektory znormalizowane
				//normal *= resize;
				norms.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));
				if (plik == "human.fbx")
				{
					//std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;

				}
			}
			//liczba zdefiniowanych zestaw?w wsp. teksturowania (zestaw?w jest max 8)
			//unsigned int liczba_zest = mesh->GetNumUVChannels();
			//Liczba sk?adowych wsp. teksturowania dla 0 zestawu.
			//unsigned int wymiar_wsp_tex = mesh->mNumUVComponents[0];
			//0 to numer zestawu wsp??rz?dnych teksturowania

			if (mesh->mTextureCoords[0])
			{
				aiVector3D texCoord = mesh->mTextureCoords[0][i];
				glm::vec2 vecTex;
				vecTex.x = mesh->mTextureCoords[0][i].x;
				vecTex.y = mesh->mTextureCoords[0][i].y;
				textures.push_back(vecTex);
				if (plik == "human.fbx")
				{
					//std::cout << vecTex.x << " " << vecTex.y << std::endl;
				}
			}
			//std::cout << vecTex.x << " " << vecTex.y << std::endl;
			//x,y,z wykorzystywane jako u,v,w. 0 je?eli tekstura ma mniej wymiar?w

		}

		//dla ka?dego wielok?ta sk?adowego
		for (int i = 0; i < mesh->mNumFaces; i++) {
			aiFace& face = mesh->mFaces[i]; //face to jeden z wielok?t?w siatki

			//dla ka?dego indeksu->wierzcho?ka tworz?cego wielok?t
			//dla aiProcess_Triangulate to zawsze b?dzie 3
			for (int j = 0; j < face.mNumIndices; j++) {
				//cout << face.mIndices[j] << " ";
				indices.push_back(face.mIndices[j]);
			}
			//cout << "h";
		}
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		/*for (int i = 0; i < 19; i++)
		{
			std::cout << i << " " << material->GetTextureCount((aiTextureType)i) << std::endl;
		}*/
		for (int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++)
		{
			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, i, &str);
			std::cout << str.C_Str() << std::endl;
		}
		return Mesh(vertices, norms, textures, indices);
	}
};