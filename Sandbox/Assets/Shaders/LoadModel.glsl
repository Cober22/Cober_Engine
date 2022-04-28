/////////////////////////////// VERTEX
#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TextCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat3 u_Normal;

out vec2 v_TextCoord;

void main()
{
    v_TextCoord = a_TextCoord;
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}



/////////////////////////////// FRAGMENT
#type fragment
#version 460 core
in vec2 v_TextCoord;

out vec4 fragmentColor;

uniform sampler2D gSampler;

void main()
{
    fragmentColor = vec4(texture(gSampler, v_TextCoord));
}