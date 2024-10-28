
#include "Model.hpp"

Model::Model(const std::string path, int objId)
	: m_objId(objId),
	  m_translation(0, 0, 0),
	  m_proj(nrg::vec4(1.358f, 0.0f, 0.0f, 0.0f),
			 nrg::vec4(0.0f, 2.41421, 0.0f, 0.0f),
			 nrg::vec4(0.0f, 0.0f, -1.0001f, -1.0f),
			 nrg::vec4(0.0f, 0.0f, -0.020001, 0.0f)),
	  m_view(nrg::translate(nrg::mat4(1.0f), nrg::vec3(0, 0, 0))),
	  view_vec(0, 0, 0)

{
	nrg::vec3 cameraPos = nrg::vec3(0.0f, 0.0f, 7.0f); // Позиция камеры
	nrg::vec3 targetPos = nrg::vec3(0.0f, 0.0f, 0.0f); // Точка, на которую смотрит камера
	nrg::vec3 upVector = nrg::vec3(0.0f, 1.0f, 0.0f);  // Вверх

	std::unordered_map<std::string, std::string> mp;
#ifdef debug
	mp["debug_flag"] = DEBUG_FLAG_STR;
#else
	mp["debug_flag"] = "";
#endif
	m_shader = std::make_unique<Shader>("res/shaders/Basic.shader", mp);

	loadModel(path);
	std::cout << path << " model loaded!\n";

	for (auto &mesh : meshes)
		mesh.setupMesh();
}
void Model::loadModel(const std::string path)
{
	unsigned int assimp_flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;
	// | aiProcess_PreTransformVertices;
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(path, assimp_flags);
	if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mRootNode == nullptr)
	{
		std::cout << "ASIMP error: " << importer.GetErrorString() << std::endl;
		return;
	}
	hasAnimation = scene->HasAnimations();
	size_t endOfDir = path.find_last_of('/');
	if (endOfDir != std::string::npos)
		m_directory = path.substr(0, endOfDir);
	processNode(scene->mRootNode, scene); // go rec on full tree
	m_modelState.boneCount = m_BoneCounter;
	std::cout << "bones number: " << m_BoneCounter << std::endl;
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	aiMesh *mesh;
	aiMatrix4x4 nodeTransformation = node->mTransformation;
	nrg::mat4 nodeTransform = GLM_utils::convertMatrixToGLMFormat(nodeTransformation);
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene, nodeTransform));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene, nrg::mat4 transform)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indeces;
	std::vector<Texture> textures;
	nrg::vec4 mesh_color;

	// positions, texture coords, normals
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex tmpVertex;
		SetVertexBoneDataToDefault(tmpVertex);
		tmpVertex.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
		tmpVertex.Normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
		if (mesh->mTextureCoords[0])
		{
			tmpVertex.TexCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
		}
		else
		{
			tmpVertex.TexCoords = {0.0f, 0.0f};
		}
		vertices.push_back(tmpVertex);
	}

	// indeces
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		for (unsigned int k = 0; k < mesh->mFaces[i].mNumIndices; k++)
		{
			indeces.push_back(mesh->mFaces[i].mIndices[k]);
		}
	}
	// material
	if (mesh->mMaterialIndex >= 0)
	{
		aiColor4D diffuseColor;
		loadMaterialTextures(scene, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, "texture_diffuse", textures);
		// loadMaterialTextures(scene, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR, "texture_specular", textures);
		// loadMaterialTextures(scene, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_HEIGHT, "texture_normal", textures);
		// loadMaterialTextures(scene, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_AMBIENT, "texture_height", textures);
		if (textures.empty() && AI_SUCCESS == aiGetMaterialColor(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_COLOR_DIFFUSE, &diffuseColor))
		{
			mesh_color = nrg::vec4(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);
		}
	}
	// bones
	if (mesh->HasBones() && hasAnimation)
	{
		ExtractBoneWeightForVertices(vertices, mesh, scene);
	}
	return Mesh(vertices, indeces, textures, mesh_color);
}

/*-------------- textures--------------*/
void Model::loadMaterialTextures(const aiScene *scene, aiMaterial *mat, aiTextureType type, std::string typeName, std::vector<Texture> &textures)
{
	aiString fileName;
	bool isTextureValid;
	Texture texture;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		mat->GetTexture(type, i, &fileName);
		// const aiTexture *embeddedTexture = scene->GetEmbeddedTexture(fileName.C_Str());
		texture.type = typeName;
		texture.path = fileName;
		if (m_TexturesMap.count(fileName.C_Str()) == 0) // new texture
		{
			texture.id = LoadTexture(scene, fileName.C_Str(), isTextureValid);
			if (isTextureValid)
			{
				textures.push_back(texture);
				std::cout << "Texture filename: " << fileName.C_Str() << " with id: " << texture.id << std::endl;
				m_TexturesMap[fileName.C_Str()] = texture.id;
			}
		}
		else // texture already loaded
		{
			texture.id = m_TexturesMap[fileName.C_Str()];
			textures.push_back(texture);
		}
	}
}

unsigned int Model::LoadTexture(const aiScene *scene, const std::string &fileName, bool &isValid)
{
	unsigned int id;
	unsigned char *localBuffer;
	int width, height, BPP;
	const aiTexture *embeddedTexture = scene->GetEmbeddedTexture(fileName.c_str());
	if (embeddedTexture)
	{
		localBuffer = GetTextureBufferFromMemory(embeddedTexture, width, height, BPP);
	}
	else
	{
		localBuffer = GetTextureBufferFromFile(fileName, width, height, BPP);
	}
	if (localBuffer == nullptr)
	{
		std::cout << "load texture error with " << fileName << std::endl;
		isValid = false;
		return 0;
	}
	id = LoadTextureBufferToGPU(localBuffer, width, height);
	if (localBuffer)
		stbi_image_free(localBuffer);
	// TODO check for id and load process to gpu
	isValid = true;
	return id;
}

unsigned int Model::LoadTextureBufferToGPU(unsigned char *localBuffer, int width, int height)
{
	unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return id;
}

unsigned char *Model::GetTextureBufferFromMemory(const aiTexture *texture, int &width, int &height, int &BPP)
{
	int len = texture->mHeight == 0 ? texture->mWidth : texture->mWidth * texture->mHeight;
	// stbi_set_flip_vertically_on_load(1); // posible to remove cause of assimp do the same
	return stbi_load_from_memory(reinterpret_cast<unsigned char *>(texture->pcData), len, &width, &height, &BPP, 4);
}

unsigned char *Model::GetTextureBufferFromFile(const std::string &fileName, int &width, int &height, int &BPP)
{
	std::string fullFileName = m_directory + "/" + fileName;
	stbi_set_flip_vertically_on_load(1); // posible to remove cause of assimp do the same
	return stbi_load(fullFileName.c_str(), &width, &height, &BPP, 4);
}

/*-------------- bones--------------*/
void Model::SetVertexBoneDataToDefault(Vertex &vertex)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.m_BoneIDs[i] = -1;
		vertex.m_Weights[i] = 0.0f;
	}
}

void Model::SetVertexBoneData(Vertex &vertex, int boneId, float weight)
{
	bool found = false;
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		if (vertex.m_BoneIDs[i] < 0)
		{
			vertex.m_Weights[i] = weight;
			vertex.m_BoneIDs[i] = boneId;
			found = true;
			break;
		}
	}
	if (found == false)
		std::cout << "4 bones for vertex is not enough\n";
}

void Model::ExtractBoneWeightForVertices(std::vector<Vertex> &vertices, aiMesh *mesh, const aiScene *scene)
{
	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneId = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (m_BoneInfoMap.count(boneName) == 0) // new bone
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = m_BoneCounter;
			newBoneInfo.offset = GLM_utils::convertMatrixToGLMFormat(
				mesh->mBones[boneIndex]->mOffsetMatrix);
			m_BoneInfoMap[boneName] = newBoneInfo;
			boneId = m_BoneCounter;
			m_BoneCounter++;
		}
		else
		{
			boneId = m_BoneInfoMap[boneName].id;
		}
		if (boneId == -1)
		{
			std::cout << "error boneId for " << boneName << std::endl;
			;
			return;
		}
		// assert(boneId != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;
		for (int weightIndex = 0; weightIndex < numWeights; weightIndex++)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			// assert(vertexId < vertices.size());
			SetVertexBoneData(vertices[vertexId], boneId, weight); // bind bone (id) and weight to vertex
		}
	}
}

void Model::Draw()
{
	nrg::mat4 model(1.0f);

	// std::cout << m_modelState.translation.x << " " << m_modelState.translation.y << " " << m_modelState.translation.z << std::endl;
	model = nrg::translate(model, m_modelState.translation); // move model. rotate have to be after move
	model = nrg::rotate(model, nrg::radians(m_modelState.rotation_x), nrg::vec3(0.0f, 1.0f, 0.0f));
	model = nrg::rotate(model, nrg::radians(m_modelState.rotation_y), nrg::vec3(1.0f, 0.0f, 0.0f));
	model = nrg::scale(model, m_modelState.scale);
	// m_view = nrg::translate(nrg::mat4(1.0f), view_vec);
	// model = nrg::scale(model, nrg::vec3(.07f, .07f, .07f));

	nrg::mat4 mvp = m_proj * m_view * model;
	m_shader->bind();
	m_shader->setUniformMat4f("u_model", model);
	m_shader->setUniformMat4f("u_proj", m_proj);
	m_shader->setUniformMat4f("u_view", m_view);
	m_shader->setUniform1i("u_hasAnimation", (hasAnimation ? 1 : 0));

	if (hasAnimation)
	{
		for (int i = 0; i < transforms.size(); ++i)
			m_shader->setUniformMat4f("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
	}
// debug bones
#ifdef debug
	m_shader->setUniform1i("DisplayBoneIndex", m_modelState.boneShowId);
#endif
	if (!m_modelState.fill_model)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_LINE
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_LINE
	for (auto &mesh : meshes)
	{
		mesh.Draw(*m_shader);
	}
}

/// mesh stuff
void Mesh::setupMesh()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	m_VAO = std::make_unique<VertexArray>();
	m_VBO = std::make_unique<VertexBuffer>(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));

	VertexBufferLayout layouts;
	layouts.push<float>(3);														// pos
	layouts.push<float>(2);														// tex coords
	layouts.push<float>(3);														// normals coords
	layouts.push<float>(3);														// tangent
	layouts.push<float>(3);														// bitangent
	layouts.push<int>(1 * MAX_BONE_INFLUENCE);									// m_BoneIDs
	layouts.push<float>(1 * MAX_BONE_INFLUENCE);								// m_Weights
	m_VAO->addBuffer(*m_VBO, layouts, m_Vertices.size() * layouts.getStride()); // no mtl
	m_IBO = std::make_unique<IndexBuffer>(m_Indices.data(), m_Indices.size());
}

void Mesh::Draw(Shader &shader)
{
	m_VAO->bind();
	int texSlot = 0;
	if (hasColor)
	{
		shader.setUniformVec3("u_mesh_color", {mesh_color.x, mesh_color.y, mesh_color.z});
		shader.setUniform1i("u_mesh_color_flag", 1);
	}
	else 
	{
		shader.setUniform1i("u_mesh_color_flag", 0);
		for (auto &text : m_Textures)
		{
			glActiveTexture(GL_TEXTURE0 + texSlot);
			glBindTexture(GL_TEXTURE_2D, text.id);
			shader.setUniform1i(text.type, texSlot);
			texSlot++;
		}
	}
	glDrawElements(GL_TRIANGLES, m_IBO->getCount(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}
