#include "Application.h"
#include "WindowEngineStats.h"
#include "SDL_opengl.h"


WindowEngineStats::WindowEngineStats(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;


	Softwareinfo_ = SysInfo.GetSoftwareInfo();

	CompilerVersion = Softwareinfo_.GetCppCompilerVersion();
	CppVersion = Softwareinfo_.GetCPPNumericalVersion();
	SDLVersion = Softwareinfo_.GetSDLVersion();
	VSCompilerVersion = Softwareinfo_.GetVSCompilerVersion();
	WindowsVersion = Softwareinfo_.GetWindowsVersion();
	CompilationTime = Softwareinfo_.GetCompilationTime();

	MemoryHardware_ = SysInfo.GetMemoryHardwareInfo();

	RamSizeSDL = MemoryHardware_.GetRAMSizeFromSDL();
	MemoryLoaded = MemoryHardware_.GetPercentageOfMemoryLoad();
	PhysicalMemory = MemoryHardware_.GetPhysicalMemory();
	VirtualMemory = MemoryHardware_.GetVirtualMemory();
	

	ProcessorHardware_ = SysInfo.GetCPUHardwareInfo();

	GPUHardware_ = SysInfo.GetGPUHardwareInfo();

	
	GPUHardware_.RecalculateGPUParameters();
	GPUTotalRam = GPUHardware_.GetGPUTotalVRAM();
	GPUModel = GPUHardware_.GetGPUModel().c_str();

	CPUBrand = ProcessorHardware_.GetCPUBrand();
	CPUCores = ProcessorHardware_.GetCPUCores();
	CPUArchitecture = ProcessorHardware_.GetCPUArchitecture();
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
		ImGui::Spacing();
		ImGui::Spacing();
		char overlay[32];
		float lastFramefps = App->prev_last_sec_frame_count;
		sprintf(overlay, "AVG %.0f", lastFramefps);

		for (int i = 0; i < 49; i++) {
			FPS_array[i] = FPS_array[i + 1];
		}
		FPS_array[49] = lastFramefps;

		sprintf(overlay, "FPS %.0f", lastFramefps);
		
		ImGui::PlotHistogram ("FPS", FPS_array, IM_ARRAYSIZE(FPS_array), 0, overlay, 0.0f, 200.0f, ImVec2(0, 100));




		char RAMoverlay[32];
		float lastFrameRAM = GPUUsedRam;
		sprintf(RAMoverlay, "RAM %.0f", lastFrameRAM);

		for (int i = 0; i < 49; i++) {
			RAM_array[i] = RAM_array[i + 1];
		}
		RAM_array[49] = lastFrameRAM;
		sprintf(RAMoverlay, "%.0f MB", lastFrameRAM);
		ImGui::PlotHistogram("RAM", RAM_array, IM_ARRAYSIZE(RAM_array), 0, RAMoverlay, 0.0f, RamSizeSDL * 1024, ImVec2(0, 100));
		//-----------------------------------------------
		//                SoftwareInfo 
		//-----------------------------------------------

		ImGui::Spacing();
		ImGui::Spacing();
		
		ImGui::Columns(2,NULL,false);
		ImGui::SetColumnWidth(0, 150);
		ImGui::Text("Total RAM:");
		ImGui::NextColumn();
		ImGui::Text("%.2f GB", RamSizeSDL);
		ImGui::NextColumn();
		ImGui::Text("VirtualMemory:");
		ImGui::NextColumn();
		ImGui::Text("%u GB", VirtualMemory);
		ImGui::NextColumn();
		ImGui::Text("MemoryLoaded:");
		ImGui::NextColumn();
		ImGui::Text("%u %%", MemoryLoaded);
		ImGui::NextColumn();
		ImGui::Text("PhysicalMemory:");
		ImGui::NextColumn();
		ImGui::Text("%u GB", PhysicalMemory);
		ImGui::NextColumn();

		ImGui::NewLine();

		ImGui::Text("CppVersion:");
		ImGui::NextColumn();
		ImGui::NewLine();
		ImGui::Text("%s", CppVersion.c_str());
		ImGui::NextColumn();
		ImGui::NewLine();
		ImGui::Text("SDLVersion:");
		ImGui::NextColumn(); 
		ImGui::NewLine();
		ImGui::TextWrapped("%s", SDLVersion.c_str());
		ImGui::NextColumn();
		ImGui::NewLine();
		ImGui::Text("VSCompilerVersion:");
		ImGui::NextColumn();
		ImGui::NewLine();
		ImGui::TextWrapped("%s", VSCompilerVersion.c_str());
		ImGui::NextColumn();
		ImGui::Text("CompilerVersion:");
		ImGui::NextColumn();
		ImGui::Text("%s", CompilerVersion.c_str());
		ImGui::NextColumn();
		ImGui::Text("WindowsVersion:");
		ImGui::NextColumn();
		ImGui::Text("%s", WindowsVersion.c_str());
		ImGui::NextColumn(); 
		ImGui::NewLine();

		//-----------------------------------------------
		//                HardWareInfo 
		//-----------------------------------------------


		GPUUsedRam = GPUHardware_.GetGPUCurrentVRAM();

		ImGui::Text("CPU Brand:");
		ImGui::NextColumn();
		ImGui::NewLine();
		ImGui::TextWrapped("%s", CPUBrand.c_str());
		ImGui::NextColumn();
		ImGui::Text("CPU Cores:");
		ImGui::NextColumn();
		ImGui::Text("%u", CPUCores);
		ImGui::NextColumn();
		ImGui::Text("CPU Architechture:");
		ImGui::NextColumn();
		ImGui::Text("%s", CPUArchitecture.c_str());
		ImGui::NextColumn();

		ImGui::NewLine();
		
		/*ImGui::Text("GPU Total RAM          %i gb", GPUTotalRam);
		ImGui::Spacing();*/
		ImGui::Text("GPU Model:");
		ImGui::NextColumn();
		ImGui::NewLine();
		ImGui::TextWrapped("%s", GPUModel.c_str());
		ImGui::NextColumn();
		ImGui::Text("GPU VRAM:");
		ImGui::NextColumn();
		ImGui::Text("%i MB", GPUTotalRam);

		ImGui::Columns(1, NULL, false);
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
