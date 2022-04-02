/////////////////////////////// VERTEX
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TextCoord;

out vec3 v_Normal;
out vec3 v_FragPos;
out vec2 v_TextCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat4 u_Transform;
uniform mat3 u_Normal;

void main()
{
	v_FragPos = vec3(u_Model * u_Transform * vec4(a_Position, 1.0));
	v_TextCoord = a_TextCoord;
	v_Normal = u_Normal * a_Normal;		// Traspose of the inverse of model matrix * a_Normal
	gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);
}

/////////////////////////////// FRAGMENT
#type fragment
#version 330 core
out vec4 fragmentColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TextCoord;

uniform Material material;
uniform Light light;
uniform vec3 u_ViewPos;
uniform vec4 u_Color;

void main()
{
	// ambient
	vec4 ambient = vec4(light.ambient, 1.0) * texture(material.diffuse, v_TextCoord);

	// diffuse
	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(light.position - v_FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = vec4(light.diffuse, 1.0) * diff * texture(material.diffuse, v_TextCoord);

	// specular
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec4 specular = vec4(light.specular, 1.0) * spec * texture(material.specular, v_TextCoord) * vec4(light.specular, 1.0);  

	fragmentColor = vec4(ambient + diffuse + specular) * u_Color;
}