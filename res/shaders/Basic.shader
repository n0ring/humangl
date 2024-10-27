#shader vertex
#version 330 core
// #define debug
$debug_flag$

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormCoord;
// layout(location = 3) in vec3 tangent;
// layout(location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 BoneIDs;
layout(location = 6) in vec4 Weights;

uniform mat4 u_MVP;
uniform mat4 u_model;
uniform mat4 u_proj;
uniform mat4 u_view;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 my_normal;
uniform int u_hasAnimation;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

// debug bones weight
#ifdef debug
flat out ivec4 BoneIds_debug;
out vec4 BoneWeight_debug;
#endif

void main(){
    if (u_hasAnimation == 1)
    {
        vec4 totalPosition = vec4(0.0f);
        vec3 totalNormal = vec3(0.0f);
        for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
        {
            if(BoneIDs[i] == -1) 
                continue;
            if(BoneIDs[i] >= MAX_BONES) 
            {
                totalPosition = vec4(pos, 1.0f);
                break;
            }
            vec4 localPosition = finalBonesMatrices[BoneIDs[i]] * vec4(pos, 1.0f);
            totalPosition += (localPosition * Weights[i]);
            
            vec3 localNormal = mat3(finalBonesMatrices[BoneIDs[i]]) * aNormCoord;
            totalNormal += localNormal * Weights[i];
        }

        mat4 viewModel = u_view * u_model;

        // gl_Position = u_proj * viewModel * pos; 
        TexCoord = aTexCoord;
        vec3 my_normal = normalize(mat3(u_model) * totalNormal);
        gl_Position =  u_proj * viewModel * totalPosition;
    }
    else 
    {
        gl_Position =  u_proj * u_view * u_model * vec4(pos, 1.0f);
    }
    FragPos = vec3(u_model * vec4(pos, 1.0f));
    

// debub bones weight
#ifdef debug
    BoneIds_debug = BoneIDs;
    BoneWeight_debug = Weights;
#endif
}

#shader fragment
#version 330 core
// #define debug
$debug_flag$

out vec4 FragColor;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 my_normal;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;
// uniform vec3 u_lightPos;
uniform int DisplayBoneIndex;

// debug bones weight
#ifdef debug
flat in ivec4 BoneIds_debug;
in vec4 BoneWeight_debug;
#endif

vec3 calculateLighting(vec3 normal, vec3 viewDir, vec3 lightDir, vec3 diffuseColor, vec3 specularColor, float shininess)
{
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * specularColor;

    return diffuse + specular;
}

void main() { 
#ifndef debug
	// vec3 u_lightPos = vec3(0, 0, -100);
	// vec3 viewPos = vec3(0, 0, 0);
    // vec3 diffuseColor = texture(texture_diffuse, TexCoord).rgb;
    // vec3 specularColor = texture(texture_specular, TexCoord).rgb;
    // vec3 normal = texture(texture_normal, TexCoord).rgb;
    // float height = texture(texture_height, TexCoord).r;

    // normal = normalize(normal * 2.0 - 1.0);

    // vec3 displacedNormal = normalize(normal + vec3(height) * 0.1);
    // vec3 lightDir = normalize(u_lightPos - FragPos);
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 lighting = calculateLighting(displacedNormal, viewDir, lightDir, diffuseColor, specularColor, 32.0);
    // FragColor = vec4(diffuseColor * specularColor * normal, 1.0);
    // FragColor = vec4(lighting, 1.0f);

    // debug remove after for textures 
    // FragColor = vec4(0.5f, 0, 0, 1.0f);

    // just texture 
    vec3 diffuseColor = texture(texture_diffuse, TexCoord).rgb;
    FragColor = vec4(diffuseColor, 1.0);

#else
    bool found = false;
    for (int i = 0; i < 4; i++)
    {
        if (DisplayBoneIndex == BoneIds_debug[i])
        {
            if (BoneWeight_debug[i] >= 0.7f)
                FragColor = vec4(1.0f, 0.0f, 0.0f, 0.0f) * BoneWeight_debug[i];
            else if (BoneWeight_debug[i] >= 0.4f && BoneWeight_debug[i] <= 0.6f)
                FragColor = vec4(0.0f, 1.0f, 0.0f, 0.0f) * BoneWeight_debug[i];
            else if (BoneWeight_debug[i] >= 0.1f)
                FragColor = vec4(1.0f, 1.0f, 0.0f, 0.0f) * BoneWeight_debug[i];
            else 
                FragColor = vec4(0.01f, 0.01f, 0.5f, 1.0f);
            found = true;
        }
    }
    if (!found)
        FragColor = vec4(0.01f, 0.01f, 0.5f, 1.0f);
    // debug bones 
    // set 
    FragColor[3] = 1.0f;
#endif
}
