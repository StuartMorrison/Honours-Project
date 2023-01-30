#include "animated_mesh_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#include <maths/vector2.h>
#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>
#include <input/keyboard.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>
#include <graphics/scene.h>
#include <animation/skeleton.h>
#include <animation/animation.h>

AnimatedMeshApp::AnimatedMeshApp(gef::Platform& platform) :
	Application(platform),
	anim_manager_(platform),
	sprite_renderer_(NULL),
	font_(NULL),
	mesh_(NULL),
	player_(NULL),
	renderer_3d_(NULL),
	model_scene_(NULL),
	walk_anim_(NULL),
	primitive_builder_(NULL),
	primitive_renderer_(NULL),
	floor_mesh_(NULL)
{
}

void AnimatedMeshApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	input_manager_ = gef::InputManager::Create(platform_);

	InitFont();
	SetupCamera();
	SetupLights();

	// create a new scene object and read in the data from the file
	// no meshes or materials are created yet
	// we're not making any assumptions about what the data may be loaded in for
	model_scene_ = new gef::Scene();
	model_scene_->ReadSceneFromFile(platform_, "xbot/xbot.scn");

	// we do want to render the data stored in the scene file so lets create the materials from the material data present in the scene file
	model_scene_->CreateMaterials(platform_);

	// if there is mesh data in the scene, create a mesh to draw from the first mesh
	mesh_ = GetFirstMesh(model_scene_);

	// get the first skeleton in the scene
	gef::Skeleton* skeleton = GetFirstSkeleton(model_scene_);

	if (skeleton)
	{
		player_ = new gef::SkinnedMeshInstance(*skeleton);
		anim_player_.Init(player_->bind_pose());
		player_->set_mesh(mesh_);
	}


	// anims
	walk_anim_ = LoadAnimation("xbot/xbot@walking_inplace.scn", "");

	if (walk_anim_)
	{
		anim_player_.set_clip(walk_anim_);
		anim_player_.set_looping(true);
		anim_player_.set_anim_time(0.0f);
	}

	run_anim_ = LoadAnimation("xbot/xbot@running_inplace.scn", "");
	idle_anim_ = LoadAnimation("xbot/xbot@idle.scn", "");

	/*output_node_.init(player_->bind_pose());
	clip_node_.init(player_->bind_pose());
	clip_node_.SetClip(walk_anim_);
	clip_node_2.init(player_->bind_pose());
	clip_node_2.SetClip(run_anim_);
	linear_blend_node_.init(player_);
	linear_blend_node_.SetInput(0, &clip_node_);
	linear_blend_node_.SetInput(1, &clip_node_2);
	output_node_.SetInput(0, &linear_blend_node_);*/

	walk_speed_min = 1.f;
	walk_speed_max = walk_anim_->duration() / run_anim_->duration();

	run_speed_min = run_anim_->duration() / walk_anim_->duration();
	run_speed_max = 1.f;

	node_manager_.init(player_);
	node_manager_.NewOutputNode();
	node_manager_.NewLinearBlendNode();
	node_manager_.NewClipNode(idle_anim_);
	node_manager_.NewClipNode(walk_anim_);
	node_manager_.NewClipNode(run_anim_);

	node_manager_.output_nodes_[0]->SetInput(0, node_manager_.linear_blend_nodes_[0]);
	node_manager_.linear_blend_nodes_[0]->SetInput(0, node_manager_.clip_nodes_[0]);
	node_manager_.linear_blend_nodes_[0]->SetInput(1, node_manager_.clip_nodes_[1]);

	primitive_builder_ = new PrimitiveBuilder(platform_);
	primitive_renderer_ = new PrimitiveRenderer(platform_);
}


void AnimatedMeshApp::CleanUp()
{

	delete primitive_renderer_;
	primitive_renderer_ = NULL;

	delete primitive_builder_;
	primitive_builder_ = NULL;

	CleanUpFont();

	delete player_;
	player_ = NULL;

	delete walk_anim_;
	walk_anim_ = NULL;

	delete mesh_;
	mesh_ = NULL;

	delete model_scene_;
	model_scene_ = NULL;

	delete input_manager_;
	input_manager_ = NULL;

	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;
}

bool AnimatedMeshApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;


	// read input devices
	if (input_manager_)
	{
		input_manager_->Update();

		// controller input
		gef::SonyControllerInputManager* controller_manager = input_manager_->controller_input();
		if (controller_manager)
		{
		}

		// keyboard input
		gef::Keyboard* keyboard = input_manager_->keyboard();
		if (keyboard)
		{
			/*if (keyboard->IsKeyPressed(gef::Keyboard::KC_SPACE))
			{
				anim_manager_.SwitchAnimation(true);
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_Z))
			{
				if (anim_manager_.GetSkeleton()->GetBlendValue() < 1.0f)
				{
					anim_manager_.GetSkeleton()->SetBlendValue(0.1f);
				}
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_X))
			{
				if (anim_manager_.GetSkeleton()->GetBlendValue() > 0.0f)
				{
					anim_manager_.GetSkeleton()->SetBlendValue(-0.1f);
				}
			}*/

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_Z))
			{
				if (speed_ >= 1.0f && speed_ <= 1.1f)
				{
					node_manager_.linear_blend_nodes_[0]->SetInput(0, node_manager_.clip_nodes_[1]);
					node_manager_.linear_blend_nodes_[0]->SetInput(1, node_manager_.clip_nodes_[2]);
					//node_manager_.clip_nodes_[1]->animTime = node_manager_.clip_nodes_[0]->animTime;

					isRunning = true;
				}

				if (speed_ >= run_speed_)
				{
					speed_ = run_speed_;
				}
				else
				{
					speed_ = speed_ + 0.1f;
				}
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_X))
			{
				if (speed_ <= 1.1f && speed_ >= 1.0f)
				{
					node_manager_.linear_blend_nodes_[0]->SetInput(0, node_manager_.clip_nodes_[0]);
					node_manager_.linear_blend_nodes_[0]->SetInput(1, node_manager_.clip_nodes_[1]);
					node_manager_.clip_nodes_[1]->playbackSpeed = 1.0f;
					isRunning = false;
				}

				if (speed_ <= 0.0f)
				{
					speed_ = 0.0f;
				}
				else
				{
					speed_ = speed_ - 0.1f;
				}
			}

		}
	}

	if(player_)
	{
		// update the pose in the anim player from the animation
		anim_player_.Update(frame_time, player_->bind_pose());

		// update the bone matrices that are used for rendering the character
		// from the newly updated pose in the anim player
		//player_->UpdateBoneMatrices(anim_player_.pose());
		//output_node_.Update(frame_time);
		node_manager_.linear_blend_nodes_[0]->blendValue = speed_;

		if (isRunning)
		{
			node_manager_.linear_blend_nodes_[0]->blendValue = (node_manager_.linear_blend_nodes_[0]->blendValue - 1.f) / (3.f - 1.f);
			float walk_speed_mod = (walk_speed_max - walk_speed_min) * node_manager_.linear_blend_nodes_[0]->blendValue;
			float run_speed_mod = (run_speed_max - run_speed_min) * node_manager_.linear_blend_nodes_[0]->blendValue;
			node_manager_.clip_nodes_[1]->playbackSpeed = (walk_speed_min + walk_speed_mod);
			node_manager_.clip_nodes_[2]->playbackSpeed = (run_speed_min + run_speed_mod);
		}

		node_manager_.output_nodes_[0]->Update(frame_time);
		player_->UpdateBoneMatrices(node_manager_.output_nodes_[0]->output);
		
	}

	// build a transformation matrix that will position the character
	// use this to move the player around, scale it, etc.
	if (player_)
	{
		gef::Matrix44 player_transform;
		player_transform.SetIdentity();
		player_transform.Scale(gef::Vector4(0.01f, 0.01f, 0.01f));
		//player_transform.RotationX(-1.503f);
		player_->set_transform(player_transform);
	}

	//anim_manager_.Update(frame_time);

	return true;
}

void AnimatedMeshApp::Render()
{
	// setup view and projection matrices
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(camera_fov_, (float)platform_.width() / (float)platform_.height(), near_plane_, far_plane_);
	view_matrix.LookAt(camera_eye_, camera_lookat_, camera_up_);
	renderer_3d_->set_projection_matrix(projection_matrix);
	renderer_3d_->set_view_matrix(view_matrix);

	// draw meshes here
	renderer_3d_->Begin();

	// draw the player, the pose is defined by the bone matrices
	if(player_)
		renderer_3d_->DrawSkinnedMesh(*player_, player_->bone_matrices());

	//anim_manager_.Render();

	renderer_3d_->DrawMesh(floor_gfx_);

	renderer_3d_->End();

	// setup the sprite renderer, but don't clear the frame buffer
	// draw 2D sprites here
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}
void AnimatedMeshApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void AnimatedMeshApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void AnimatedMeshApp::DrawHUD()
{
	if(font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

void AnimatedMeshApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void AnimatedMeshApp::SetupCamera()
{
	// initialise the camera settings
	camera_eye_ = gef::Vector4(-4.0f, 1.0f, 4.0f);
	camera_lookat_ = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_up_ = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_fov_ = gef::DegToRad(45.0f);
	near_plane_ = 0.01f;
	far_plane_ = 1000.f;
}


gef::Skeleton* AnimatedMeshApp::GetFirstSkeleton(gef::Scene* scene)
{
	gef::Skeleton* skeleton = NULL;
	if (scene)
	{
		// check to see if there is a skeleton in the the scene file
		// if so, pull out the bind pose and create an array of matrices
		// that wil be used to store the bone transformations
		if (scene->skeletons.size() > 0)
			skeleton = scene->skeletons.front();
	}

	return skeleton;
}

gef::Mesh* AnimatedMeshApp::GetFirstMesh(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	if (scene)
	{
		// now check to see if there is any mesh data in the file, if so lets create a mesh from it
		if (scene->mesh_data.size() > 0)
			mesh = model_scene_->CreateMesh(platform_, scene->mesh_data.front());
	}

	return mesh;
}

gef::Animation* AnimatedMeshApp::LoadAnimation(const char* anim_scene_filename, const char* anim_name)
{
	gef::Animation* anim = NULL;

	gef::Scene anim_scene;
	if (anim_scene.ReadSceneFromFile(platform_, anim_scene_filename))
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
