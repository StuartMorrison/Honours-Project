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

	bone_name_number_.emplace("hips", 0);               //add 1 to all index positions when using as root was excluded
	bone_name_number_.emplace("leftUpperLeg", 1);
	bone_name_number_.emplace("rightUpperLeg", 2);
	bone_name_number_.emplace("torso", 3);
	bone_name_number_.emplace("leftLowerLeg", 4);
	bone_name_number_.emplace("leftHeel", 5);
	bone_name_number_.emplace("leftToes", 6);
	bone_name_number_.emplace("rightLowerLeg", 7);
	bone_name_number_.emplace("rightHeel", 8);
	bone_name_number_.emplace("rightToes", 9);
	bone_name_number_.emplace("lowerChest", 10);
	bone_name_number_.emplace("upperChest", 11);
	bone_name_number_.emplace("leftShoulder", 12);
	bone_name_number_.emplace("neck", 13);
	bone_name_number_.emplace("rightShoulder", 14);
	bone_name_number_.emplace("leftUpperArm", 15);
	bone_name_number_.emplace("leftLowerArm", 16);
	bone_name_number_.emplace("leftHand", 17);
	bone_name_number_.emplace("leftIndexBase", 18);
	bone_name_number_.emplace("leftMiddleBase", 19);
	bone_name_number_.emplace("leftPinkyBase", 20);
	bone_name_number_.emplace("leftRingBase", 21);
	bone_name_number_.emplace("leftThumbBase", 22);
	bone_name_number_.emplace("leftIndexJoint", 23);
	bone_name_number_.emplace("leftIndexTip", 24);
	bone_name_number_.emplace("leftMiddleJoint", 25);
	bone_name_number_.emplace("leftMiddleTip", 26);
	bone_name_number_.emplace("leftPinkyJoint", 27);
	bone_name_number_.emplace("leftPinkyTip", 28);
	bone_name_number_.emplace("leftRingJoint", 29);
	bone_name_number_.emplace("leftRingTip", 30);
	bone_name_number_.emplace("leftThumbJoint", 31);
	bone_name_number_.emplace("leftThumbTip", 32);
	bone_name_number_.emplace("head", 33);
	bone_name_number_.emplace("rightUpperArm", 34);
	bone_name_number_.emplace("rightLowerArm", 35);
	bone_name_number_.emplace("rightHand", 36);
	bone_name_number_.emplace("rightIndexBase", 37);
	bone_name_number_.emplace("rightMiddleBase", 38);
	bone_name_number_.emplace("rightPinkyBase", 39);
	bone_name_number_.emplace("rightRingBase", 40);
	bone_name_number_.emplace("rightThumbBase", 41);
	bone_name_number_.emplace("rightIndexJoint", 42);
	bone_name_number_.emplace("rightIndexTip", 43);
	bone_name_number_.emplace("rightMiddleJoint", 44);
	bone_name_number_.emplace("rightMiddleTip", 45);
	bone_name_number_.emplace("rightPinkyJoint", 46);
	bone_name_number_.emplace("rightPinkyTip", 47);
	bone_name_number_.emplace("rightRingJoint", 48);
	bone_name_number_.emplace("rightRingTip", 49);
	bone_name_number_.emplace("rightThumbJoint", 50);
	bone_name_number_.emplace("rightThumbTip", 51);
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
