#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGUI.h"
#include "ModuleImporter.h"
#include "ModuleFileSystem.h"
#include "RNGenerator.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleGUI* gui;
	ModuleImporter* importer;
	ModuleFileSystem* filesystem;
	ModuleResources* resources;


	RNGenerator RandomNumberGenerator;
private:

	Timer   dt_timer;						//timer to calculate things related to delta time
	float   dt;								//delta time

	Timer frame_time;						//timer to calculate how match time does a frame take in miliseconds
	Timer last_sec_frame_time;				//How much time did last frame cost
	Timer ms_timer;							//timer to count miliseconds since App::Start()

	double frame_count = 0;					//number of frames since App::Init();


	float last_frame_ms;//last frame milisseconds
	float last_sec_frame_count = 0;//frames in last sec


	
	

	p2List<Module*> list_modules;			//All Application modules inside

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	const char * GetAppName() const;
	void SetAppName(const char * name);
	const char * GetOrganizationName() const;
	void SetOrganizationName(const char * name);
	bool CleanUp();
	int framerate_cap = 30;
	bool framerate_cap_activated = false;
	float prev_last_sec_frame_count = 0;	//frames the sec before last sec.
private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	std::string app_name;
	std::string organization_name;

};
extern Application* App;
