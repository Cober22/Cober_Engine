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
uniform vec3 u_Normal;

void main()
{
	v_FragPos = vec3(u_Model * u_Transform * vec4(a_Position, 1.0));
	v_TextCoord = a_TextCoord;
	v_Normal = u_Normal * a_Normal;		// Traspose of the inverse of model matrix * a_Normal
	v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);
}


#type fragment
#version 330 core
			
out vec4 color;

in vec2 v_TextCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform sampler2D u_Texture;
uniform vec3 u_ViewPos;
uniform vec4 u_Color;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;

void main()
{
	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_LightColor;

	// diffuse
	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightPos - v_FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * u_LightColor;

	// specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specular = specularStrength * spec * u_LightColor;  

	color = vec4((ambient + diffuse + specular), 1.0) * texture(u_Texture, v_TextCoord) * u_Color;
	//color = (ambient + diffuse + specular) * u_Color;
}