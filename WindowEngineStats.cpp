#include "Application.h"
#include "WindowEngineStats.h"



WindowEngineStats::WindowEngineStats(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;
}


WindowEngineStats::~WindowEngineStats()
{
}


bool WindowEngineStats::Start()
{
	return true;
}


bool WindowEngineStats::Draw()
{
	if (App->gui->ShowFPS) {
		ImGui::Begin("EngineStats",&App->gui->ShowFPS);

		char overlay[32];
		float lastFramefps = App->prev_last_sec_frame_count;
		sprintf(overlay, "avg %f", lastFramefps);

		for (int i = 0; i < 19; i++) {
			FPS_array[i] = FPS_array[i + 1];
		}
		FPS_array[19] = lastFramefps;

		sprintf(overlay, "fps %f", lastFramefps);
		ImGui::PlotHistogram("Graph", FPS_array, IM_ARRAYSIZE(FPS_array), 0, overlay, -100.0f, 100.0f, ImVec2(0, 100));


		ImGui::End();
	}
	return false;
}

bool WindowEngineStats::Cleanup()
{
	return true;
}

bool WindowEngineStats::isEnabled()
{
	return enabled;
}
