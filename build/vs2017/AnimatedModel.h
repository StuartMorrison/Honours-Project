#pragma once
#include <graphics/skinned_mesh_instance.h>
#include <graphics/scene.h>
#include <graphics/renderer_3d.h>
#include <system/platform.h>
#include <motion_clip_player.h>


class AnimatedModel
{
public:
	AnimatedModel(gef::Platform& platform);
	~AnimatedModel();
	void init();
	void Update(float dt, MotionClipPlayer* anim_player_);
	void Update(float dt, MotionClipPlayer* anim_player_, MotionClipPlayer* anim_player_2);
	void SetRenderer(gef::Renderer3D* renderer_3d_) { rendererRef = renderer_3d_; };
	void Render();

	gef::Mesh* GetFirstMesh();
	
	gef::Mesh* GetMesh() { return mesh_; };
	gef::SkinnedMeshInstance** GetInstance() { return &player_; };
	gef::Scene* GetScene() { return model_scene_; };

private:
	gef::Renderer3D* rendererRef;
	gef::Platform& platformRef;


	class gef::Mesh* mesh_;
	gef::SkinnedMeshInstance* player_;
	gef::Scene* model_scene_;
};

