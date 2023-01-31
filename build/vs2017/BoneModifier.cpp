#include "BoneModifier.h"

BoneModifier::BoneModifier()
{
	player_pointer_ = nullptr;
	emotion_number_ = 0;
}

BoneModifier::~BoneModifier()
{
	player_pointer_ = nullptr;
}

void BoneModifier::Init(gef::SkinnedMeshInstance* player_)
{
	player_pointer_ = player_;
}

void BoneModifier::ChangeEmotion(int input)
{
	emotion_number_ = input;
	switch (emotion_number_)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	}
}

void BoneModifier::ModifyBones(std::string bone_name_, gef::Vector4 translation, gef::Vector4 rotation, gef::Vector4 scale)
{
	int index = bone_name_number_.at(bone_name_);
	gef::Matrix44 originalValues = player_pointer_->bone_matrices()[index];
	gef::Matrix44 modifyValues;
	gef::Quaternion modifyRotation;
	modifyValues.Scale(originalValues.GetScale());
	modifyRotation = gef::Quaternion(rotation.x(), rotation.y(), rotation.z(), 1.0f);
	modifyRotation.Normalise();
	modifyValues.Rotation(modifyRotation);
	modifyValues.SetTranslation(translation);

	player_pointer_->bone_matrices()[index] = originalValues * modifyValues;
}
