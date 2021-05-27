#version 330

//Zm jednorodne
uniform sampler2D textureMap0;

//Zm interpolowane
out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Atrybuty
in vec4 ic;
in vec4 l;
in vec4 v;
in vec4 n;
in vec2 iTexCoord0;

void main(void) {
	vec4 r =  reflect(-l,n);

	//Parametry powierzchni
	vec4 kd = texture(textureMap0, iTexCoord0);
	vec4 ks = vec4(1, 1, 1, 1);

	//Obliczanie oœwietlenia
	float nl = clamp(dot(n,l),0,1);
    float rv = pow(clamp(dot(r,v), 0, 1),25);


	vec4 icol = vec4(kd.rgb*nl, kd.a ) + vec4(ks.rgb*rv,0);
	pixelColor=icol;
}
