#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"

#include "vendor/stb_image/stb_image.h"
#include "nrg_math.hpp"
#include "Shader.hpp"

#include "glm_inc.hpp"
#include "GLM_utils.hpp"

#define MAX_BONE_INFLUENCE 4

#ifdef debug
	#define DEBUG_FLAG_STR "#define debug"
#endif

struct BoneInfo
{
	int id;
	nrg::mat4 offset;
};

struct Vertex
{
	nrg::vec3 Position;
	nrg::vec2 TexCoords;
	nrg::vec3 Normal;

	// tangent
	nrg::vec3 Tangent;
	// bitangent
	nrg::vec3 Bitangent;
	// bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	// weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture
{
	unsigned int id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	// mesh data
	std::vector<Vertex> m_Vertices; // to positions
	std::vector<unsigned int> m_Indices;
	std::vector<Texture> m_Textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, nrg::vec4 color)
		: m_Vertices(vertices), m_Indices(indices), m_Textures(textures), mesh_color(color) {}
	void Draw(Shader &shader);
	void setupMesh(); // set buffers
private:
	//  render data
	unsigned int VAO, VBO, EBO;

	std::unique_ptr<UniformBuffer> m_UBO;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<IndexBuffer> m_IBO;
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<::Texture> m_texture;
	nrg::vec4 mesh_color;
};

class Model
{
public:
	Model(const std::string path, int objId);
	void Draw();
	inline void setObjectAsActive() { m_isobjectActive = true; }
	inline void setObjectAsNotActive() { m_isobjectActive = false; }
	inline ModelState &getModelState() { return m_modelState; }

	// bones 
	std::map<std::string, BoneInfo> &getBonesInfoMap() { return m_BoneInfoMap; }
	int &getBoneCounter() { return m_BoneCounter; }

	void setTransforms(std::vector<nrg::mat4>& trans) { transforms = trans;}
private:
	// model data
	std::vector<Mesh> meshes;
	std::string m_directory;
	std::unordered_map<std::string, unsigned int> m_TexturesMap;
	std::unique_ptr<Shader> m_shader;

	// render data
	nrg::vec3 m_translation;
	nrg::mat4 m_proj, m_view;
	ModelState m_modelState;
	bool m_isobjectActive = false;
	unsigned int m_objId;
	nrg::vec3 view_vec;
	nrg::vec3 m_LightPos;

	bool hasAnimation = false;

	// bones
	std::map<std::string, BoneInfo> m_BoneInfoMap; // [name, boneinfo with id and offset]
	int m_BoneCounter = 0;

	std::vector<nrg::mat4> transforms;

	void loadModel(const std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene, nrg::mat4 transform);
	void loadMaterialTextures(const aiScene *scene, aiMaterial *mat, aiTextureType type, std::string typeName, std::vector<Texture> &textures);
	// nrg::mat4 convertMatrixToGLMFormat(const aiMatrix4x4 &aiMat);

	// textures
	unsigned int LoadTexture(const aiScene *scene, const std::string &fileName, bool &isValid);
	unsigned char *GetTextureBufferFromFile(const std::string &fileName, int &width, int &height, int &BPP);
	unsigned char *GetTextureBufferFromMemory(const aiTexture *texture, int &width, int &height, int &BPP);
	unsigned int LoadTextureBufferToGPU(unsigned char *localBuffer, int width, int height);

	// bones
	void SetVertexBoneDataToDefault(Vertex &vertex);
	void SetVertexBoneData(Vertex &vertex, int boneId, float weight);
	void ExtractBoneWeightForVertices(std::vector<Vertex> &vertices, aiMesh *mesh, const aiScene *scene);
};