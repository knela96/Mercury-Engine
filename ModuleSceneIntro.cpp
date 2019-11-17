#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "GameObject.h"
#include "C_Transform.h"
#include "Gizmo.h"
#include "KDimensionalTree.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}


bool ModuleSceneIntro::Init() {
	KDimensionalTree quat;
	root = new GameObject("Scene");
	root->childs.push_back(new GameObject("Main Camera",root));
	root->childs[0]->components.push_back(root->childs[0]->AddComponent(Component_Type::Camera));//FIX
	root->transform->UpdateMatrices();
	return true;
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	for (int i = 0; i < root->childs.size(); ++i) {
		if (root->childs[i]->active && root->active)
			root->childs[i]->Start();
		root->childs[i]->StartChilds();
	}
	return ret;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->importer->CleanUp();
	for (GameObject* obj : root->childs) {
		if (obj != nullptr) {
			obj->CleanUp();
			delete obj;
			obj = nullptr;
		}
	}
	root->childs.clear();

	return true;
}

bool ModuleSceneIntro::Draw()
{
	for (int i = 0; i < root->childs.size(); ++i) {
		if (root->childs[i]->active && root->active)
			root->childs[i]->Draw();
			root->childs[i]->drawChilds();
	}

	DrawBB();


	return true;
}

void ModuleSceneIntro::DrawBB() {
	for (int i = 0; i < aabbs.size(); ++i) {
		Gizmo::DrawBox(*aabbs[i].box, aabbs[i].color);
	}
	for (int i = 0; i < obbs.size(); ++i) {
		Gizmo::DrawBox(*obbs[i].box, obbs[i].color);
	}
	for (int i = 0; i < frustums.size(); ++i) {
		Gizmo::DrawBox(*frustums[i].box, frustums[i].color);
	}
}

void ModuleSceneIntro::AddAABB(AABB* box,Color color) {
	aabbs.push_back(BBox<AABB>(box,color));
}

void ModuleSceneIntro::AddOBB(OBB* box, Color color) {
	obbs.push_back(BBox<OBB>(box, color));
}

void ModuleSceneIntro::AddFrustum(Frustum* box, Color color) {
	frustums.push_back(BBox<Frustum>(box, color));
}

bool ModuleSceneIntro::setParent(GameObject * parent, GameObject * child)
{
	for (int i = 0; i < child->childs.size(); ++i) {
		if (child->childs[i] == parent) {
			LOGC("Cannot parent to a child GameObject");
			return false;
		}
	}

	for (int i = 0; i < child->parent->childs.size(); ++i) {
		if (child->parent->childs[i] == child)
			child->parent->childs.erase(child->parent->childs.begin() + i);
	}
	child->parent = parent;
	parent->childs.push_back(child);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{

	Plane_ p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	
	Draw();

	return UPDATE_CONTINUE;
}
