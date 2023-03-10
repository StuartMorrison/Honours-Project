#ifndef _ANIMATED_MESH_APP_H
#define _ANIMATED_MESH_APP_H

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include <vector>
#include <graphics/skinned_mesh_instance.h>
#include "motion_clip_player.h"

#include "AnimationManager.h"
#include "NodeManager.h"
#include "OutputNode.h" 
#include "ClipNode.h"
#include "LinearBlendNode.h"

#include "BoneModifier.h"

#include "primitive_builder.h"
#include "primitive_renderer.h"


// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class Scene;
	class Skeleton;
	class InputManager;
}
class AnimatedMeshApp : public gef::Application
{
public:
	AnimatedMeshApp(gef::Platform& platform);
	void Init();

	gef::Skeleton* GetFirstSkeleton(gef::Scene* scene);

	gef::Mesh* GetFirstMesh(gef::Scene* scene);

	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void SetupLights();
	void SetupCamera();
	gef::Animation* LoadAnimation(const char* anim_scene_filename, const char* anim_name);

	//EMOTION FUNCTIONS
	void InitHappy();
	void InitSad();
	void InitAnger();
	void InitFear();

	gef::SpriteRenderer* sprite_renderer_;
	gef::Renderer3D* renderer_3d_;
	gef::InputManager* input_manager_;
	gef::Font* font_;

	float fps_;

	class gef::Mesh* mesh_;
	gef::SkinnedMeshInstance* player_;

	gef::Scene* model_scene_;

	gef::Vector4 camera_eye_;
	gef::Vector4 camera_lookat_;
	gef::Vector4 camera_up_;
	float camera_fov_;
	float near_plane_;
	float far_plane_;

	MotionClipPlayer anim_player_;
	gef::Animation* walk_anim_;
	gef::Animation* run_anim_;
	gef::Animation* idle_anim_;

	PrimitiveBuilder* primitive_builder_;
	PrimitiveRenderer* primitive_renderer_;

	gef::Mesh* floor_mesh_;
	gef::MeshInstance floor_gfx_; //initialize later

	AnimationManager anim_manager_;
	float speed_ = 0.0f;
	float walk_speed_ = 1.0f;
	float run_speed_ = 4.0f;
	float walk_speed_min, walk_speed_max;
	float run_speed_min, run_speed_max;
	bool isRunning = false;
	NodeManager node_manager_;

	//DEMO
	BoneModifier bone_modifier_;
	int bone_index_;

	bool translation_selected_;
	bool rotation_selected_;

	gef::Matrix44 originalValues;
	gef::Matrix44 modifyValues;
	gef::Vector4 modifyTranslation;
	gef::Quaternion modifyRotation;

	/*OutputNode output_node_;
	ClipNode clip_node_;
	ClipNode clip_node_2;
	LinearBlendNode linear_blend_node_;*/
};

#endif // _ANIMATED_MESH_APP_H
