#ifndef WINDOW_FILESYSTEM
#define WINDOW_FILESYSTEM
#include "Module.h"
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
class weFolder;
class WindowFileSystem : public Module
{
public:
	WindowFileSystem(Application* app, bool start_enabled = true);
	~WindowFileSystem();

	bool Start();
	bool Draw();
	bool Cleanup();
	bool isEnabled();

	void CreateHierarchy(weFolder* folder, int &node_clicked, int index);
	void ShowFiles();

	int node_clicked = -1;
	int HierarchyFolderID = -1;

	weFolder* currentFolder = nullptr;
private:
	bool enabled;
};

#endif