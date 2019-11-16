#include "Application.h"
#include "ModuleFileSystem.h"

#include "WindowFileSystem.h"
#include "imgui.h"


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
	if (App->gui->openFileSystem) {
		ImGui::Begin("Assets", &App->gui->openFileSystem);

		for (int i = 0; i < App->filesystem->weFoldersArray.size(); i++) {
			ImGui::TreeNodeEx(App->filesystem->weFoldersArray[i]->weFolderName.data());
			//for(int j = 0;)
		}
			/*if (ImGui::CollapsingHeader("Help")) {
				
			}*/
		
		


		ImGui::End();
	}
	return false;
}

bool WindowFileSystem::Cleanup()
{
	return true;
}

bool WindowFileSystem::isEnabled()
{
	return enabled;
}
