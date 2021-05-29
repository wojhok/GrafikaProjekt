#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;


//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 normal;
in vec2 texCoord0;

// Zmienne interpolowane
out vec3 pixelPosition;
out vec3 normalOut;
out vec2 iTexCoord0;

void main(void) {

    pixelPosition = vec3(M*vertex);
    normalOut = vec3(M* normal);
    iTexCoord0 = texCoord0;
    gl_Position = P*V*M*vertex;

}
