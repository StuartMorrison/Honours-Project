#pragma once

#include <map>
#include <unordered_map>
#include "graphics/skinned_mesh_instance.h"

class BoneModifier
{
private:

	gef::SkinnedMeshInstance* player_pointer_;
	std::unordered_map<int, std::string> bone_name_number_;
	int emotion_number_;

public:

	BoneModifier();
	~BoneModifier();

	void Init(gef::SkinnedMeshInstance* player_);
	void ChangeEmotion(int input);
	std::string GetMap(int index) { return bone_name_number_.at(index); };
	void ModifyBones(int bone_index_, gef::Vector4 translation, gef::Vector4 rotation, gef::Vector4 scale);
	void HappyInit();
	void SadInit();
	void AngryInit();
	void ScaredInit();
};

