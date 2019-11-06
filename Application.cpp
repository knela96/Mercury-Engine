#include "Application.h"
#include "managers/memory_manager.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);	
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	gui = new ModuleGUI(this);
	camera = new ModuleCamera3D(this);
	importer = new ModuleImporter(this);
	filesystem = new weFileSystem(this);

	
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(importer);
	
	// Scenes
	AddModule(scene_intro);

	//RECHECK
	AddModule(gui);
	AddModule(filesystem);

	// Renderer last!
	AddModule(renderer3D);

}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
		
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{

}

// ---------------------------------------------
void Application::FinishUpdate()
{

}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	
	//------------------------------------------------------------------------------------------------------------------------//PrepareUpdate();
	dt = dt_timer.ReadMs();
	frame_count++;
	last_sec_frame_count++;
	dt_timer.Start();
	frame_time.Start();
	//------------------------------------------------------------------------------------------------------------------------//PrepareUpdate();
	

	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PreUpdate(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->Update(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PostUpdate(dt);
		item = item->next;
	}


	//------------------------------------------------------------------------------------------------------------------------//FinishUpdate();
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / ms_timer.ReadSec();
	float seconds_since_startup = ms_timer.ReadSec();
	last_frame_ms = frame_time.Read();
	float frames_on_last_update = prev_last_sec_frame_count;

	//gui->console.AddLog(" Mercury Engine  ||  FPS: %f | Av.FPS: %f | Last Frame Ms: %f | FpsCap: %i | Vsync: off | DT %f", frames_on_last_update, avg_fps, last_frame_ms, framerate_cap, dt);
	
	//framerate caop
	float capped_ms = 1000 / framerate_cap;

	if (capped_ms > 0 && framerate_cap < 100 && last_frame_ms < capped_ms && framerate_cap_activated)
	{
		//MasterTimer t;
		SDL_Delay(capped_ms - last_frame_ms);
		//LOG("We waited for %d milliseconds and got back in %f", capped_ms - last_frame_ms, t.ReadMs());
	}

	
	//------------------------------------------------------------------------------------------------------------------------//FinishUpdate();
	
	
	
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}