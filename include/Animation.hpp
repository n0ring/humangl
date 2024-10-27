#pragma once

// #include <vector>
// #include <iostream>
// #include "glm_inc.hpp"

// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>
#include "Model.hpp"
#include "Bone.hpp"
#include <map>

// #define nrg glm glm

struct AssimpNodeData
{
	nrg::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation
{
public:
	Animation() = default;
	Animation(const std::string &animationPath, Model *model);

	inline float GetTicksPerSecond() { return m_TicksPerSecond; }
	inline float GetDuration() { return m_Duration; }
	inline const AssimpNodeData &GetRootNode() { return m_RootNode; }
	inline const std::map<std::string, BoneInfo> &GetBoneIDMap() { return m_BoneInfoMap; }
	Bone *FindBone(const std::string &name);


	nrg::mat4 m_globalTransform;
private:
	float m_Duration;
	int m_TicksPerSecond;
	std::vector<Bone> m_Bones;
	AssimpNodeData m_RootNode;
	std::map<std::string, BoneInfo> m_BoneInfoMap;

	void ReadMissingBones(const aiAnimation *animation, Model &model);
	void ReadHeirarchyData(AssimpNodeData &dest, const aiNode *src);

};