#version 330 core

	layout(location = 0) in vec3 a_Position;
	layout(location = 1) in vec2 a_TextCoord;

	uniform mat4 u_ViewProjection;
	uniform mat4 u_Transform;

	out vec2 v_TextCoord;

	void main()
	{
		v_TextCoord = a_TextCoord;
		gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
	}