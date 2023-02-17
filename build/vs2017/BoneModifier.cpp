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

	bone_name_number_.emplace(0, "hips");               //add 1 to all index positions when using as root was excluded
	bone_name_number_.emplace(1, "leftUpperLeg");
	bone_name_number_.emplace(2, "rightUpperLeg");
	bone_name_number_.emplace(3, "torso");
	bone_name_number_.emplace(4, "leftLowerLeg");
	bone_name_number_.emplace(5, "leftHeel");
	bone_name_number_.emplace(6, "leftToes");
	bone_name_number_.emplace(7, "rightLowerLeg");
	bone_name_number_.emplace(8, "rightHeel");
	bone_name_number_.emplace(9, "rightToes");
	bone_name_number_.emplace(10, "lowerChest");
	bone_name_number_.emplace(11, "upperChest");
	bone_name_number_.emplace(12, "leftShoulder");
	bone_name_number_.emplace(13, "neck");
	bone_name_number_.emplace(14, "rightShoulder");
	bone_name_number_.emplace(15, "leftUpperArm");
	bone_name_number_.emplace(16, "leftLowerArm");
	bone_name_number_.emplace(17, "leftHand");
	bone_name_number_.emplace(18, "leftIndexBase");
	bone_name_number_.emplace(19, "leftMiddleBase");
	bone_name_number_.emplace(20, "leftPinkyBase");
	bone_name_number_.emplace(21, "leftRingBase");
	bone_name_number_.emplace(22, "leftThumbBase");
	bone_name_number_.emplace(23, "leftIndexJoint");
	bone_name_number_.emplace(24, "leftIndexTip");
	bone_name_number_.emplace(25, "leftMiddleJoint");
	bone_name_number_.emplace(26, "leftMiddleTip");
	bone_name_number_.emplace(27, "leftPinkyJoint");
	bone_name_number_.emplace(28, "leftPinkyTip");
	bone_name_number_.emplace(29, "leftRingJoint");
	bone_name_number_.emplace(30, "leftRingTip");
	bone_name_number_.emplace(31, "leftThumbJoint");
	bone_name_number_.emplace(32, "leftThumbTip");
	bone_name_number_.emplace(33, "head");
	bone_name_number_.emplace(34, "rightUpperArm");
	bone_name_number_.emplace(35, "rightLowerArm");
	bone_name_number_.emplace(36, "rightHand");
	bone_name_number_.emplace(37, "rightIndexBase");
	bone_name_number_.emplace(38, "rightMiddleBase");
	bone_name_number_.emplace(39, "rightPinkyBase");
	bone_name_number_.emplace(40, "rightRingBase");
	bone_name_number_.emplace(41, "rightThumbBase");
	bone_name_number_.emplace(42, "rightIndexJoint");
	bone_name_number_.emplace(43, "rightIndexTip");
	bone_name_number_.emplace(44, "rightMiddleJoint");
	bone_name_number_.emplace(45, "rightMiddleTip");
	bone_name_number_.emplace(46, "rightPinkyJoint");
	bone_name_number_.emplace(47, "rightPinkyTip");
	bone_name_number_.emplace(48, "rightRingJoint");
	bone_name_number_.emplace(49, "rightRingTip");
	bone_name_number_.emplace(50, "rightThumbJoint");
	bone_name_number_.emplace(51, "rightThumbTip");
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

void BoneModifier::ModifyBones(int bone_index_, gef::Vector4 translation, gef::Vector4 rotation, gef::Vector4 scale)
{
	int index = bone_index_ + 1;
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
