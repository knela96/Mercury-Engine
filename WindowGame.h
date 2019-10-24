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
};

#endif // WINDOW_GAME


