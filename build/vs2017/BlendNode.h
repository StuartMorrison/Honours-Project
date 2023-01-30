#pragma once
#include <vector>
#include <animation/skeleton.h>
#include <animation/animation.h>
class BlendNode
{
public:
	BlendNode();
	~BlendNode();
	bool Update(float dt);
	virtual void SetInput(int inputNum, BlendNode* newInput);

	std::vector<BlendNode*> inputs;
	gef::SkeletonPose output;

protected:
	virtual bool Process(float dt) = 0;
};

