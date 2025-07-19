#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in float a_TexIndex;

out vec4 v_Color;
out vec2 v_TexCoords;
out float v_TexIndex;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
	v_Color = a_Color;
	v_TexCoords = a_TexCoords;
	v_TexIndex = a_TexIndex;

	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}