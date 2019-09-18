#ifndef MODULEGUI_H
#define MODULEGUI_H




#include "Module.h"
#include "Application.h"

class ModuleGUI : public Module {

public:


	ModuleGUI(Application* app, bool start_enabled = true);
	~ModuleGUI();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	

};

#endif