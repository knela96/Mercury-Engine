#ifndef WINDOW_FILESYSTEM
#define WINDOW_FILESYSTEM
#include "Module.h"
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "FolderContainer.h"

#include <GL/glew.h>

struct Texture;

class WindowFileSystem : public Module
{
public:
	WindowFileSystem(Application* app, bool start_enabled = true);
	~WindowFileSystem();

	bool Start();
	void UpdateAssets();
	bool Draw();
	void CreateHierarchy(FolderContainer* folder, int & node_clicked, int index);
	bool Cleanup();
	bool isEnabled();

	void ShowFiles();

	void DrawNode(FolderContainer * node);

	uint * GetNodeTexID(FolderContainer * node, uint64 & resourceID, std::string texIcon);

	int node_clicked = -1;
	int HierarchyFolderID = -1;


	FolderContainer root;

	std::vector<FolderContainer> cointainer;

	FolderContainer* currentFolder;

	std::vector<Texture*> icons;

private:
	bool enabled;
};

#endif