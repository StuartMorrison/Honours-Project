#include "NodeManager.h"

NodeManager::NodeManager()
{
}

NodeManager::~NodeManager()
{
	for (int x = 0; x < clip_nodes_.size(); x++)
	{
		delete clip_nodes_[x];
		clip_nodes_[x] = nullptr;
	}

	for (int y = 0; y < linear_blend_nodes_.size(); y++)
	{
		delete linear_blend_nodes_[y];
		linear_blend_nodes_[y] = nullptr;
	}

	for (int z = 0; z < output_nodes_.size(); z++)
	{
		delete output_nodes_[z];
		output_nodes_[z] = nullptr;
	}
}

void NodeManager::init(gef::SkinnedMeshInstance* player_)
{
	player_ref_ = player_;
}

void NodeManager::NewClipNode(gef::Animation* animation)
{
	clip_nodes_.push_back(new ClipNode);
	clip_nodes_.back()->init(player_ref_->bind_pose());
	clip_nodes_.back()->SetClip(animation);
}

void NodeManager::NewLinearBlendNode()
{
	linear_blend_nodes_.push_back(new LinearBlendNode);
	linear_blend_nodes_.back()->init(player_ref_);
}

void NodeManager::NewOutputNode()
{
	output_nodes_.push_back(new OutputNode);
	output_nodes_.back()->init(player_ref_->bind_pose());
}
