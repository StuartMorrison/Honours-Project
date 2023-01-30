#pragma once
#include "BlendNode.h"
class ClipNode : public BlendNode
{
public:
	ClipNode();
	~ClipNode();
	void init(gef::SkeletonPose bindPose);
	void SetClip(gef::Animation* inputAnimation) { clip = inputAnimation; };

	float animTime;
	bool isLooping;
	float playbackSpeed;

protected:
	gef::Animation* clip;
	gef::SkeletonPose bind_pose_;
	bool Process(float dt) override;
};

