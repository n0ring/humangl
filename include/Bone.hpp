#pragma once

#include <vector>
#include <iostream>
#include "GLM_utils.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assert.h>


// #define nrg glm

// #include "nrg_math.hpp"

struct KeyPosition
{
	nrg::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	nrg::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	nrg::vec3 scale;
	float timeStamp;
};

class Bone
{
public:
	Bone(const std::string &name, int ID, const aiNodeAnim *channel);
	void update(float animationTime);
	nrg::mat4 GetLocalTransform() { return m_LocalTransform; }
	std::string GetBoneName() const { return m_name; }
	int GetBoneID() { return m_id; }
	int GetPositionIndex(float animationTime);
	int GetRotationIndex(float animationTime);
	int GetScaleIndex(float animationTime);

private:
	std::vector<KeyPosition> m_positions;
	std::vector<KeyRotation> m_rotations;
	std::vector<KeyScale> m_scales;

	int m_numPositions;
	int m_numRotations;
	int m_numScales;

	nrg::mat4 m_LocalTransform;
	std::string m_name;
	int m_id;
	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	nrg::mat4 InterpolatePosition(float animationTime);
	nrg::mat4 InterpolateRotation(float animationTime);
	nrg::mat4 InterpolateScaling(float animationTime);
};
