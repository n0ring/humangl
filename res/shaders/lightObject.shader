#shader vertex
#version 330 core
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormCoord;

out vec4 v_ColorCoord;
uniform mat4 u_MVP;
uniform mat4 u_model;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;



vec4 getColorFromPosition(vec4 poss) {
    return vec4(
        0.5 * sin(poss.x) + 0.5,
        0.5 * sin(poss.y) + 0.5,
        0.5 * sin(poss.z) + 0.5,
		1.0f
    );
}

void main(){
	gl_Position = u_MVP * pos; 
	TexCoord = aTexCoord; // texture
	v_ColorCoord = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    FragPos = vec3(u_model * pos);
    Normal = mat3(transpose(inverse(u_model))) *  normalize(aNormCoord);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_ColorCoord;
in vec2 TexCoord;

in vec3 FragPos;
in vec3 Normal;

uniform vec4 our_color;
uniform sampler2D u_Texture;
uniform int u_RenderMode;
uniform int u_HasNormal;
uniform int u_Light;
uniform int u_isActive;
uniform vec3 u_lightPos;



void main() {
	if (u_Light == 1 && u_lightPos.x == 1) // just to remove Warning
	{}

	if (u_RenderMode == 0) // color
	{
		if (u_HasNormal == 1)
			color = vec4(v_ColorCoord);
		else 
			color = vec4(v_ColorCoord);
	}
	else  // texture
	{
		if (u_HasNormal == 1)
			color = vec4(texture(u_Texture, TexCoord));
		else 
			color = vec4(texture(u_Texture, TexCoord));
	}
	if (u_isActive == 1)
		color *= 1.4f;
}