#pragma once 
#include "nrg_math.hpp"

#include "glm_inc.hpp"

#define MOVE_STEP 1.0f
#define SCALE_STEP 0.01f
#define MOVE_MODEL_STEP 0.1f
// #define MOVE_MODEL_STEP 0.1f


#define CUR_ANIM_IDLE 0
#define CUR_ANIM_JUMP 1
#define CUR_ANIM_WALK 2
#define CUR_ANIM_KICK 3

enum class RenderMode
{
	COLOR,
	TEXTURE
};


struct ModelState
{
	float rotation_x = 0.001f;
	float rotation_y = 0.001f;
	nrg::vec3 scale;
	bool fill_model = false;
	nrg::vec3 translation;
	RenderMode renderMode = RenderMode::COLOR;
	bool hasNormals = false;
	nrg::vec3 lightPos;
	bool lightOn = false;

	int boneShowId = 0;
	int boneCount = 0;

	int m_currentAnimation = CUR_ANIM_IDLE;
	bool m_animationChange = false;

	ModelState() : scale(1.0f, 1.0f, 1.0f), 
		translation(nrg::vec3(0.5f)), 
		lightPos(0.0f, 10.0f, 3.0f)
		{}

	void moveX(float val)
	{
		rotation_x += val;
		if (rotation_x >= 360)
			rotation_x = 0;
		else if (rotation_x <= 0)
			rotation_x = 360;
	}

	void moveY(float val)
	{
		rotation_y += val;
		if (rotation_y >= 360)
			rotation_y = 0;
		else if (rotation_y <= 0)
			rotation_y = 360;
	}

	void moveUp()
	{
		moveY(MOVE_STEP);
	}
	void moveDown()
	{
		moveY(-MOVE_STEP);
	}
	void moveLeft()
	{
		moveX(-MOVE_STEP);
	}
	void moveRight()
	{
		moveX(MOVE_STEP);
	}

	void increaseScale()
	{
		scale.z += SCALE_STEP;
		scale.x += SCALE_STEP;
		scale.y += SCALE_STEP;
		if (scale.x > 2.5f)
		{
			scale.z = 2.5f;
			scale.x = 2.5f;
			scale.y = 2.5f;
		}
	}

	void decreaseScale()
	{
		scale.z -= SCALE_STEP;
		scale.x -= SCALE_STEP;
		scale.y -= SCALE_STEP;
		if (scale.z < 0)
		{
			scale.z = 0;
			scale.x = 0;
			scale.y = 0;
		}
	}

	void moveModelUp()
	{
			translation.y += MOVE_MODEL_STEP;
	}
	void moveModelDown()
	{
			translation.y -= MOVE_MODEL_STEP;
	}

	void moveModelRight()
	{
			translation.x += MOVE_MODEL_STEP;
	}
	void moveModelLeft()
	{
			translation.x -= MOVE_MODEL_STEP;
	}

	void moveModelClose()
	{
			translation.z -= MOVE_MODEL_STEP;
	}

	void moveModelFar()
	{
			translation.z += MOVE_MODEL_STEP;
	}

	void toggleTexColor()
	{
		renderMode = renderMode == RenderMode::COLOR ? RenderMode::TEXTURE : RenderMode::COLOR;
	}

	void lumos()
	{
		lightOn = true;
	}

	void nox()
	{
		lightOn = false;
	}

	void setElder()
	{
		translation = nrg::vec3(4.2f, -0.5f, 2.1f);
		scale = nrg::vec3(0.51f, 0.51f, 0.51f);
		rotation_x = 79.001f;
		rotation_y = 65.001f;
		renderMode = RenderMode::TEXTURE;
	}

	void showNextBone()
	{
		boneShowId = (boneShowId + 1) % boneCount;
	}
	void changeAnimToWalk()
	{
		m_currentAnimation = CUR_ANIM_WALK;
	}

	void changeAnimToJump()
	{
		#ifndef debug
		m_currentAnimation = CUR_ANIM_JUMP;
		#endif
	}

	void setAnimationChangeStatus(bool status)
	{
		m_animationChange = status;
	}

	void changeAnimToKick()
	{
		m_currentAnimation = CUR_ANIM_KICK;
	}
};