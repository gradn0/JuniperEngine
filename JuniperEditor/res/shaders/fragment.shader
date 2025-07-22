#version 450 core

#define MAX_TEXTURES 32

in vec4 v_Color;
in vec2 v_TexCoords;
in float v_TexIndex;

out vec4 f_Color;

uniform sampler2D u_Textures[MAX_TEXTURES];

void main()
{
	f_Color = texture(u_Textures[int(v_TexIndex)], v_TexCoords) * v_Color;
}