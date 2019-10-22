#include "Application.h"
#include "WindowEngineStats.h"



WindowEngineStats::WindowEngineStats(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;


	Softwareinfo_ = SysInfo.GetSoftwareInfo();

	CompilerVersion = Softwareinfo_.GetCppCompilerVersion();
	CppVersion = Softwareinfo_.GetCPPNumericalVersion();
	SDLVersion = Softwareinfo_.GetSDLVersion();
	VSCompilerVersion = Softwareinfo_.GetVSCompilerVersion();
	WindowsVersion = Softwareinfo_.GetWindowsVersion();


	MemoryHardware_ = SysInfo.GetMemoryHardwareInfo();

	RamSizeSDL = MemoryHardware_.GetRAMSizeFromSDL();
	MemoryLoaded = MemoryHardware_.GetPercentageOfMemoryLoad();
	PhysicalMemory = MemoryHardware_.GetPhysicalMemory();
	VirtualMemory = MemoryHardware_.GetVirtualMemory();

	ProcessorHardware_ = SysInfo.GetCPUHardwareInfo();

	GPUHardware_ = SysInfo.GetGPUHardwareInfo();

	CPUBrand = ProcessorHardware_.GetCPUBrand();
	CPUCores = ProcessorHardware_.GetCPUCores();
	/*

	LOGC("RamSize = %f", RamSizeSDL);
	LOGC("MemoryLoaded % = %u", MemoryLoaded);
	LOGC("PhysicalMemory = %u", PhysicalMemory);
	LOGC("VirtualMemory = %u", VirtualMemory);

    */
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

		//-----------------------------------------------
		//                FPS
		//-----------------------------------------------
		char overlay[32];
		float lastFramefps = App->prev_last_sec_frame_count;
		sprintf(overlay, "avg %f", lastFramefps);

		for (int i = 0; i < 19; i++) {
			FPS_array[i] = FPS_array[i + 1];
		}
		FPS_array[19] = lastFramefps;

		sprintf(overlay, "fps %f", lastFramefps);
		
		ImGui::PlotLines ("Frames per second", FPS_array, IM_ARRAYSIZE(FPS_array), 0, overlay, -100.0f, 100.0f, ImVec2(0, 100));

		//-----------------------------------------------
		//                SoftwareInfo 
		//-----------------------------------------------
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Memoriae temere Acces Thecuites     %f", RamSizeSDL);
		ImGui::Spacing();
		ImGui::Text("VirtualMemory                       %u", VirtualMemory);
		ImGui::Spacing();
		ImGui::Text("MemoryLoaded                        %u", MemoryLoaded);
		ImGui::Spacing();
		ImGui::Text("PhysicalMemory                      %u", PhysicalMemory);

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("CppVersion                          %s", CppVersion.c_str());
		ImGui::Spacing();
		ImGui::Text("CompilerVersion                     %s", CompilerVersion.c_str());
		ImGui::Spacing();
		ImGui::Text("SDLVersion                          %s", SDLVersion.c_str());
		ImGui::Spacing();
		ImGui::Text("VSCompilerVersion                   %s", VSCompilerVersion.c_str());
		ImGui::Spacing();
		ImGui::Text("WindowsVersion                      %s", WindowsVersion.c_str());


		ImGui::Spacing();
		ImGui::Spacing(); 
		ImGui::Spacing();

		ImGui::Text("CPU Brand                     %s", CPUBrand.c_str());
		ImGui::Spacing();
		ImGui::Text("CPU Cores                     %u", CPUCores);


		ImGui::Spacing(); 
		ImGui::Spacing();
		ImGui::Spacing();


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
