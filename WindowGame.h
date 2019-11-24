#pragma once
#ifndef WINDOWGAME_H
#define WINDOWGAME_H
#include "Module.h"
#include "Application.h"
#include "FrameBuffer.h"

class WindowGame : public Module
{
public:
	WindowGame(Application* app, bool start_enabled = true);
	~WindowGame();

	bool Start();
	update_status PreUpdate(float dt);
	bool Draw();
	bool CleanUp();
	bool isEnabled();

	bool mouseHover();

private:
	bool enabled;

public:
	FrameBuffer* fbo = nullptr;
	ImVec2 size_Game;
	ImVec2 position;
	ImVec2 newsize_Game;


	//-----------------------------//guizmo
	//Variables for gizmo handling
	ImGuizmo::OPERATION gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE gizmoMode = ImGuizmo::MODE::WORLD;

	void HandleGuizmo();


	vec2 win_size;
	vec2 img_offset;

	vec2 img_corner;
	vec2 img_size;
	vec2 cornerPos;

	//-----------------------------//guizmo_end
};

#endif // WINDOW_GAME


