#include "Animator.hpp"

Animator::Animator(Animation *base) : m_CurrentAnimation(nullptr)
{
	m_CurrentTime = 0.0;
	m_BaseAnimation = base;
	m_CurrentAnimation = base;

	m_FinalBoneMatrices.resize(100, 1.0f);
}

void Animator::UpdateAnimation(float dt)
{
	m_DeltaTime = dt;
	if (m_CurrentAnimation)
	{
		m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
		
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
		CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), nrg::mat4(1.0f));
	}
}

void Animator::PlayAnimation(Animation *pAnimation)
{
	if (pAnimation != m_CurrentAnimation)
	{
		m_CurrentAnimation = pAnimation;
		m_CurrentTime = 0.0f;
	}
}

void Animator::CalculateBoneTransform(const AssimpNodeData *node, nrg::mat4 parentTransform)
{
	if (node == NULL || m_CurrentAnimation == NULL)
		return ;

	std::string nodeName = node->name;
	nrg::mat4 nodeTransform = node->transformation;
	Bone *Bone = m_CurrentAnimation->FindBone(nodeName);

	if (Bone)
	{
		Bone->update(m_CurrentTime);
		nodeTransform = Bone->GetLocalTransform();
	}

	nrg::mat4 globalTransformation = parentTransform * nodeTransform;
	// GLM_utils::printMatrix(globalTransformation);
	auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();

	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		nrg::mat4 offset = boneInfoMap[nodeName].offset;
		m_FinalBoneMatrices[index] = m_CurrentAnimation->m_globalTransform * globalTransformation * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
		CalculateBoneTransform(&node->children[i], globalTransformation);
	
}