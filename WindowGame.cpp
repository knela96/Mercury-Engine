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
		ImGui::SetCursorPos(ImVec2(img_offset.x, img_offset.y));
		img_corner = vec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y) + vec2(0, img_size.y);
		img_corner.y = SCREEN_HEIGHT - img_corner.y;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			gizmoOperation = ImGuizmo::OPERATION::ROTATE;
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
			gizmoOperation = ImGuizmo::OPERATION::SCALE;


		if (App->gui->inspector->active_gameObject != nullptr) {
			GameObject* gameObject = App->gui->inspector->active_gameObject;

			float4x4 viewM = App->camera->camera->frustum.ViewMatrix();
			viewM.Transpose();

			float4x4 projectionM = App->camera->camera->frustum.ProjectionMatrix().Transposed();
			float4x4 modelProjection = gameObject->mat2float4(gameObject->transform->localMatrix);

			ImGuizmo::SetDrawlist();
			cornerPos = vec2(img_corner.x, SCREEN_HEIGHT - img_corner.y - img_size.y);
			ImGuizmo::SetRect(img_corner.x, cornerPos.y, img_size.x, img_size.y);

			float modelPtr[16];
			memcpy(modelPtr, modelProjection.ptr(), 16 * sizeof(float));
			ImGuizmo::MODE finalMode = (gizmoOperation == ImGuizmo::OPERATION::SCALE ? ImGuizmo::MODE::LOCAL : gizmoMode);
			ImGuizmo::Manipulate(viewM.ptr(), projectionM.ptr(), gizmoOperation, finalMode, modelPtr);

			if (ImGuizmo::IsUsing())
			{
				float4x4 newMatrix;
				newMatrix.Set(modelPtr);
				modelProjection = newMatrix.Transposed();
				/*primer pillar quina variacio de x o y o z ha fer
			despres podrem fer un updatre matrices per actualitzar posicions
				*/
				//gameObject->GetComponent<C_Transform>()->SetGlobalMatrix(modelProjection);
				float3 Position;
				Quat Rotation;
				float3 Scale;

				modelProjection.Decompose(Position, Rotation, Scale);

				gameObject->transform->vposition = Position;
				gameObject->transform->vrotation = Rotation;
				gameObject->transform->vscale = Scale;

				gameObject->transform->UpdateMatrices();
			}

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
