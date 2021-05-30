#version 330

//Zm jednorodne
uniform sampler2D textureMap0;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

//Zm interpolowane
out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Atrybuty
in vec2 iTexCoord0;
in vec3 pixelPosition;
in vec3 normalOut;

void main(void) {
	float ambientStrenght = 0.1;
	vec3 ambient = ambientStrenght* vec3(1,1,1);
	///
	vec3 norm = normalize(normalOut);
	///
	vec3 lightDir = normalize(lightPosition- pixelPosition);
	float dif = max(dot(norm, lightDir),0);
	vec3 diffuse = dif*vec3(1,1,1)*texture(textureMap0,iTexCoord0).rgb;

	float specularStrenght = 0.61;
	///
	vec3 viewDir = normalize(viewPosition - pixelPosition);
	///
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0),32);

	vec3 specular = specularStrenght*spec*vec3(1,1,1);
	vec3 result = (ambient+diffuse+specular);

	pixelColor = vec4(result,1.0);
}
