#include "BoneModifier.h"

BoneModifier::BoneModifier()
{
	player_pointer_ = nullptr;
}

BoneModifier::~BoneModifier()
{
	player_pointer_ = nullptr;
}

void BoneModifier::Init(gef::SkinnedMeshInstance* player_)
{
	player_pointer_ = player_;
}

void BoneModifier::Update()
{

}
