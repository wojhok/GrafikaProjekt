#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "Model.h"
#include "shaderProgram.h"
using namespace std;
using namespace glm;

//Klasa Mesh - jeden element wczytywanego obiektu np noga broñ itp itd

struct Vertex {
    vec3 Position;
    vec3 Normal;   
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    // mesh dane
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO;

    // konstruktor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // settowanie bufferów i pointerów.
        setupMesh();
    }

    // renderowanie mesha
    void Draw(ShaderProgram& shader)
    {
        // przypisanie tekstur
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // aktywowanie tekstury z ka¿da iteracja kolejna tekstura w ten sposób niewa¿ne ile obiekt ich ma wszyskie wczytamy
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = to_string(specularNr++); 
            else if (name == "texture_normal")
                number = to_string(normalNr++); 
            else if (name == "texture_height")
                number = to_string(heightNr++); 

            
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // bindowanie tekstury
            glBindTexture(GL_TEXTURE_2D, textures[i].id); //(typ, teksura wczytywana po ID)
        }

        // rysowanie mesha
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // zamykanie
        glActiveTexture(GL_TEXTURE0);
    }

private:
    unsigned int VBO, EBO; //vertex buffer object i element buffer object

    // wczytywanie wszystkich wektorów
    void setupMesh()
    {
        //towrzenie wektorów/bufferów 
        glGenVertexArrays(1, &VAO); //(liczba arrayów, gdzie przypisaæ id arraya)
        glGenBuffers(1, &VBO); //to samo co array tylko dla bufferów
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO); //wi¹¿e vertex arraya ze zmienn¹ 
        // wczytywanie danych do bufera
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //alokowanie pamiêci i storowanie w buferze (target buffer, rozmiar, co ma byæ skopiowane do buffora, funkcja rysowania)
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        //pointery
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); //(indeks, rozmiar, typ, czy znormalizowany, krok, pointer)
        //normalne 
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        //koordy textur
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        //tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        //bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent)); //offset(struct, nazwa structa)

        glBindVertexArray(0);
    }
};


