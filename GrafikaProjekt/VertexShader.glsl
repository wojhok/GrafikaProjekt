#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lp;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 color;
in vec4 normal;
in vec2 texCoord0;

// Zmienne interpolowane
out vec4 ic;
out vec4 n;
out vec4 l;
out vec4 v;
out vec2 iTexCoord0;

void main(void) {

    l = normalize(V*lp - V*M*vertex); // w przerstrzeni oka
    n = normalize(V*M*normal); //w przestrzeni oka
    //vec4 r =  reflect(-l,n);
    v = normalize(vec4(0,0,0,1) - V*M*vertex);
    
    //float nl = clamp(dot(n,l),0,1);
    //float rv = pow(clamp(dot(r,v), 0, 1),25);

    iTexCoord0 = texCoord0;

    ic = color;
    gl_Position=P*V*M*vertex;

}
