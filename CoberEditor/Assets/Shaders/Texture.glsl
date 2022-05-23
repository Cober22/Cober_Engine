/////////////////////////////// VERTEX
#type vertex
#version 460

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in vec3 a_Normal;
layout(location = 6) in int a_EntityID;

out vec4 v_Color;
out vec2 v_TexCoord;
flat out float v_TexIndex;
out float v_TilingFactor;
out flat int v_EntityID;

out vec3 v_FragPos;
out vec3 v_Normal;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat3 u_Normal;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	v_Normal = u_Normal * a_Normal;
	v_EntityID = a_EntityID;

	v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
	gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);
	//gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}


/////////////////////////////// FRAGMENT
#type fragment
#version 460
layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out int fragmentColor2;

struct DirLight {
	vec3 direction;
	vec3 color;

	float ambient;
	float diffuse;
	float specular;
};

struct PointLight {
	vec3 position;
	vec3 color;

	float constant;
	float linear;
	float quadratic;

	float ambient;
	float diffuse;
	float specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
	vec3 color;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    float ambient;
    float diffuse;
    float specular;       
};

in vec3 v_FragPos;
in vec3 v_Normal;

in vec4 v_Color;
in vec2 v_TexCoord;
flat in float v_TexIndex;
in float v_TilingFactor;
in flat int v_EntityID;

uniform sampler2D u_Textures[32];

uniform DirLight dirLight;
uniform PointLight pointLight[5];
uniform SpotLight spotLight[5];

vec4 CalculateDirLight(DirLight light,	   vec3 normal, vec4 texture);
vec4 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec4 texture);
vec4 CalculateSpotLight(SpotLight light,   vec3 normal, vec3 fragPos, vec4 texture);

void main()
{
	vec3 normal = normalize(v_Normal);
	vec4 result;
	vec4 texColor = v_Color;

	switch(int(v_TexIndex))
	{
		case  0: texColor *= texture(u_Textures[ 0], v_TexCoord * v_TilingFactor); break;
		case  1: texColor *= texture(u_Textures[ 1], v_TexCoord * v_TilingFactor); break;
		case  2: texColor *= texture(u_Textures[ 2], v_TexCoord * v_TilingFactor); break;
		case  3: texColor *= texture(u_Textures[ 3], v_TexCoord * v_TilingFactor); break;
		case  4: texColor *= texture(u_Textures[ 4], v_TexCoord * v_TilingFactor); break;
		case  5: texColor *= texture(u_Textures[ 5], v_TexCoord * v_TilingFactor); break;
		case  6: texColor *= texture(u_Textures[ 6], v_TexCoord * v_TilingFactor); break;
		case  7: texColor *= texture(u_Textures[ 7], v_TexCoord * v_TilingFactor); break;
		case  8: texColor *= texture(u_Textures[ 8], v_TexCoord * v_TilingFactor); break;
		case  9: texColor *= texture(u_Textures[ 9], v_TexCoord * v_TilingFactor); break;
		case 10: texColor *= texture(u_Textures[10], v_TexCoord * v_TilingFactor); break;
		case 11: texColor *= texture(u_Textures[11], v_TexCoord * v_TilingFactor); break;
		case 12: texColor *= texture(u_Textures[12], v_TexCoord * v_TilingFactor); break;
		case 13: texColor *= texture(u_Textures[13], v_TexCoord * v_TilingFactor); break;
		case 14: texColor *= texture(u_Textures[14], v_TexCoord * v_TilingFactor); break;
		case 15: texColor *= texture(u_Textures[15], v_TexCoord * v_TilingFactor); break;
		case 16: texColor *= texture(u_Textures[16], v_TexCoord * v_TilingFactor); break;
		case 17: texColor *= texture(u_Textures[17], v_TexCoord * v_TilingFactor); break;
		case 18: texColor *= texture(u_Textures[18], v_TexCoord * v_TilingFactor); break;
		case 19: texColor *= texture(u_Textures[19], v_TexCoord * v_TilingFactor); break;
		case 20: texColor *= texture(u_Textures[20], v_TexCoord * v_TilingFactor); break;
		case 21: texColor *= texture(u_Textures[21], v_TexCoord * v_TilingFactor); break;
		case 22: texColor *= texture(u_Textures[22], v_TexCoord * v_TilingFactor); break;
		case 23: texColor *= texture(u_Textures[23], v_TexCoord * v_TilingFactor); break;
		case 24: texColor *= texture(u_Textures[24], v_TexCoord * v_TilingFactor); break;
		case 25: texColor *= texture(u_Textures[25], v_TexCoord * v_TilingFactor); break;
		case 26: texColor *= texture(u_Textures[26], v_TexCoord * v_TilingFactor); break;
		case 27: texColor *= texture(u_Textures[27], v_TexCoord * v_TilingFactor); break;
		case 28: texColor *= texture(u_Textures[28], v_TexCoord * v_TilingFactor); break;
		case 29: texColor *= texture(u_Textures[29], v_TexCoord * v_TilingFactor); break;
		case 30: texColor *= texture(u_Textures[30], v_TexCoord * v_TilingFactor); break;
		case 31: texColor *= texture(u_Textures[31], v_TexCoord * v_TilingFactor); break;
	}
	
	result += CalculateDirLight(dirLight, normal, texColor);
	
	for(int i = 0; i < 5; i++) {
		if (pointLight[i].ambient != 0 || pointLight[i].diffuse != 0)
			result += CalculatePointLight(pointLight[i], normal, v_FragPos, texColor);
	}
	
	for(int i = 0; i < 5; i++) {
		if (spotLight[i].ambient != 0 || spotLight[i].diffuse != 0)
			result += CalculateSpotLight(spotLight[i], normal, v_FragPos, texColor);
	}

    fragmentColor = result;
	fragmentColor2 = v_EntityID;
}

vec4 CalculateDirLight(DirLight light, vec3 normal, vec4 texture) {

	vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

	vec4 color = vec4(light.color, 1.0);
	vec4 ambientLight  = vec4(light.ambient, light.ambient, light.ambient, 1.0);
	vec4 diffuseLight  = vec4(light.diffuse, light.diffuse, light.diffuse, 1.0);

    vec4 ambient  =  ambientLight  * color;
    vec4 diffuse  =  diffuseLight  * color * diff;
	
	return (ambient + diffuse) * texture;
}


vec4 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec4 texture) {
	
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	float distance	  = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

	vec4 color = vec4(light.color, 1.0);
	vec4 ambientLight  = vec4(light.ambient, light.ambient, light.ambient, 1.0);
	vec4 diffuseLight  = vec4(light.diffuse, light.diffuse, light.diffuse, 1.0);

    vec4 ambient  =  ambientLight  * color;
    vec4 diffuse  =  diffuseLight  * color * diff;
	
	return (ambient + diffuse) * attenuation * texture;
}

vec4 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec4 texture) {

    vec3 lightDir = normalize(light.position - fragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
    
	float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    float theta = dot(lightDir, normalize(-light.direction)); 
    float intensity = smoothstep(light.outerCutOff, light.cutOff, theta);
    
	vec4 color = vec4(light.color, 1.0);
	vec4 ambientLight  = vec4(light.ambient, light.ambient, light.ambient, 1.0);
	vec4 diffuseLight  = vec4(light.diffuse, light.diffuse, light.diffuse, 1.0);

    vec4 ambient  =  ambientLight  * color;
    vec4 diffuse  =  diffuseLight  * color * diff;
	
	return (ambient + diffuse) * attenuation * intensity * texture;
}