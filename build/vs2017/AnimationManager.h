#pragma once
#include <animation/animation.h>
#include <motion_clip_player.h>

#include "AnimatedModel.h"
#include "AnimatedSkeleton.h"

class AnimationManager
{
public:
	AnimationManager(gef::Platform& platform);
	~AnimationManager();
	void init(gef::Renderer3D* renderer_3d_);
	void Update(float dt);
	void SwitchAnimation(bool animationSwitch);
	void Render();

	float GetWalkSpeed(bool isMin) { if (isMin) { return walk_speed_min; } else { return walk_speed_max; } };
	float GetRunSpeed(bool isMin) { if (isMin) { return run_speed_min; } else { return run_speed_max; } };

	AnimatedModel* GetModel() { return &model_; };
	AnimatedSkeleton* GetSkeleton() { return &skeleton_; };

private:
	gef::Animation* LoadAnimation(const char* anim_scene_filename, const char* anim_name);

	AnimatedModel model_;
	AnimatedSkeleton skeleton_;

	int animationCounter;
	float walk_speed_min, walk_speed_max;
	float run_speed_min, run_speed_max;

	MotionClipPlayer anim_player_;
	MotionClipPlayer anim_player_2;
	gef::Animation* walk_anim_;
	gef::Animation* idle_anim_;
	gef::Animation* jump_anim_;
	gef::Animation* run_anim_;

	gef::Platform& platformRef;
};

