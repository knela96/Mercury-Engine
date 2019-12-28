#include "Application.h"
#include "WindowGame.h"
#include "ModuleRenderer3D.h"
#include "Primitive.h"
#include "C_Camera.h"
#include "GameObject.h"
#include "C_Transform.h"

WindowGame::WindowGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;
}


WindowGame::~WindowGame()
{
}


bool WindowGame::Start()
{
	size_Game = { SCREEN_WIDTH,SCREEN_HEIGHT };

	//Init FrameBuffer
	fbo = new FrameBuffer();
	fbo->Start(size_Game.x, size_Game.y);
	ImGuizmo::Enable(true);
	return true;
}

update_status WindowGame::PreUpdate(float dt) {
	fbo->PreUpdate();
	return UPDATE_CONTINUE;
}

bool WindowGame::Draw()
{
	fbo->PostUpdate();
	if (App->gui->openGame) {
		ImGuiWindowFlags window_flags = 0;
		Primitive p;
		window_flags |= ImGuiWindowFlags_MenuBar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2, 2));

		ImGui::Begin("Game", &App->gui->openGame, window_flags);
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
			if (ImGui::BeginMenu("Render", true))
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

		if ((size_Game.x != newsize_Game.x || size_Game.y != newsize_Game.y)) {
			size_Game = newsize_Game;
			App->renderer3D->OnResize(newsize_Game.x, newsize_Game.y);
		}
		
		//-----------------------------------------------//
		ImGui::Image((void*)fbo->GetTexture(), ImVec2(size_Game.x, size_Game.y), ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource"))
			{
				if (payload->DataSize == sizeof(uint64))
				{
					uint64 ID = *(const uint64*)payload->Data;
					Resources* resource = App->resources->Get(ID);

					if (resource->type == ResourceType::ObjectR)
					{
						ifstream stream;
						stream.open(resource->resource_path);
						json file = json::parse(stream);

						uint elements = file["Game Objects"]["Count"].get<uint>();
						uint count = 0;
						App->scene_intro->LoadAllScene(App->scene_intro->root, file, &elements, count);
					}

				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}






	return true;
}

bool WindowGame::CleanUp()
{
	enabled = false; 
	fbo->CleanUp();
	delete fbo;
	fbo = nullptr;
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
