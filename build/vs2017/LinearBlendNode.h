#pragma once
#include "BlendNode.h"
#include "graphics/skinned_mesh_instance.h"
class LinearBlendNode : public BlendNode
{
public:
	LinearBlendNode();
	~LinearBlendNode();
	void init(gef::SkinnedMeshInstance* player_);

	float blendValue;

private:
	bool Process(float dt) override;

	gef::SkinnedMeshInstance* temp;
};

