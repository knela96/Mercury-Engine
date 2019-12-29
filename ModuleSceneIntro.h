#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__
#include "Globals.h"
#include "Module.h"
#include "p2DynArray.h"
#include "SystemInfo.h"
#include "Color.h"
#include <vector>
#include "Gizmo.h"
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
	void setParentByID(UID parent_ID, GameObject * parent, GameObject * child);
	void LoadScenePopUp();

	void SaveScenePopUp();

	void SaveScene(std::string fileName);

	uint SaveAllScene(GameObject * root, json & file, uint & count);

	void LoadScene(std::string * fileName);

	uint LoadAllScene(GameObject * root, json & file, uint * elements, uint & count);

	std::vector<BBox<AABB>> aabbs;
	std::vector<BBox<OBB>> obbs;
	std::vector<BBox<Frustum>> frustums;

public:
	GameObject* root = nullptr;

	Gizmo gizmo;

	GameObject* main_camera = nullptr;

	std::vector<GameObject*> use_elements;
	bool camera_culling = false;
	Quadtree* quat = nullptr;
	AABB* box = nullptr;

	bool lookQuads = true;

	bool savePopUp = false;
	uint LoadElementResources(json & file, uint elements);
};
#endif
