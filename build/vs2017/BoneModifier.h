#pragma once

#include <map>
#include <unordered_map>
#include "graphics/skinned_mesh_instance.h"

class BoneModifier
{
private:

	BoneModifier();
	~BoneModifier();

	gef::SkinnedMeshInstance* player_pointer_;
	std::unordered_map<std::string, int> bone_name_number_;

public:

	void Init(gef::SkinnedMeshInstance* player_);
	void Update();

};

