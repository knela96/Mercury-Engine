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
	update_status PreUpdate();
	update_status PostUpdate();
	bool Draw();
	bool Cleanup();
	bool isEnabled();

private:
	bool enabled;
	FrameBuffer* fbo = nullptr;

public:
	ImVec2 size_Game;
	ImVec2 newsize_Game;
};

#endif // WINDOW_GAME


