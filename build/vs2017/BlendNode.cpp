#include "BlendNode.h"

BlendNode::BlendNode()
{
}

BlendNode::~BlendNode()
{
	/*for(int x = 0; x < inputs.size(); x++)
	{
		delete inputs[x];
		inputs[x] = nullptr;
	}*/
}

bool BlendNode::Update(float dt)
{
	//initialise check that all inputs are valid
	bool all_inputs_valid = true;

	for(int input_num = 0; input_num < inputs.size(); input_num++)
	{
		//recursively call BlendNode::Update to perform depth 1st traversal
		bool input_valid = false;
		if(inputs[input_num])
		{
			input_valid = inputs[input_num]->Update(dt);
		}

		//check the output of each input node is valid
		if(!input_valid)
		{
			all_inputs_valid = false;
		}
	}

	//post oder processing starts here
	bool output_valid = false;

	//only process this node if all inputs are valid
	if(all_inputs_valid)
	{
		output_valid = Process(dt);
	}
	return output_valid;
}

void BlendNode::SetInput(int inputNum, BlendNode* newInput)
{
	if(inputs.size() > inputNum)
	{
		inputs[inputNum] = newInput;
	}
	else
	{
		inputs.push_back(newInput);
	}
}
