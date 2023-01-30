#include "AnimationManager.h"

AnimationManager::AnimationManager(gef::Platform& platform) : platformRef(platform),
								  model_(platform),
								  walk_anim_(NULL),
								  idle_anim_(NULL),
								  jump_anim_(NULL),
								  run_anim_(NULL)
{
}

AnimationManager::~AnimationManager()
{
	delete walk_anim_;
	walk_anim_ = NULL;

	delete idle_anim_;
	idle_anim_ = NULL;

	delete jump_anim_;
	jump_anim_ = NULL;

	delete run_anim_;
	run_anim_ = NULL;
}

void AnimationManager::init(gef::Renderer3D* renderer_3d_)
{
	animationCounter = 0;

	// anims
	walk_anim_ = LoadAnimation("xbot/xbot@walking_inplace.scn", "");
	idle_anim_ = LoadAnimation("xbot/xbot@idle.scn", "");
	jump_anim_ = LoadAnimation("xbot/xbot@jump.scn", "");
	run_anim_ = LoadAnimation("xbot/xbot@running_inplace.scn", "");

	if (walk_anim_)
	{
		anim_player_.set_clip(walk_anim_);
		anim_player_.set_looping(true);
		anim_player_.set_anim_time(0.0f);
	}

	if (run_anim_)
	{
		anim_player_2.set_clip(run_anim_);
		anim_player_2.set_looping(true);
		anim_player_2.set_anim_time(0.0f);
	}

	if (walk_anim_ && run_anim_)
	{
		walk_speed_min = 1.f;
		walk_speed_max = walk_anim_->duration() / run_anim_->duration();

		run_speed_min = run_anim_->duration() / walk_anim_->duration();
		run_speed_max = 1.f;
	}

	model_.SetRenderer(renderer_3d_);
	skeleton_.init(model_.GetScene(), model_.GetInstance(), model_.GetMesh(), &anim_player_, &anim_player_2);
}

void AnimationManager::Update(float dt)
{
	model_.Update(dt, &anim_player_, &anim_player_2);

	float walkRunScaledTime = skeleton_.GetBlendValue();
	float walk_speed_mod = (walk_speed_max - walk_speed_min) * walkRunScaledTime;
	float run_speed_mod = (run_speed_max - run_speed_min) * walkRunScaledTime;
	anim_player_.set_playback_speed(walk_speed_min + walk_speed_mod);
	anim_player_2.set_playback_speed(run_speed_min + run_speed_mod);

	skeleton_.SkeletonBlend(model_.GetInstance(), &anim_player_, &anim_player_2);
}

void AnimationManager::SwitchAnimation(bool animationSwitch)
{
	if (animationSwitch)
	{
		if (animationCounter > 2)
		{
			animationCounter = 0;
		}
		else
		{
			animationCounter++;
		}

		switch (animationCounter)
		{
		case 0:
			if (walk_anim_)
			{
				anim_player_.set_clip(walk_anim_);
				//anim_player_.set_looping(true);
				anim_player_.set_anim_time(0.0f);
			}
			break;
		case 1:
			if (idle_anim_)
			{
				anim_player_.set_clip(idle_anim_);
				//anim_player_.set_looping(true);
				anim_player_.set_anim_time(0.0f);
			}
			break;
		case 2:
			if (jump_anim_)
			{
				anim_player_.set_clip(jump_anim_);
				//anim_player_.set_looping(true);
				anim_player_.set_anim_time(0.0f);
			}
			break;
		case 3:
			if (run_anim_)
			{
				anim_player_.set_clip(run_anim_);
				//anim_player_.set_looping(true);
				anim_player_.set_anim_time(0.0f);
			}
			break;
		}
	}
}

void AnimationManager::Render()
{
	model_.Render();
}

gef::Animation* AnimationManager::LoadAnimation(const char* anim_scene_filename, const char* anim_name)
{
	gef::Animation* anim = NULL;

	gef::Scene anim_scene;
	if (anim_scene.ReadSceneFromFile(platformRef, anim_scene_filename))
	{
		// if the animation name is specified then try and find the named anim
		// otherwise return the first animation if there is one
		std::map<gef::StringId, gef::Animation*>::const_iterator anim_node_iter;
		if (anim_name)
			anim_node_iter = anim_scene.animations.find(gef::GetStringId(anim_name));
		else
			anim_node_iter = anim_scene.animations.begin();

		if (anim_node_iter != anim_scene.animations.end())
			anim = new gef::Animation(*anim_node_iter->second);
	}

	return anim;
}
