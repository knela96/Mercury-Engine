#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__
#include "Globals.h"
#include "Module.h"
#include "p2DynArray.h"
#include "SystemInfo.h"
#include "Color.h"
#include <vector>
#include "Quadtree.h"

#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"

template <typename Box>
struct BBox {
	BBox(const Box* box, Color& color) : box(box), color(color) {}

	const Box* box;
	Color color;
};

class GameObject;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool Draw();
	void DrawBB();
	void AddAABB(AABB * box, Color color);
	void AddOBB(OBB * box, Color color);
	void AddFrustum(Frustum * box, Color color);
	void Insert2Quat(GameObject * gameobject);
	void Remove2Quat(GameObject * gameobject);
	bool setParent(GameObject* to_parent, GameObject* to_child);

	//Engine::SystemInfo s_info;

	std::vector<BBox<AABB>> aabbs;
	std::vector<BBox<OBB>> obbs;
	std::vector<BBox<Frustum>> frustums;

public:
	GameObject* root = nullptr;
	GameObject* main_camera = nullptr;

	std::vector<GameObject*> use_elements;
	bool camera_culling = false;
	Quadtree* quat = nullptr;
	AABB* box = nullptr;
};
#endif
