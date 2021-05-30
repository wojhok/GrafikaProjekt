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
class Model
{
public:
	std::vector<Mesh> meshes;
	std::vector<GLuint> tex;
	glm::mat4 matrix;
	Model(std::string plik, float resize, std::vector<GLuint> tex, glm::mat4 matrix)
	{
		this->resize = resize;
		loadModel(plik);
		this->tex = tex;
		this->matrix = matrix;
	}

	void drawModel()
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].drawMesh(tex[i],matrix);
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
		this->processNode(scene->mRootNode, scene);
	}
	void processNode(aiNode* node, const aiScene* scene)
	{
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
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

			aiVector3D normal = mesh->mNormals[i]; //Wektory znormalizowane
			//normal *= resize;
			norms.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));

			//liczba zdefiniowanych zestawów wsp. teksturowania (zestawów jest max 8)
			//unsigned int liczba_zest = mesh->GetNumUVChannels();
			//Liczba sk³adowych wsp. teksturowania dla 0 zestawu.
			//unsigned int wymiar_wsp_tex = mesh->mNumUVComponents[0];
			//0 to numer zestawu wspó³rzêdnych teksturowania


			aiVector3D texCoord = mesh->mTextureCoords[0][i];
			//texCoord *= resize;
			glm::vec2 vecTex;
			vecTex.x = mesh->mTextureCoords[0][i].x;
			vecTex.y = mesh->mTextureCoords[0][i].y;
			textures.push_back(vecTex);
			//std::cout << vecTex.x << " " << vecTex.y << std::endl;
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