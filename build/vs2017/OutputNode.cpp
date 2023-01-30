#include "OutputNode.h"

OutputNode::OutputNode()
{
}

OutputNode::~OutputNode()
{
}

void OutputNode::init(gef::SkeletonPose bindPose)
{
    output = bindPose;
}

bool OutputNode::Process(float dt)
{
    if(inputs.size() > 0)
    {
        output = inputs[0]->output;
    }
    return false;
}
