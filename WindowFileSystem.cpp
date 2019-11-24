#include "Application.h"
#include "ModuleFileSystem.h"
#include "MathGeoLib/include/Math/Math.h"
#include "WindowFileSystem.h"
#include "imgui.h"
#include <cmath>
#include "ModuleFileSystem.h"
#include "GameObject.h"
#include "ModuleResources.h"

WindowFileSystem::WindowFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;
}


WindowFileSystem::~WindowFileSystem()
{
}


bool WindowFileSystem::Start()
{
	root = FolderContainer(ASSETS_FOLDER);

	root = App->filesystem->RecursiveGetFoldersFiles(root.path.c_str());

	currentFolder = &root;

	Texture* tex;
	tex = new Texture();
	App->material_importer->LoadTexture("Settings/Icons/folder-icon.png", tex->id, tex->size);
	icons.push_back(tex);

	tex = new Texture();
	App->material_importer->LoadTexture("Settings/Icons/file.png", tex->id, tex->size);
	icons.push_back(tex);


	return true;
}


bool WindowFileSystem::Draw()
{

	static int selection_mask = (1 << -1);
	int node_clicked = -1;

	int index = 0;

	if (App->gui->openFileSystem) {
		ImGui::Begin("Assets", &App->gui->openFileSystem);

		ImVec2 max = ImGui::GetWindowContentRegionMax();

		static float w = 200.0f;
		float h = max.y - 27;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::BeginChild("child1", ImVec2(w, h), false);
		CreateHierarchy(&root, node_clicked, index);
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::Button("vsplitter", ImVec2(2.0f, h));
		if (ImGui::IsItemActive())
			w += ImGui::GetIO().MouseDelta.x;
		ImGui::SameLine();
		ImGui::BeginChild("child2", ImVec2(0, h), false);
		ImGui::Text(" "); ImGui::SameLine(); ImGui::Text(currentFolder->localPath.c_str());
		ImGui::Separator();
		ShowFiles();
		ImGui::EndChild();
		ImGui::PopStyleVar();

		if (node_clicked != -1)
		{
			selection_mask = (1 << node_clicked);           // Click to single-select
		}


		ImGui::End();
	}
	return false;
}

void WindowFileSystem::ShowFiles() {
	for (int i = 0; i < currentFolder->folderchilds.size(); ++i) {
		DrawNode(&currentFolder->folderchilds.at(i));
	}
	for (int i = 0; i < currentFolder->filechilds.size(); ++i) {
		DrawNode(&currentFolder->filechilds.at(i));
	}
}

void WindowFileSystem::DrawNode(FolderContainer* node) {
	uint64 resourceID = 0;
	std::string texIcon("");
	uint ID = *GetNodeTexID(node, resourceID, texIcon);

	glBindTexture(GL_TEXTURE_2D, ID);
	ImGui::Image((ImTextureID)ID, ImVec2(40, 40), ImVec2(0, 1), ImVec2(1, 0));

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		ImGui::SetDragDropPayload(node->localPath.c_str(), &resourceID, sizeof(uint64));
		ImGui::Image((ImTextureID)ID, ImVec2(40, 40), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Text(node->localPath.c_str());

		ImGui::EndDragDropSource();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

uint* WindowFileSystem::GetNodeTexID(FolderContainer* node, uint64& resourceID, std::string texIcon) {

	//resourceID = App->resources->GetResource(node->path.c_str());
	//resourceID = App->resources->Get(-1)->ID;

	if (node->isFile == true)
		return &icons.at(0)->id;
	else
		return &icons.at(1)->id;
}

void WindowFileSystem::CreateHierarchy(FolderContainer* folder, int &node_clicked, int index) {

	++index;

	ModuleFileSystem* fileSystem = App->filesystem;

	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;

	/*if (parent == gameObject)  FIX TODO
		node_flags |= ImGuiTreeNodeFlags_Selected;*/


	if (folder->folderchilds.size() == 0)
		node_flags |= ImGuiTreeNodeFlags_Leaf;


	if (folder->ID == HierarchyFolderID)
		node_flags |= ImGuiTreeNodeFlags_Selected;



	if (ImGui::TreeNodeEx((void*)(intptr_t)folder->ID, node_flags, folder->localPath.c_str())) {

		if (ImGui::IsItemClicked()) {
			HierarchyFolderID = folder->ID;
			currentFolder = folder;
			node_clicked = index;
		}

		if (folder->folderchilds.size() > 0) {
			for (int i = 0; i < folder->folderchilds.size(); ++i) {
				CreateHierarchy(&folder->folderchilds[i], node_clicked, index);
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
