#pragma once
#include <graphics/skinned_mesh_instance.h>
#include <animation/skeleton.h>
#include <graphics/scene.h>
#include <graphics/renderer_3d.h>
#include <system/platform.h>
#include <motion_clip_player.h>

class AnimatedSkeleton
{
public:
	AnimatedSkeleton();
	~AnimatedSkeleton();

	void init(gef::Scene* model_scene_, gef::SkinnedMeshInstance** player_, gef::Mesh* mesh_, MotionClipPlayer* anim_player_, MotionClipPlayer* anim_player_2);
	void SkeletonBlend(gef::SkinnedMeshInstance** player_, MotionClipPlayer* anim_player_, MotionClipPlayer* anim_player_2);
	gef::Skeleton* GetFirstSkeleton(gef::Scene* scene);

	float GetBlendValue() { return blendValue; };
	void SetBlendValue(float valueAlter) { blendValue = valueAlter; };
	void ChangeBlendValue(float valueAlter) { blendValue += valueAlter; };

private:
	gef::Skeleton* model_skeleton_;
	gef::SkeletonPose blended_pose_;
	float blendValue;
};

