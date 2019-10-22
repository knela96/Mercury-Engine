#pragma once
#ifndef INSPECTOR_H
#define INSPECTOR_H
#include "Module.h"
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>

class GameObject;

class WindowInspector : public Module
{
public:
	WindowInspector(Application* app, bool start_enabled = true);
	~WindowInspector();

	bool Start();
	bool Draw();
	bool Cleanup();
	bool isEnabled();

private:
	bool enabled = false;
	bool unFold = false;
	bool open_transform = true;
	bool open_normals = true;
	bool open_mesh_info = true;
	bool open_material = true;
public:
	float face_lenght;
	float vertex_lenght;
	Color face_color;
	Color vertex_color;

	GameObject* active_gameObject = nullptr;
};
#endif
