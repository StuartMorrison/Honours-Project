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

//	run_anim_ = LoadAnimation("xbot/xbot@running_inplace.scn", "");
	//idle_anim_ = LoadAnimation("xbot/xbot@idle.scn", "");

	/*output_node_.init(player_->bind_pose());
	clip_node_.init(player_->bind_pose());
	clip_node_.SetClip(walk_anim_);
	clip_node_2.init(player_->bind_pose());
	clip_node_2.SetClip(run_anim_);
	linear_blend_node_.init(player_);
	linear_blend_node_.SetInput(0, &clip_node_);
	linear_blend_node_.SetInput(1, &clip_node_2);
	output_node_.SetInput(0, &linear_blend_node_);*/

	//walk_speed_min = 1.f;
	//walk_speed_max = walk_anim_->duration() / run_anim_->duration();

	//run_speed_min = run_anim_->duration() / walk_anim_->duration();
	//run_speed_max = 1.f;

	node_manager_.init(player_);
	node_manager_.NewOutputNode();
	//node_manager_.NewLinearBlendNode();
	//node_manager_.NewClipNode(idle_anim_);
	node_manager_.NewClipNode(walk_anim_);
	//node_manager_.NewClipNode(run_anim_);

	node_manager_.output_nodes_[0]->SetInput(0, node_manager_.clip_nodes_[0]);
	//node_manager_.linear_blend_nodes_[0]->SetInput(0, node_manager_.clip_nodes_[0]);
	//node_manager_.linear_blend_nodes_[0]->SetInput(1, node_manager_.clip_nodes_[1]);

	bone_modifier_.Init(player_);
	bone_index_ = 0;

	translation_selected_ = false;
	rotation_selected_ = false;

	originalValues.SetIdentity();
	modifyValues.SetIdentity();

	modifyTranslation.set_x(0.0f);
	modifyTranslation.set_y(0.0f);
	modifyTranslation.set_z(0.0f);
	modifyTranslation.set_w(0.0f);

	modifyRotation.x = 0.0f;
	modifyRotation.y = 0.0f;
	modifyRotation.z = 0.0f;
	modifyRotation.w = 0.0f;

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
			if (keyboard->IsKeyPressed(gef::Keyboard::KC_LEFT))
			{
				if (bone_index_ != 0)
				{
					//node_manager_.output_nodes_[0]->output.local_pose()[bone_index_] = originalValues;
					bone_index_--;
				}
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_RIGHT))
			{
				if (bone_index_ != 51)
				{
					//node_manager_.output_nodes_[0]->output.local_pose()[bone_index_] = originalValues;
					bone_index_++;
				}
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_T))
			{
				translation_selected_ = true;
				rotation_selected_ = false;

				originalValues = node_manager_.output_nodes_[0]->output.local_pose()[bone_index_].GetMatrix();
				modifyValues.SetIdentity();
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_R))
			{
				rotation_selected_ = true;
				translation_selected_ = false;

				originalValues = node_manager_.output_nodes_[0]->output.local_pose()[bone_index_].GetMatrix();
				modifyValues.SetIdentity();
			}

			if (keyboard->IsKeyDown(gef::Keyboard::KC_X))
			{
				if (translation_selected_)
				{
					if (keyboard->IsKeyDown(gef::Keyboard::KC_EQUALS))
					{
						modifyTranslation.set_x(modifyTranslation.x() + 1.0f);
					}

					if (keyboard->IsKeyDown(gef::Keyboard::KC_MINUS))
					{
						modifyTranslation.set_x(modifyTranslation.x() - 1.0f);
					}
				}

				if (rotation_selected_)
				{
					if (keyboard->IsKeyDown(gef::Keyboard::KC_EQUALS))
					{
						modifyRotation.x += 0.01;
					}

					if (keyboard->IsKeyDown(gef::Keyboard::KC_MINUS))
					{
						modifyRotation.x -= 0.01;
					}
				}
			}

			if (keyboard->IsKeyDown(gef::Keyboard::KC_Y))
			{
				if (translation_selected_)
				{
					if (keyboard->IsKeyDown(gef::Keyboard::KC_EQUALS))
					{
						modifyTranslation.set_y(modifyTranslation.y() + 1.0f);
					}

					if (keyboard->IsKeyDown(gef::Keyboard::KC_MINUS))
					{
						modifyTranslation.set_y(modifyTranslation.y() - 1.0f);
					}
				}

				if (rotation_selected_)
				{
					if (keyboard->IsKeyDown(gef::Keyboard::KC_EQUALS))
					{
						modifyRotation.y += 0.01;
					}

					if (keyboard->IsKeyDown(gef::Keyboard::KC_MINUS))
					{
						modifyRotation.y -= 0.01;
					}
				}
			}

			if (keyboard->IsKeyDown(gef::Keyboard::KC_Z))
			{
				if (translation_selected_)
				{
					if (keyboard->IsKeyDown(gef::Keyboard::KC_EQUALS))
					{
						modifyTranslation.set_z(modifyTranslation.z() + 1.0f);
					}

					if (keyboard->IsKeyDown(gef::Keyboard::KC_MINUS))
					{
						modifyTranslation.set_z(modifyTranslation.z() - 1.0f);
					}
				}

				if (rotation_selected_)
				{
					if (keyboard->IsKeyDown(gef::Keyboard::KC_EQUALS))
					{
						modifyRotation.z =+ 0.01;
					}

					if (keyboard->IsKeyDown(gef::Keyboard::KC_MINUS))
					{
						modifyRotation.z -= 0.01;
					}
				}
			}

			if (keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD0))
			{
				bone_modifier_.ChangeEmotion(0);

				node_manager_.clip_nodes_[0]->playbackSpeed = 1.0f;
			}

			if (keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD1))
			{
				bone_modifier_.ChangeEmotion(1);
			}

			if (keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD2))
			{
				bone_modifier_.ChangeEmotion(2);

				node_manager_.clip_nodes_[0]->playbackSpeed = 0.6f;
			}

			if (keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD3))
			{
				bone_modifier_.ChangeEmotion(3);
			}

			if (keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD4))
			{
				bone_modifier_.ChangeEmotion(4);
			}
		}
	}

	if(player_)
	{
		// update the pose in the anim player from the animation
		anim_player_.Update(frame_time, player_->bind_pose());

		node_manager_.output_nodes_[0]->Update(frame_time);

		/*modifyValues.SetIdentity();
		modifyValues.Scale(gef::Vector4(1.0f, 1.0f, 1.0f));
		gef::Quaternion temp = node_manager_.output_nodes_[0]->output.local_pose()[bone_index_ + 1].rotation() + modifyRotation;
		temp.Normalise();
		modifyValues.Rotation(temp);
		modifyValues.SetTranslation(node_manager_.output_nodes_[0]->output.local_pose()[bone_index_ + 1].translation() + modifyTranslation);*/

		if (bone_modifier_.GetEmotion() == 0)
		{
			node_manager_.output_nodes_[0]->output.local_pose()[bone_index_ + 1] = bone_modifier_.ModifyBones(
				node_manager_.output_nodes_[0]->output.local_pose()[bone_index_ + 1].GetMatrix(),
				node_manager_.output_nodes_[0]->output.local_pose()[bone_index_ + 1].rotation(),
				modifyTranslation,
				modifyRotation);
		}

		if (bone_modifier_.GetEmotion() == 1)
		{
			InitHappy();
		}

		if (bone_modifier_.GetEmotion() == 2)
		{
			InitSad();
		}

		//node_manager_.output_nodes_[0]->output.local_pose()[bone_index_ + 1].set_rotation(node_manager_.output_nodes_[0]->output.local_pose()[bone_index_ + 1].rotation() + modifyRotation);
		//node_manager_.output_nodes_[0]->output.local_pose()[bone_index_ + 1].set_translation(node_manager_.output_nodes_[0]->output.local_pose()[bone_index_ + 1].translation() + modifyTranslation);

		//node_manager_.output_nodes_[0]->output.local_pose()[bone_index_ + 1] = modifyValues;

		node_manager_.output_nodes_[0]->output.CalculateGlobalPose();

		player_->UpdateBoneMatrices(node_manager_.output_nodes_[0]->output);
		//put change emotion function here

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
		font_->RenderText(sprite_renderer_, gef::Vector4(0.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, bone_modifier_.GetMap(bone_index_).c_str());

		font_->RenderText(sprite_renderer_, gef::Vector4(200.0f, 410.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT,  std::to_string(modifyRotation.x).c_str());
		font_->RenderText(sprite_renderer_, gef::Vector4(200.0f, 460.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT,  std::to_string(modifyRotation.y).c_str());
		font_->RenderText(sprite_renderer_, gef::Vector4(200.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT,  std::to_string(modifyRotation.z).c_str());
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

void AnimatedMeshApp::InitHappy()
{

}

void AnimatedMeshApp::InitSad()
{
	modifyTranslation = gef::Vector4(0.0f, 0.0f, 0.0f);
	modifyRotation = gef::Quaternion(0.0f, 0.0f, 0.0f, 0.0f);

	//body + head
	//no hips as that is essentially the root

	//torso
	modifyRotation.x = 0.15f;

	node_manager_.output_nodes_[0]->output.local_pose()[4] = bone_modifier_.ModifyBones(
		node_manager_.output_nodes_[0]->output.local_pose()[4].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[4].rotation(),
		modifyTranslation,
		modifyRotation);

	modifyRotation.x = 0.0f;

	//lower chest
	modifyRotation.x = 0.2f;

	node_manager_.output_nodes_[0]->output.local_pose()[11] = bone_modifier_.ModifyBones(
		node_manager_.output_nodes_[0]->output.local_pose()[11].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[11].rotation(),
		modifyTranslation,
		modifyRotation);

	modifyRotation.x = 0.0f;

	//upper chest
	modifyRotation.x = 0.05f;

	node_manager_.output_nodes_[0]->output.local_pose()[12] = bone_modifier_.ModifyBones(
		node_manager_.output_nodes_[0]->output.local_pose()[12].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[12].rotation(),
		modifyTranslation,
		modifyRotation);

	modifyRotation.x = 0.0f;

	//neck
	modifyRotation.x = 0.4f;

	node_manager_.output_nodes_[0]->output.local_pose()[14] = bone_modifier_.ModifyBones(
		node_manager_.output_nodes_[0]->output.local_pose()[14].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[14].rotation(),
		modifyTranslation,
		modifyRotation);

	modifyRotation.x = 0.0f;

	//head
	modifyRotation.x = 0.01f;

	node_manager_.output_nodes_[0]->output.local_pose()[34] = bone_modifier_.ModifyBones(
		node_manager_.output_nodes_[0]->output.local_pose()[34].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[34].rotation(),
		modifyTranslation,
		modifyRotation);

	modifyRotation.x = 0.0f;

	//arms
	//left shoulder
	modifyRotation.y = -0.3f;

	node_manager_.output_nodes_[0]->output.local_pose()[13] = bone_modifier_.ModifyBones(
		node_manager_.output_nodes_[0]->output.local_pose()[13].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[13].rotation(),
		modifyTranslation,
		modifyRotation);

	modifyRotation.y = 0.0f;

	//right shoulder
	modifyRotation.y = 0.3f;

	node_manager_.output_nodes_[0]->output.local_pose()[15] = bone_modifier_.ModifyBones(
		node_manager_.output_nodes_[0]->output.local_pose()[15].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[15].rotation(),
		modifyTranslation,
		modifyRotation);


	modifyRotation.y = 0.0f;

	//left upper arm
	modifyRotation.x = -0.5f;

	node_manager_.output_nodes_[0]->output.local_pose()[16] = bone_modifier_.ModifyBones(
		node_manager_.output_nodes_[0]->output.local_pose()[16].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[16].rotation(),
		modifyTranslation,
		modifyRotation);

	modifyRotation.x = 0.0f;

	//right upper arm
	modifyRotation.x = -1.0f;

	node_manager_.output_nodes_[0]->output.local_pose()[35] = bone_modifier_.ModifyBones(
		node_manager_.output_nodes_[0]->output.local_pose()[35].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[35].rotation(),
		modifyTranslation,
		modifyRotation);

	modifyRotation.x = 0.0f;

	//left shoulder rotation change
	node_manager_.output_nodes_[0]->output.local_pose()[13] = bone_modifier_.ModifyRotation(
		node_manager_.output_nodes_[0]->output.local_pose()[13].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[13].rotation(),
		gef::Quaternion(0.6f, 1.0f, 0.6f, 1.0f));

	//right shoulder rotation change
	node_manager_.output_nodes_[0]->output.local_pose()[15] = bone_modifier_.ModifyRotation(
		node_manager_.output_nodes_[0]->output.local_pose()[15].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[15].rotation(),
		gef::Quaternion(0.6f, 1.0f, 0.6f, 1.0f));

	//left upper arm rotation change
	node_manager_.output_nodes_[0]->output.local_pose()[16] = bone_modifier_.ModifyRotation(
		node_manager_.output_nodes_[0]->output.local_pose()[16].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[16].rotation(),
		gef::Quaternion(2.2f, 1.0f, 1.2f, 1.0f));

	//left lower arm rotation change
	node_manager_.output_nodes_[0]->output.local_pose()[17] = bone_modifier_.ModifyRotation(
		node_manager_.output_nodes_[0]->output.local_pose()[17].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[17].rotation(),
		gef::Quaternion(0.1f, 0.2f, 0.1f, 1.0f));

	//right upper arm rotation change
	node_manager_.output_nodes_[0]->output.local_pose()[35] = bone_modifier_.ModifyRotation(
		node_manager_.output_nodes_[0]->output.local_pose()[35].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[35].rotation(),
		gef::Quaternion(0.7f, 0.0f, 1.1f, 1.0f));

	//right lower arm rotation change
	node_manager_.output_nodes_[0]->output.local_pose()[36] = bone_modifier_.ModifyRotation(
		node_manager_.output_nodes_[0]->output.local_pose()[36].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[36].rotation(),
		gef::Quaternion(0.3f, 0.2f, 0.3f, 1.0f));

	//left hand rotation change
	node_manager_.output_nodes_[0]->output.local_pose()[18] = bone_modifier_.ModifyRotation(
		node_manager_.output_nodes_[0]->output.local_pose()[18].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[18].rotation(),
		gef::Quaternion(0.1f, 0.1f, 0.1f, 1.0f));

	//right hand rotation change
	node_manager_.output_nodes_[0]->output.local_pose()[37] = bone_modifier_.ModifyRotation(
		node_manager_.output_nodes_[0]->output.local_pose()[37].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[37].rotation(),
		gef::Quaternion(0.1f, 0.1f, 0.1f, 1.0f));

	//legs
	//left upper leg rotation change
	node_manager_.output_nodes_[0]->output.local_pose()[2] = bone_modifier_.ModifyRotation(
		node_manager_.output_nodes_[0]->output.local_pose()[2].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[2].rotation(),
		gef::Quaternion(0.9f, 0.9f, 0.9f, 1.0f));

	//left lower leg rotation change
	node_manager_.output_nodes_[0]->output.local_pose()[5] = bone_modifier_.ModifyRotation(
		node_manager_.output_nodes_[0]->output.local_pose()[5].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[5].rotation(),
		gef::Quaternion(0.5f, 1.0f, 0.5f, 1.0f));

	//left heel rotation change
	node_manager_.output_nodes_[0]->output.local_pose()[6] = bone_modifier_.ModifyRotation(
		node_manager_.output_nodes_[0]->output.local_pose()[6].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[6].rotation(),
		gef::Quaternion(0.3f, 1.0f, 0.3f, 1.0f));

	//right upper leg rotation change
	node_manager_.output_nodes_[0]->output.local_pose()[3] = bone_modifier_.ModifyRotation(
		node_manager_.output_nodes_[0]->output.local_pose()[3].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[3].rotation(),
		gef::Quaternion(0.9f, 0.9f, 0.9f, 1.0f));

	//right lower leg rotation change
	node_manager_.output_nodes_[0]->output.local_pose()[8] = bone_modifier_.ModifyRotation(
		node_manager_.output_nodes_[0]->output.local_pose()[8].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[8].rotation(),
		gef::Quaternion(0.5f, 1.0f, 0.5f, 1.0f));

	//right heel rotation change
	node_manager_.output_nodes_[0]->output.local_pose()[9] = bone_modifier_.ModifyRotation(
		node_manager_.output_nodes_[0]->output.local_pose()[9].GetMatrix(),
		node_manager_.output_nodes_[0]->output.local_pose()[9].rotation(),
		gef::Quaternion(0.3f, 1.0f, 0.3f, 1.0f));

}