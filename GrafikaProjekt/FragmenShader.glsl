#version 330
#define NUM_LIGHT 5
//Zm jednorodne
uniform sampler2D textureMap0;
uniform vec3 lightPositions[NUM_LIGHT];
uniform vec3 viewPosition;
uniform vec3 lightColors[NUM_LIGHT];

//Zm interpolowane
out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Atrybuty
in vec2 iTexCoord0;
in vec3 pixelPosition;
in vec3 normalOut;


vec3 calLight(vec3 lightPosition,vec3 norm, sampler2D textureMap, vec3 viewDir, vec3 lightColor)
{
	float ambientStrenght = 0.01;
	vec3 ambient = ambientStrenght* vec3(1,1,1);
	vec3 lightDir = normalize(lightPosition- pixelPosition);
	float dif = max(dot(norm, lightDir),0);
	vec3 diffuse = dif*lightColor*texture(textureMap0,iTexCoord0).rgb;

	float specularStrenght = 0.01;
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0),32);

	vec3 specular = specularStrenght*spec*lightColor;
	float dist =  length(lightPosition - pixelPosition);
	float attenuation = 1/(1.0 + 0.01*dist + 0.007*dist*dist);
	ambient *= attenuation;
	diffuse *=attenuation;
	specular *=attenuation;
	return (ambient + diffuse + specular);
}

void main(void) {
	
	vec3 norm = normalize(normalOut);
	vec3 viewDir = normalize(viewPosition - pixelPosition);
	vec3 result ;
	for (int i =0; i < NUM_LIGHT; i++)
	{
		result += calLight(lightPositions[i],norm, textureMap0, viewDir, lightColors[i]);
	}


	pixelColor = vec4(result,1.0);
}
