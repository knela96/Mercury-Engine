#include "Application.h"
#include "WindowGame.h"
#include "ModuleRenderer3D.h"


WindowGame::WindowGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;
}


WindowGame::~WindowGame()
{
}


bool WindowGame::Start()
{
	size_Game = { 1280,720 };

	//Init FrameBuffer
	fbo = new FrameBuffer();
	fbo->Start(size_Game.x, size_Game.y);
	return true;
}

update_status WindowGame::PreUpdate(float dt) {
	fbo->PreUpdate();
	return UPDATE_CONTINUE;
}

bool WindowGame::Draw()
{
	fbo->PostUpdate();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Game");
	ImVec2 position = ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
	newsize_Game = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);

	if (size_Game.x != newsize_Game.x || size_Game.y != newsize_Game.y){
		size_Game = newsize_Game;
		App->renderer3D->OnResize(size_Game.x, size_Game.y);
	}

	ImGui::Image((void*)fbo->GetTexture(), ImVec2(size_Game.x, size_Game.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End(); 
	ImGui::PopStyleVar();
	return false;
}

bool WindowGame::Cleanup()
{
	enabled = false;
	return true;
}

bool WindowGame::isEnabled()
{
	return enabled;
}
