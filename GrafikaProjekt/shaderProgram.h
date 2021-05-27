#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H


#include <GL/glew.h>
#include "stdio.h"



class ShaderProgram {
private:
	 //Uchwyt reprezentuj�cy program cieniujacy
	GLuint vertexShader; //Uchwyt reprezentuj�cy vertex shader
	GLuint geometryShader; //Uchwyt reprezentuj�cy geometry shader
	GLuint fragmentShader; //Uchwyt reprezentuj�cy fragment shader
	char* readFile(const char* fileName); //metoda wczytuj�ca plik tekstowy do tablicy znak�w
	GLuint loadShader(GLenum shaderType, const char* fileName); //Metoda wczytuje i kompiluje shader, a nast�pnie zwraca jego uchwyt
public:
	GLuint shaderProgram;
	ShaderProgram(const char* vertexShaderFile, const char* geometryShaderFile, const char* fragmentShaderFile);
	~ShaderProgram();
	void use(); //W��cza wykorzystywanie programu cieniuj�cego
	GLuint u(const char* variableName); //Pobiera numer slotu zwi�zanego z dan� zmienn� jednorodn�
	GLuint a(const char* variableName); //Pobiera numer slotu zwi�zanego z danym atrybutem
};




#endif