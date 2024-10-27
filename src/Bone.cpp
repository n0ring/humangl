#include "Bone.hpp"




Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
	: m_name(name), m_id(ID), m_LocalTransform(1.0f)
{
	m_numPositions = channel->mNumPositionKeys;
	// std::cout << "m_numPositions for bone " << name << " " << m_numPositions << std::endl;
	for (int positionIndex = 0; positionIndex < m_numPositions; ++positionIndex)
	{
		KeyPosition data;
		aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
		float timeStamp = channel->mPositionKeys[positionIndex].mTime;
		GLM_utils::setGLMVector(data.position, aiPosition);
		data.timeStamp = timeStamp;
		m_positions.push_back(data);
	}

	m_numRotations = channel->mNumRotationKeys;
	// std::cout << "m_numRotations for bone " << name << " " << m_numRotations << std::endl;
	for (int rotationIndex = 0; rotationIndex < m_numRotations; ++rotationIndex)
	{
		aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
		float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
		KeyRotation data;
		data.orientation = GLM_utils::GetGLMQuat(aiOrientation);
		data.timeStamp = timeStamp;
		m_rotations.push_back(data);
	}

	m_numScales = channel->mNumScalingKeys;
	for (int keyIndex = 0; keyIndex < m_numScales; ++keyIndex)
	{
		KeyScale data;
		aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
		float timeStamp = channel->mScalingKeys[keyIndex].mTime;
		GLM_utils::setGLMVector(data.scale, scale);
		data.timeStamp = timeStamp;
		m_scales.push_back(data);
	}
}

void Bone::update(float animationTime)
{
	nrg::mat4 translation = InterpolatePosition(animationTime);
	nrg::mat4 rotation = InterpolateRotation(animationTime);
	nrg::mat4 scale = InterpolateScaling(animationTime);
	m_LocalTransform = translation * rotation * scale;
}

/* Gets the current index on mKeyPositions to interpolate to based on 
the current animation time*/
int Bone::GetPositionIndex(float animationTime)
{
	for (int index = 0; index < m_numPositions - 1; ++index)
	{
		if (animationTime < m_positions[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

/* Gets the current index on mKeyRotations to interpolate to based on the 
current animation time*/
int Bone::GetRotationIndex(float animationTime)
{
	for (int index = 0; index < m_numRotations - 1; ++index)
	{
		if (animationTime < m_rotations[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

/* Gets the current index on mKeyScalings to interpolate to based on the 
current animation time */
int Bone::GetScaleIndex(float animationTime)
{
	for (int index = 0; index < m_numScales - 1; ++index)
	{
		if (animationTime < m_scales[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

/* Gets normalized value for Lerp & Slerp*/
float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

/*figures out which position keys to interpolate b/w and performs the interpolation 
and returns the translation matrix*/
nrg::mat4 Bone::InterpolatePosition(float animationTime)
{
	if (1 == m_numPositions)
		return nrg::translate(nrg::mat4(1.0f), m_positions[0].position);

	int p0Index = GetPositionIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(m_positions[p0Index].timeStamp,
		m_positions[p1Index].timeStamp, animationTime);
	nrg::vec3 finalPosition = nrg::mix(m_positions[p0Index].position,
		m_positions[p1Index].position, scaleFactor);
	return nrg::translate(nrg::mat4(1.0f), finalPosition);
}

/*figures out which rotations keys to interpolate b/w and performs the interpolation 
and returns the rotation matrix*/
nrg::mat4 Bone::InterpolateRotation(float animationTime)
{
	if (1 == m_numRotations)
	{
		auto rotation = nrg::normalize(m_rotations[0].orientation);
		return nrg::toMat4(rotation);
	}

	int p0Index = GetRotationIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(m_rotations[p0Index].timeStamp,
		m_rotations[p1Index].timeStamp, animationTime);
	nrg::quat finalRotation = nrg::slerp(m_rotations[p0Index].orientation,
		m_rotations[p1Index].orientation, scaleFactor);
	finalRotation =  nrg::normalize(finalRotation);
	// return finalRotation.toMatrix();
	return nrg::toMat4(finalRotation);
}


/*figures out which scaling keys to interpolate b/w and performs the interpolation 
and returns the scale matrix*/
nrg::mat4 Bone::InterpolateScaling(float animationTime)
{
	if (1 == m_numScales)
		return nrg::scale(nrg::mat4(1.0f), m_scales[0].scale);

	int p0Index = GetScaleIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(m_scales[p0Index].timeStamp,
		m_scales[p1Index].timeStamp, animationTime);
	nrg::vec3 finalScale = nrg::mix(m_scales[p0Index].scale, m_scales[p1Index].scale
		, scaleFactor);
	return nrg::scale(nrg::mat4(1.0f), finalScale);
}




