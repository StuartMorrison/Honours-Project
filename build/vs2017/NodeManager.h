#pragma once
#include "ClipNode.h"
#include "LinearBlendNode.h"
#include "OutputNode.h"
class NodeManager
{
public:
	NodeManager();
	~NodeManager();
	void init(gef::SkinnedMeshInstance* player_);

	void NewClipNode(gef::Animation* animation);
	void NewLinearBlendNode();
	void NewOutputNode();

	std::vector<ClipNode*> clip_nodes_;
	std::vector<LinearBlendNode*> linear_blend_nodes_;
	std::vector<OutputNode*> output_nodes_;

private:
	gef::SkinnedMeshInstance* player_ref_;
};

