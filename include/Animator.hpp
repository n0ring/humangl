#pragma once

#include "Animation.hpp"

class Animator
{
public:
	Animator(Animation *base);
	void UpdateAnimation(float dt);
	void PlayAnimation(Animation *pAnimation);
	void CalculateBoneTransform(const AssimpNodeData *node, nrg::mat4 parentTransform);
	std::vector<nrg::mat4> GetFinalBoneMatrices() { return m_FinalBoneMatrices; }
	bool isAnimationEnd(float dt)
	{ 
		if (m_CurrentAnimation)
		{
			return m_CurrentTime +  m_CurrentAnimation->GetTicksPerSecond() * dt > m_CurrentAnimation->GetDuration();
		}
		return false;
	}
private:
	std::vector<nrg::mat4> m_FinalBoneMatrices;
	Animation *m_CurrentAnimation;
	Animation *m_BaseAnimation;

	float m_CurrentTime;
	float m_DeltaTime;


};