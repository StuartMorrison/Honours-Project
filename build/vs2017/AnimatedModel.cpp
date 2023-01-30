#include "AnimatedModel.h"
#include <graphics/mesh.h>

AnimatedModel::AnimatedModel(gef::Platform& platform) : platformRef(platform),
							mesh_(NULL),
							player_(NULL),
							model_scene_(NULL)
{
	init();
}

AnimatedModel::~AnimatedModel()
{
	delete player_;
	player_ = NULL;

	delete mesh_;
	mesh_ = NULL;

	delete model_scene_;
	model_scene_ = NULL;

	rendererRef = NULL;
}

void AnimatedModel::init()
{
	// create a new scene object and read in the data from the file
	// no meshes or materials are created yet
	// we're not making any assumptions about what the data may be loaded in for
	model_scene_ = new gef::Scene();
	model_scene_->ReadSceneFromFile(platformRef, "xbot/xbot.scn");

	// we do want to render the data stored in the scene file so lets create the materials from the material data present in the scene file
	model_scene_->CreateMaterials(platformRef);

	// if there is mesh data in the scene, create a mesh to draw from the first mesh
	mesh_ = GetFirstMesh();
}

void AnimatedModel::Update(float dt, MotionClipPlayer* anim_player_)
{
	if (player_)
	{
		// update the pose in the anim player from the animation
		anim_player_->Update(dt, player_->bind_pose());

		// update the bone matrices that are used for rendering the character
		// from the newly updated pose in the anim player
		player_->UpdateBoneMatrices(anim_player_->pose());
	}

	// build a transformation matrix that will position the character
	// use this to move the player around, scale it, etc.
	if (player_)
	{
		gef::Matrix44 player_transform;
		player_transform.SetIdentity();
		player_transform.Scale(gef::Vector4(0.01f, 0.01f, 0.01f));
		player_->set_transform(player_transform);
	}
}

void AnimatedModel::Update(float dt, MotionClipPlayer* anim_player_, MotionClipPlayer* anim_player_2)
{
	if (player_)
	{
		// update the pose in the anim player from the animation
		anim_player_->Update(dt, player_->bind_pose());
		anim_player_2->Update(dt, player_->bind_pose());
	}

	// build a transformation matrix that will position the character
	// use this to move the player around, scale it, etc.
	if (player_)
	{
		gef::Matrix44 player_transform;
		player_transform.SetIdentity();
		player_transform.Scale(gef::Vector4(0.01f, 0.01f, 0.01f));
		player_->set_transform(player_transform);
	}
}

void AnimatedModel::Render()
{

	// draw the player, the pose is defined by the bone matrices
	if (player_)
		rendererRef->DrawSkinnedMesh(*player_, player_->bone_matrices());
}

gef::Mesh* AnimatedModel::GetFirstMesh()
{
	gef::Mesh* mesh = NULL;

	if (model_scene_)
	{
		// now check to see if there is any mesh data in the file, if so lets create a mesh from it
		if (model_scene_->mesh_data.size() > 0)
			mesh = model_scene_->CreateMesh(platformRef, model_scene_->mesh_data.front());
	}

	return mesh;
}
