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
	int emotion_number_;

public:

	void Init(gef::SkinnedMeshInstance* player_);
	void ChangeEmotion(int input);
	void ModifyBones(std::string bone_name_, gef::Vector4 translation, gef::Vector4 rotation, gef::Vector4 scale);
	void HappyInit();
	void SadInit();
	void AngryInit();
	void ScaredInit();
};

