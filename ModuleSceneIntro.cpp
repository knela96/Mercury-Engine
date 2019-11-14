#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
//#include "MathGeoLib/include/Geometry/Sphere.h"
#include "Primitive.h"
#include "GameObject.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}


bool ModuleSceneIntro::Init() {
	root = new GameObject("Scene");
	return true;
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
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
		if (root->childs[i]->active)
			root->childs[i]->Draw();
			root->childs[i]->drawChilds();
	}

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
