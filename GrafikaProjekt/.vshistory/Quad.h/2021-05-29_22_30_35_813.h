#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION

class Quad
{
public:
    std::vector<float> verts;
//Tablica wspó³rzêdnych teksturowania
    std::vector<float> texCoords;
    //Tablica wektorów normalnych
    std::vector<float> normals;
    int vertexCount;
    Quad()
    {
        verts = {
            1.0f,-1.0f,0.0f,1.0f, //A
            -1.0f, 1.0f,0.0f,1.0f, //B
            -1.0f,-1.0f,0.0f,1.0f, //C

             1.0f,-1.0f,0.0f,1.0f, //A
             1.0f, 1.0f,0.0f,1.0f, //D
            -1.0f, 1.0f,0.0f,1.0f }; //B;

        texCoords = {
                1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //ABC
                1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, //ADB
                    };
        normals = {
               0.0f,0.0f,1.0f,0.0f, //A
               0.0f,0.0f,1.0f,0.0f, //B
               0.0f,0.0f,1.0f,0.0f, //C

               0.0f,0.0f,1.0f,0.0f, //A
               0.0f,0.0f,1.0f,0.0f, //D
               0.0f,0.0f,1.0f,0.0f, //B
                };
        vertexCount = 6;
    }
};


