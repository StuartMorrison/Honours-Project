#include "ClipNode.h"

ClipNode::ClipNode()
{
	animTime = 0.0f;
	isLooping = true;
	playbackSpeed = 1.0f;
}

ClipNode::~ClipNode()
{
}

void ClipNode::init(gef::SkeletonPose bindPose)
{
	bind_pose_ = bindPose;
	output = bindPose;
}

bool ClipNode::Process(float dt)
{
	bool finished = false;

	if (clip)
	{
		// update the animation playback time
		animTime += dt * playbackSpeed;

		// check to see if the playback has reached the end of the animation
		if (animTime > clip->duration())
		{
			// if the animation is looping then wrap the playback time round to the beginning of the animation
			// other wise set the playback time to the end of the animation and flag that we have reached the end
			if (isLooping)
				animTime = std::fmodf(animTime, clip->duration());
			else
			{
				animTime = clip->duration();
				finished = true;
			}
		}

		// add the clip start time to the playback time to calculate the final time
		// that will be used to sample the animation data
		float time = animTime + clip->start_time();

		// sample the animation data at the calculated time
		// any bones that don't have animation data are set to the bind pose
		output.SetPoseFromAnim(*clip, bind_pose_, time);
	}
	else
	{
		// no animation associated with this player
		// just set the pose to the bind pose
		output = bind_pose_;
	}

	// return true if we have reached the end of the animation, always false when playback is looped
	return !finished;
}
