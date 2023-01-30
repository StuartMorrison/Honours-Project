#include "AnimatedSkeleton.h"

AnimatedSkeleton::AnimatedSkeleton()
{
}

AnimatedSkeleton::~AnimatedSkeleton()
{
}

void AnimatedSkeleton::init(gef::Scene* model_scene_, gef::SkinnedMeshInstance** player_, gef::Mesh* mesh_, MotionClipPlayer* anim_player_, MotionClipPlayer* anim_player_2)
{
	blendValue = 0.0f;

	// get the first skeleton in the scene
	model_skeleton_ = GetFirstSkeleton(model_scene_);

	if (model_skeleton_)
	{
		*player_ = new gef::SkinnedMeshInstance(*model_skeleton_);
		gef::SkinnedMeshInstance* temp = *player_;
		anim_player_->Init(temp->bind_pose());
		anim_player_2->Init(temp->bind_pose());
		temp->set_mesh(mesh_);
		blended_pose_ = temp->bind_pose();
		temp->UpdateBoneMatrices(blended_pose_);
	}
}

void AnimatedSkeleton::SkeletonBlend(gef::SkinnedMeshInstance** player_, MotionClipPlayer* anim_player_, MotionClipPlayer* anim_player_2)
{
	gef::SkinnedMeshInstance* temp = *player_;
	blended_pose_.Linear2PoseBlend(anim_player_->pose(), anim_player_2->pose(), blendValue);
	temp->UpdateBoneMatrices(blended_pose_);
}

gef::Skeleton* AnimatedSkeleton::GetFirstSkeleton(gef::Scene* scene)
{
	gef::Skeleton* skeleton = NULL;
	if (scene)
	{
		// check to see if there is a skeleton in the the scene file
		// if so, pull out the bind pose and create an array of matrices
		// that wil be used to store the bone transformations
		if (scene->skeletons.size() > 0)
			skeleton = scene->skeletons.front();
	}

	return skeleton;
}
