#pragma once
#include "BlendNode.h"
class OutputNode : public BlendNode
{
public:
	OutputNode();
	~OutputNode();

	void init(gef::SkeletonPose bindPose);
protected:
	bool Process(float dt) override;
};

