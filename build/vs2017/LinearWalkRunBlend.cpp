#include "LinearWalkRunBlend.h"

LinearWalkRunBlend::LinearWalkRunBlend()
{
    blendValue = 0.5f;
}

LinearWalkRunBlend::~LinearWalkRunBlend()
{
    temp = nullptr;
}

void LinearWalkRunBlend::init(gef::SkinnedMeshInstance* player_)
{
    temp = player_;
    output = temp->bind_pose();
}

bool LinearWalkRunBlend::Process(float dt)
{
    if (temp)
    {
        if (inputs[0] && inputs[1])
        {
            output.Linear2PoseBlend(inputs[0]->output, inputs[1]->output, blendValue); //is the bad belnd rn for running and walking
            temp->UpdateBoneMatrices(output);
            return true;
        }
    }
    return false;
}