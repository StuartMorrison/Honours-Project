#include "LinearBlendNode.h"

LinearBlendNode::LinearBlendNode()
{
    blendValue = 0.5f;
}

LinearBlendNode::~LinearBlendNode()
{
    temp = nullptr;
}

void LinearBlendNode::init(gef::SkinnedMeshInstance* player_)
{
    temp = player_;
    output = temp->bind_pose();
}

bool LinearBlendNode::Process(float dt)
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
