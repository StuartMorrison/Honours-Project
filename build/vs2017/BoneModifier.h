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
	int GetEmotion();
	std::string GetMap(int index) { return bone_name_number_.at(index); };
	gef::Matrix44 ModifyBones(gef::Matrix44 originalMatrix, gef::Quaternion originalRotation, gef::Vector4 translation, gef::Quaternion rotation);
	gef::Matrix44 ModifyRotation(gef::Matrix44 originalMatrix, gef::Quaternion originalRotation, gef::Quaternion scalarValues);
};

