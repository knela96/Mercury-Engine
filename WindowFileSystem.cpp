#include "Application.h"
#include "ModuleFileSystem.h"
#include "MathGeoLib/include/Math/Math.h"
#include "WindowFileSystem.h"
#include "imgui.h"
#include <cmath>

WindowFileSystem::WindowFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;
}


WindowFileSystem::~WindowFileSystem()
{
}


bool WindowFileSystem::Start()
{
	return true;
}


bool WindowFileSystem::Draw()
{

	static int selection_mask = (1 << -1);
	int node_clicked = -1;

	int index = 0;

	if (App->gui->openFileSystem) {
		ImGui::Begin("Assets", &App->gui->openFileSystem);
		ImGui::Columns(2, NULL, true);
		//ImGui::SetColumnWidth(0, 100);
		//ImGui::SetColumnWidth(1,ImGui::GetContentRegionAvailWidth()-100);
		CreateHierarchy(App->filesystem->RootFolder, node_clicked, index);
		ImGui::NextColumn();
		//Visually Select Node in Hirearchy
		ShowFiles();
		if (node_clicked != -1)
		{
			if (ImGui::GetIO().KeyCtrl)
				selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
			else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
				selection_mask = (1 << node_clicked);           // Click to single-select
		}
		


		ImGui::End();
	}
	return false;
}

void WindowFileSystem::ShowFiles() {
	
	//static bool selected[5 * 5] = { true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
	//for (int i = 0; i < 5 * 5; i++)
	//{
	//	ImGui::PushID(i);
	//	if (ImGui::Selectable(currentFolder->childFiles[i]->weName.c_str()/*"Sailor"*/, &selected[i], 0, ImVec2(70, 50)))
	//	{
	//		currentFolder->childFiles.size();
	//		//currentFolder->childFiles[i]->weName.c_str();
	//		// Note: We _unnecessarily_ test for both x/y and i here only to silence some static analyzer. The second part of each test is unnecessary.
	//		
	//	}
	//	if ((i % 4) < 3) ImGui::SameLine();
	//	ImGui::PopID();
	//}
	
}

void WindowFileSystem::CreateHierarchy(weFolder* folder,int &node_clicked,int index) {
	
	++index;

	ModuleFileSystem* fileSystem = App->filesystem;

	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;

	/*if (parent == gameObject)  FIX TODO
		node_flags |= ImGuiTreeNodeFlags_Selected;*/

	if (folder->childFolders.size() == 0)
		node_flags |= ImGuiTreeNodeFlags_Leaf;

	
	if (folder->ID == HierarchyFolderID)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	
	

	if (ImGui::TreeNodeEx((void*)(intptr_t)folder->ID, node_flags, folder->FolderName.c_str())) {

		if (ImGui::IsItemClicked()) {
			HierarchyFolderID = folder->ID;
			currentFolder = folder;
			node_clicked = index;
		}

		if (folder->childFolders.size() > 0) {
			for (int i = 0; i < folder->childFolders.size();  ++i) {
				CreateHierarchy(folder->childFolders[i], node_clicked, index);
			}
		}

		

		ImGui::TreePop();
	}
}
bool WindowFileSystem::Cleanup()
{
	return true;
}

bool WindowFileSystem::isEnabled()
{
	return enabled;
}
