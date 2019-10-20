#include "Application.h"
#include "WindowGame.h"
#include "ModuleRenderer3D.h"
#include "Primitive.h"


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
	if (App->gui->openGame) {
		fbo->PreUpdate();
	}
	return UPDATE_CONTINUE;
}

bool WindowGame::Draw()
{
	if (App->gui->openGame) {
		fbo->PostUpdate();
		ImGuiWindowFlags window_flags = 0;
		Primitive p;
		window_flags |= ImGuiWindowFlags_MenuBar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2, 2));

		ImGui::Begin("Game", NULL, window_flags);
		if (ImGui::BeginMenuBar())
		{
			
			if (ImGui::BeginMenu("GameObject", true))
			{
				ImGui::PushItemWidth(300);
				if (ImGui::MenuItem("Plane")) { p.DrawObj(Primitive_Plane); }
				if(ImGui::MenuItem("Cube")){ p.DrawObj(Primitive_Cube); }
				if(ImGui::MenuItem("Cone")){ p.DrawObj(Primitive_Cone); }
				if(ImGui::MenuItem("Cylinder")){ p.DrawObj(Primitive_Cylinder); }
				if(ImGui::MenuItem("Sphere")){ p.DrawObj(Primitive_Sphere); }
				ImGui::EndMenu();
			}

			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 85);//Put the button on the right
			if (ImGui::BeginMenu("Gizmos", true))
			{
				ImGui::PushItemWidth(300);
				ImGui::MenuItem("Depth", NULL, &App->renderer3D->depth_active);
				ImGui::MenuItem("Cull Face", NULL, &App->renderer3D->cullface_active);
				ImGui::MenuItem("Lighting", NULL, &App->renderer3D->lighting_active);
				ImGui::MenuItem("Color", NULL, &App->renderer3D->color_active);
				ImGui::MenuItem("Texture", NULL, &App->renderer3D->texture_active);
				ImGui::MenuItem("Wireframe", NULL, &App->renderer3D->wireframe_active);
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		position = ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
		newsize_Game = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);

		if (size_Game.x != newsize_Game.x || size_Game.y != newsize_Game.y) {
			size_Game = newsize_Game;
			App->renderer3D->OnResize(size_Game.x, size_Game.y);
		}


		ImGui::Image((void*)fbo->GetTexture(), ImVec2(size_Game.x, size_Game.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();
	}
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

bool WindowGame::mouseHover() {
	if ((App->input->GetMouseX() >= position.x && App->input->GetMouseX() <= position.x + newsize_Game.x) && (App->input->GetMouseY() - 40 >= position.y && App->input->GetMouseY() - 40 <= position.y + newsize_Game.y)) {
		return true;
	}
	else {
		return false;
	}
}
