/////////////////////////////// VERTEX
#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TextCoord;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TextCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat3 u_Normal;

void main()
{
	v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
	v_TextCoord = a_TextCoord;
	v_Normal = u_Normal * a_Normal;		// Traspose of the inverse of model matrix * a_Normal
	gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);
}

/////////////////////////////// FRAGMENT
#type fragment
#version 460 core
out vec4 fragmentColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

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
in vec2 v_TextCoord;

uniform vec3 u_ViewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight[5];
uniform SpotLight spotLight[5];

vec4 CalculateDirLight(DirLight light,	   vec3 normal, vec3 viewDir);
vec4 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalculateSpotLight(SpotLight light,   vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 normal = normalize(v_Normal);
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec4 result;

	result += CalculateDirLight(dirLight, normal, viewDir);
	
	for(int i = 0; i < 5; i++) {
		if (pointLight[i].ambient != 0 || pointLight[i].diffuse != 0)
			result += CalculatePointLight(pointLight[i], normal, v_FragPos, viewDir);
	}
	
	for(int i = 0; i < 5; i++) {
		if (spotLight[i].ambient != 0 || spotLight[i].diffuse != 0)
			result += CalculateSpotLight(spotLight[i], normal, v_FragPos, viewDir);
	}

    fragmentColor = result;
}

vec4 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir) {

	vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec4 color = vec4(light.color, 1.0);
	vec4 ambientLight  = vec4(light.ambient, light.ambient, light.ambient, 1.0);
	vec4 diffuseLight  = vec4(light.diffuse, light.diffuse, light.diffuse, 1.0);
	vec4 specularLight = vec4(light.specular, light.specular, light.specular, 1.0);

    vec4 ambient  =  ambientLight  * color * texture(material.diffuse, v_TextCoord);
    vec4 diffuse  =  diffuseLight  * color * diff * texture(material.diffuse, v_TextCoord);
    vec4 specular =  specularLight * color * spec * texture(material.specular, v_TextCoord);
	
	return (ambient + diffuse + specular);
}


vec4 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	float distance	  = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

	vec4 color = vec4(light.color, 1.0);
	vec4 ambientLight  = vec4(light.ambient, light.ambient, light.ambient, 1.0);
	vec4 diffuseLight  = vec4(light.diffuse, light.diffuse, light.diffuse, 1.0);
	vec4 specularLight = vec4(light.specular, light.specular, light.specular, 1.0);

    vec4 ambient  =  ambientLight  * color * texture(material.diffuse, v_TextCoord);
    vec4 diffuse  =  diffuseLight  * color * diff * texture(material.diffuse, v_TextCoord);
    vec4 specular =  specularLight * color * spec * texture(material.specular, v_TextCoord);
	
	return (ambient + diffuse + specular) * attenuation;
}

vec4 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec3 lightDir = normalize(light.position - fragPos);
    
	// diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    
	// specular 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
	// attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
	// spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float intensity = smoothstep(light.outerCutOff, light.cutOff, theta);
    
	vec4 color = vec4(light.color, 1.0);
	vec4 ambientLight  = vec4(light.ambient, light.ambient, light.ambient, 1.0);
	vec4 diffuseLight  = vec4(light.diffuse, light.diffuse, light.diffuse, 1.0);
	vec4 specularLight = vec4(light.specular, light.specular, light.specular, 1.0);

    vec4 ambient  =  ambientLight  * color * texture(material.diffuse, v_TextCoord);
    vec4 diffuse  =  diffuseLight  * color * diff * texture(material.diffuse, v_TextCoord);
    vec4 specular =  specularLight * color * spec * texture(material.specular, v_TextCoord);

	return (ambient + diffuse + specular) * attenuation * intensity;
}