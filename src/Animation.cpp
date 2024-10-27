#include "Animation.hpp"
float Determinant(nrg::mat4& m)
{
        return m[0][0]*m[1][1]*m[2][2]*m[3][3] - m[0][0]*m[1][1]*m[2][3]*m[3][2] + m[0][0]*m[1][2]*m[2][3]*m[3][1] - m[0][0]*m[1][2]*m[2][1]*m[3][3]
                + m[0][0]*m[1][3]*m[2][1]*m[3][2] - m[0][0]*m[1][3]*m[2][2]*m[3][1] - m[0][1]*m[1][2]*m[2][3]*m[3][0] + m[0][1]*m[1][2]*m[2][0]*m[3][3]
                - m[0][1]*m[1][3]*m[2][0]*m[3][2] + m[0][1]*m[1][3]*m[2][2]*m[3][0] - m[0][1]*m[1][0]*m[2][2]*m[3][3] + m[0][1]*m[1][0]*m[2][3]*m[3][2]
                + m[0][2]*m[1][3]*m[2][0]*m[3][1] - m[0][2]*m[1][3]*m[2][1]*m[3][0] + m[0][2]*m[1][0]*m[2][1]*m[3][3] - m[0][2]*m[1][0]*m[2][3]*m[3][1]
                + m[0][2]*m[1][1]*m[2][3]*m[3][0] - m[0][2]*m[1][1]*m[2][0]*m[3][3] - m[0][3]*m[1][0]*m[2][1]*m[3][2] + m[0][3]*m[1][0]*m[2][2]*m[3][1]
                - m[0][3]*m[1][1]*m[2][2]*m[3][0] + m[0][3]*m[1][1]*m[2][0]*m[3][2] - m[0][3]*m[1][2]*m[2][0]*m[3][1] + m[0][3]*m[1][2]*m[2][1]*m[3][0];
}

nrg::mat4 inverse(nrg::mat4& m)
{
        // Compute the reciprocal determinant
        float det = Determinant(m);

        if(det == 0.0f)
        {
            assert(0);
            return m;
        }

        float invdet = 1.0f / det;

        nrg::mat4 res;
        res[0][0] = invdet  * (m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] *
                                 (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[1][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
        res[0][1] = -invdet * (m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] *
                                 (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[0][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
        res[0][2] = invdet  * (m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] *
                                 (m[1][3] * m[3][1] - m[1][1] * m[3][3]) + m[0][3] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]));
        res[0][3] = -invdet * (m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] *
                                 (m[1][3] * m[2][1] - m[1][1] * m[2][3]) + m[0][3] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]));
        res[1][0] = -invdet * (m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] *
                                 (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
        res[1][1] = invdet  * (m[0][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] *
                                 (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
        res[1][2] = -invdet * (m[0][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] *
                                 (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][2] - m[1][2] * m[3][0]));
        res[1][3] = invdet  * (m[0][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] *
                                 (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]));
        res[2][0] = invdet  * (m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[1][1] *
                                 (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
        res[2][1] = -invdet * (m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[0][1] *
                                 (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
        res[2][2] = invdet  * (m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) + m[0][1] *
                                 (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
        res[2][3] = -invdet * (m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) + m[0][1] *
                                 (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
        res[3][0] = -invdet * (m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[1][1] *
                                 (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[1][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
        res[3][1] = invdet  * (m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[0][1] *
                                 (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[0][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
        res[3][2] = -invdet * (m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) + m[0][1] *
                                 (m[1][2] * m[3][0] - m[1][0] * m[3][2]) + m[0][2] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
        res[3][3] = invdet  * (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) + m[0][1] *
                                 (m[1][2] * m[2][0] - m[1][0] * m[2][2]) + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
        return res;
}
aiMatrix4x4 ConvertFBXtoDAE(aiMatrix4x4 matrix) {
    // Корректировка матрицы для приведения FBX к DAE
	std::cout << "do correction from fbx to dae matrix\n";
    aiMatrix4x4 rotation(1, 0, 0, 0,
                         0, 0, 1, 0,
                         0, -1, 0, 0,
                         0, 0, 0, 1);
    matrix = rotation * matrix;
	return matrix;
}

Animation::Animation(const std::string &animationPath, Model *model)
{
	std::cout << "Load animation file " << animationPath << std::endl;
	unsigned int assimp_flags = aiProcess_Triangulate
        | aiProcess_JoinIdenticalVertices
        | aiProcess_FlipUVs
        | aiProcess_GenSmoothNormals;
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(animationPath, assimp_flags);
	// const aiScene *scene = importer.ReadFile(animationPath, aiProcess_Triangulate | aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder);
	if (!scene || !scene->mRootNode)
	{
		std::cout << "Error load animation : " << animationPath << std::endl;
		return;
	}

	m_globalTransform =  GLM_utils::convertMatrixToGLMFormat(scene->mRootNode->mTransformation);
	m_globalTransform = inverse(m_globalTransform);
	// GLM_utils::printMatrix(m_globalTransform);
	assert(scene && scene->mRootNode);
	if (scene->mNumAnimations == 0)
	{
		std::cout << "No animation found\n";
		return ;
	}
	auto animation = scene->mAnimations[0];
	m_Duration = animation->mDuration;
	m_TicksPerSecond = animation->mTicksPerSecond;
	ReadHeirarchyData(m_RootNode, scene->mRootNode);
	ReadMissingBones(animation, *model);

}

Bone *Animation::FindBone(const std::string &name)
{
	auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
							 [&](const Bone &Bone)
							 {
								 return Bone.GetBoneName() == name;
							 });
	if (iter == m_Bones.end())
		return nullptr;
	else
		return &(*iter);
}

void Animation::ReadMissingBones(const aiAnimation *animation, Model &model)
{
	int size = animation->mNumChannels;

	std::map<std::string, BoneInfo> &boneInfoMap = model.getBonesInfoMap(); // getting m_BoneInfoMap from Model class
	int &boneCount = model.getBoneCounter();	 // getting the m_BoneCounter from Model class

	// reading channels(bones engaged in an animation and their keyframes)
	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		m_Bones.push_back(Bone(channel->mNodeName.data,
							   boneInfoMap[channel->mNodeName.data].id, channel));
	}

	m_BoneInfoMap = boneInfoMap;
}


void Animation::ReadHeirarchyData(AssimpNodeData &dest, const aiNode *src)
{
	assert(src);
	if (src == nullptr)
	{
		std::cout << "error in heirarchy\n";
	}
	dest.name = src->mName.data;
	
	// std::cout << dest.name << std::endl;
	dest.transformation = GLM_utils::convertMatrixToGLMFormat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;
	// GLM_utils::printMatrix(dest.transformation);
	for (int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHeirarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}