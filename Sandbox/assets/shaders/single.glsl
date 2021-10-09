#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

out vec4 v_Color;
out vec2 v_TexCoords;
out float v_TexIndex;
out float v_TilingFactor;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
	v_Color = a_Color;
	v_TexCoords = a_TexCoords;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;

	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0f);
}


#type fragment
#version 450

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoords;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 outputColor = v_Color;

	// ugly code because AMD SUCKS
	switch (int(v_TexIndex))
	{
		case 0:  outputColor *= texture(u_Textures[0],  v_TexCoords * v_TilingFactor); break;
		case 1:  outputColor *= texture(u_Textures[1],  v_TexCoords * v_TilingFactor); break;
		case 2:  outputColor *= texture(u_Textures[2],  v_TexCoords * v_TilingFactor); break;
		case 3:  outputColor *= texture(u_Textures[3],  v_TexCoords * v_TilingFactor); break;
		case 4:  outputColor *= texture(u_Textures[4],  v_TexCoords * v_TilingFactor); break;
		case 5:  outputColor *= texture(u_Textures[5],  v_TexCoords * v_TilingFactor); break;
		case 6:  outputColor *= texture(u_Textures[6],  v_TexCoords * v_TilingFactor); break;
		case 7:  outputColor *= texture(u_Textures[7],  v_TexCoords * v_TilingFactor); break;
		case 8:  outputColor *= texture(u_Textures[8],  v_TexCoords * v_TilingFactor); break;
		case 9:  outputColor *= texture(u_Textures[9],  v_TexCoords * v_TilingFactor); break;
		case 10: outputColor *= texture(u_Textures[10], v_TexCoords * v_TilingFactor); break;
		case 11: outputColor *= texture(u_Textures[11], v_TexCoords * v_TilingFactor); break;
		case 12: outputColor *= texture(u_Textures[12], v_TexCoords * v_TilingFactor); break;
		case 13: outputColor *= texture(u_Textures[13], v_TexCoords * v_TilingFactor); break;
		case 14: outputColor *= texture(u_Textures[14], v_TexCoords * v_TilingFactor); break;
		case 15: outputColor *= texture(u_Textures[15], v_TexCoords * v_TilingFactor); break;
		case 16: outputColor *= texture(u_Textures[16], v_TexCoords * v_TilingFactor); break;
		case 17: outputColor *= texture(u_Textures[17], v_TexCoords * v_TilingFactor); break;
		case 18: outputColor *= texture(u_Textures[18], v_TexCoords * v_TilingFactor); break;
		case 19: outputColor *= texture(u_Textures[19], v_TexCoords * v_TilingFactor); break;
		case 20: outputColor *= texture(u_Textures[20], v_TexCoords * v_TilingFactor); break;
		case 21: outputColor *= texture(u_Textures[21], v_TexCoords * v_TilingFactor); break;
		case 22: outputColor *= texture(u_Textures[22], v_TexCoords * v_TilingFactor); break;
		case 23: outputColor *= texture(u_Textures[23], v_TexCoords * v_TilingFactor); break;
		case 24: outputColor *= texture(u_Textures[24], v_TexCoords * v_TilingFactor); break;
		case 25: outputColor *= texture(u_Textures[25], v_TexCoords * v_TilingFactor); break;
		case 26: outputColor *= texture(u_Textures[26], v_TexCoords * v_TilingFactor); break;
		case 27: outputColor *= texture(u_Textures[27], v_TexCoords * v_TilingFactor); break;
		case 28: outputColor *= texture(u_Textures[28], v_TexCoords * v_TilingFactor); break;
		case 29: outputColor *= texture(u_Textures[29], v_TexCoords * v_TilingFactor); break;
		case 30: outputColor *= texture(u_Textures[30], v_TexCoords * v_TilingFactor); break;
		case 31: outputColor *= texture(u_Textures[31], v_TexCoords * v_TilingFactor); break;
	}

	o_Color = outputColor;
}
