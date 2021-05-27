#pragma once
#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "shaderProgram.h"

class Model
{
public:
	Model(std::string file)
	{
		this->loadModel(file);
	};
	~Model();
	void draw(ShaderProgram shader)
	{
		for (GLuint i = 0; i < this->meshes.size(); i++)
		{
			this->meshes[i].draw(shader);
		}

	}


private:
	std::vector<Mesh> meshes;

	void loadModel(std::string plik) {
		using namespace std;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(plik,
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
		cout << importer.GetErrorString() << endl;
		this->processNode(scene->mRootNode, scene);
		//for (int k = 0; k < scene->mNumMeshes; k++)
		//{
		//	aiMesh* mesh = scene->mMeshes[k];

		//	for (int i = 0; i < mesh->mNumVertices; i++) {
		//		aiVector3D vertex = mesh->mVertices[i]; //aiVector3D podobny do glm::vec3
		//		vertices.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));

		//		aiVector3D normal = mesh->mNormals[i]; //Wektory znormalizowane
		//		norms.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));

		//		//liczba zdefiniowanych zestawów wsp. teksturowania (zestawów jest max 8)
		//		//unsigned int liczba_zest = mesh->GetNumUVChannels();
		//		//Liczba sk³adowych wsp. teksturowania dla 0 zestawu.
		//		//unsigned int wymiar_wsp_tex = mesh->mNumUVComponents[0];
		//		//0 to numer zestawu wspó³rzêdnych teksturowania


		//		aiVector3D texCoord = mesh->mTextureCoords[0][i];
		//		textures.push_back(glm::vec2(texCoord.x, texCoord.y));
		//		//x,y,z wykorzystywane jako u,v,w. 0 je¿eli tekstura ma mniej wymiarów

		//	}

		//	//dla ka¿dego wielok¹ta sk³adowego
		//	for (int i = 0; i < mesh->mNumFaces; i++) {
		//		aiFace& face = mesh->mFaces[i]; //face to jeden z wielok¹tów siatki

		//		//dla ka¿dego indeksu->wierzcho³ka tworz¹cego wielok¹t
		//		//dla aiProcess_Triangulate to zawsze bêdzie 3
		//		for (int j = 0; j < face.mNumIndices; j++) {
		//			//cout << face.mIndices[j] << " ";
		//			indices.push_back(face.mIndices[j]);
		//		}
		//		//cout << "h";
		//	}
		//}
	}

	void processNode(aiNode* node, const aiScene* scene)
	{
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			// The node object only contains indices to index the actual objects in the scene.
			// The scene contains all the data, node is just to keep stuff organized(like relations between nodes).
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
		// Data to fill
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;
		// Walk through each of the mesh's vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			// Texture Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texturecoordinates ?
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			vertices.push_back(vertex);	
		}
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (GLuint j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// Diffuse maps
			std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material,
				aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(),
				diffuseMaps.end());
		}
		return Mesh(vertices, indices, textures);
	}

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName)
	{
		std::vector<Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			GLboolean skip = false;
			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (textures_loaded[j].path == str)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // A texture with the same fil epath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{ // If texture hasn't been loaded already, load it
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				this->textures_loaded.push_back(texture); // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}
};