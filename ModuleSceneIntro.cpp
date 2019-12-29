#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "GameObject.h"
#include "C_Transform.h"
#include "Gizmo.h"
#include "C_Camera.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}


bool ModuleSceneIntro::Init() {
	box = new AABB(float3(-1000, -1000, -1000), float3(1000, 1000, 1000));
	quat = new Quadtree(*box);
	root = new GameObject("Scene");
	root->childs.push_back(new GameObject("Main Camera",root));
	root->childs[0]->AddComponent(Component_Type::Camera);//FIX
	main_camera = root->childs[0];
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
			if(App->scene_intro->main_camera->camera->CullFace(root))
				root->childs[i]->Draw();
			root->childs[i]->drawChilds();
	}

	DrawBB();

	quat->Draw();
	return true;
}

void ModuleSceneIntro::DrawBB() {
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

void ModuleSceneIntro::Insert2Quat(GameObject* gameobject) {
	quat->Insert(gameobject);
}

void ModuleSceneIntro::Remove2Quat(GameObject* gameobject) {
	quat->Remove(gameobject);
	lookQuads = true;
}
bool ModuleSceneIntro::setParent(GameObject * parent, GameObject * child)
{
	for (int i = 0; i < child->childs.size(); ++i) {
		if (child->childs[i] == parent) {
			LOGC("Cannot parent to a child GameObject");
			return false;
		}
	}
	if (child->parent != nullptr) {
		for (int i = 0; i < child->parent->childs.size(); ++i) {
			if (child->parent->childs[i] == child) {
				child->transform->vposition += child->parent->transform->vposition;
				child->transform->vrotation.x += child->parent->transform->vrotation.x;
				child->transform->vrotation.y += child->parent->transform->vrotation.y;
				child->transform->vrotation.z += child->parent->transform->vrotation.z;
				child->transform->vscale.x *= child->parent->transform->vscale.x;
				child->transform->vscale.y *= child->parent->transform->vscale.y;
				child->transform->vscale.z *= child->parent->transform->vscale.z;
				child->parent->childs.erase(child->parent->childs.begin() + i);
			}
		}
		child->parent = parent;
		child->transform->vposition -= parent->transform->vposition;
		child->transform->vrotation.x -= parent->transform->vrotation.x;
		child->transform->vrotation.y -= parent->transform->vrotation.y;
		child->transform->vrotation.z -= parent->transform->vrotation.z;
		child->transform->vscale.x /= parent->transform->vscale.x;
		child->transform->vscale.y /= parent->transform->vscale.y;
		child->transform->vscale.z /= parent->transform->vscale.z;
		child->transform->UpdateMatrices();
	}
	child->parent = parent;
	parent->childs.push_back(child);

	return true;
}

void ModuleSceneIntro::setParentByID(UID parent_ID, GameObject * parent, GameObject * child)
{
	if(parent->ID == parent_ID)
		setParent(parent, child);
	else {
		for (int i = 0; i < parent->childs.size(); ++i) {
			setParentByID(parent_ID, parent->childs[i], child);
		}
	}
}

void ModuleSceneIntro::LoadScenePopUp() {
	vector<std::string> scenes;
	App->filesystem->GetAllFilesWithExtension(ASSETS_FOLDER, "scene", scenes);


	for (uint i = 0; i < scenes.size(); i++)
	{
		if (ImGui::MenuItem(scenes[i].c_str()))
		{
			std::string file;
			App->filesystem->SplitFilePath(scenes[i].c_str(), nullptr, &file, nullptr, true);
			LoadScene(&file);
		}
	}
}

void ModuleSceneIntro::SaveScenePopUp() {
	if (ImGui::BeginPopupModal("Save..", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static char name[100] = "";
		std::string sceneName;

		ImGui::InputText("", name, 100, ImGuiInputTextFlags_AutoSelectAll);

		ImGui::Separator();

		if (ImGui::Button("Save", ImVec2(140, 0)))
		{
			sceneName = name;
			SaveScene(sceneName);
			App->gui->savePopUp = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(140, 0)))
		{
			App->gui->savePopUp = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void ModuleSceneIntro::SaveScene(std::string fileName) {
	json file;
	string path = "Assets/" + fileName + ".scene";

	uint count = 0;
	SaveAllScene(root, file, count);
	file["Game Objects"]["Count"] = count;
	file["Game Objects"]["Name"] = fileName;

	ofstream stream;
	stream.open(path);
	stream << setw(4) << file << endl;
	stream.close();

}

uint ModuleSceneIntro::SaveAllScene(GameObject* root, json& file, uint& count) {
	char name[25];
	sprintf_s(name, 25, "Game Object %i", ++count);
	root->Save(name,file);

	for (int i = 0; i < root->childs.size(); ++i)
		SaveAllScene(root->childs[i], file,count);

	return count;
}


void ModuleSceneIntro::LoadScene(std::string* fileName) {
	CleanUp();
	root = new GameObject("Scene");

	json file;
	string path = "Assets/" + *fileName + ".scene";

	ifstream stream;
	stream.open(path);
	file = json::parse(stream);

	uint elements = file["Game Objects"]["Count"].get<uint>();
	uint count = 0;
	LoadAllScene(root, file,&elements, count);
	App->gui->inspector->active_gameObject = nullptr;

	stream.close();
}

uint ModuleSceneIntro::LoadAllScene(GameObject* root, json& file, uint* elements,uint& count) {
	GameObject* go = nullptr;
	if (count > 0) {
		char name[25];
		sprintf_s(name, 25, "Game Object %i", count);
		root->Load(name, file);
	}

	if (count < *elements) {
		go = new GameObject("");
		LoadAllScene(go, file, elements, ++count);
	}
	return count;
}



// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{

	Plane_ p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	for (int i = 0; i < root->childs.size(); ++i) {
		if (root->childs[i]->active && root->active)
			root->childs[i]->UpdateChilds();
	}
	

	Draw();


	return UPDATE_CONTINUE;
}
