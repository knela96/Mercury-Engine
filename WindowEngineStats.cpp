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

	
	
	GPUTotalRam = GPUHardware_.GetGPUTotalVRAM();
	GPUModel = (unsigned char*)GPUHardware_.GetGPUModel();

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
		sprintf(overlay, "avg %f", lastFramefps);

		for (int i = 0; i < 49; i++) {
			FPS_array[i] = FPS_array[i + 1];
		}
		FPS_array[49] = lastFramefps;

		sprintf(overlay, "fps %f", lastFramefps);
		
		ImGui::PlotHistogram ("Frames per second", FPS_array, IM_ARRAYSIZE(FPS_array), 0, overlay, 0.0f, 200.0f, ImVec2(0, 100));

		//-----------------------------------------------
		//                SoftwareInfo 
		//-----------------------------------------------

		ImGui::Spacing();
		ImGui::Spacing();
	
		ImGui::Text("Total RAM              %f", RamSizeSDL);
		ImGui::Spacing();
		ImGui::Text("VirtualMemory          %u", VirtualMemory);
		ImGui::Spacing();
		ImGui::Text("MemoryLoaded           %u", MemoryLoaded);
		ImGui::Spacing();
		ImGui::Text("PhysicalMemory         %u", PhysicalMemory);

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("CppVersion             %s", CppVersion.c_str());
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("SDLVersion             %s", SDLVersion.c_str());
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("VSCompilerVersion      %s", VSCompilerVersion.c_str());
		ImGui::Spacing();
		ImGui::Text("CompilerVersion        %s", CompilerVersion.c_str()); 
		ImGui::Spacing();
		ImGui::Text("WindowsVersion         %s", WindowsVersion.c_str());


		ImGui::Spacing();
		ImGui::Spacing(); 
		ImGui::Spacing();

		

		//-----------------------------------------------
		//                HardWareInfo 
		//-----------------------------------------------


		GPUUsedRam = GPUHardware_.GetGPUCurrentVRAM();

		ImGui::Text("CPU Brand             %s", CPUBrand.c_str());
		ImGui::Spacing();
		ImGui::Text("CPU Cores             %u", CPUCores);
		ImGui::Spacing();
		ImGui::Text("CPU Architechture     %s", CPUArchitecture.c_str());
		ImGui::Spacing();
		ImGui::Spacing();

		char RAMoverlay[32];
		float lastFrameRAM = GPUUsedRam;
		sprintf(RAMoverlay, "avg %f", lastFrameRAM);

		for (int i = 0; i < 49; i++) {
			RAM_array[i] = RAM_array[i + 1];
		}
		RAM_array[49] = lastFrameRAM;
		sprintf(RAMoverlay, "Bytes %f", lastFrameRAM);
		ImGui::PlotHistogram("RAM USED LAST SECOND", RAM_array, IM_ARRAYSIZE(RAM_array), 0, RAMoverlay, 0.0f, RamSizeSDL*1024, ImVec2(0, 100));
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("GPU Used RAM           %i", GPUUsedRam);
		ImGui::Spacing();
		ImGui::Text("GPU Total RAM          %i", GPUTotalRam);
		ImGui::Spacing();
		ImGui::Text("GPU Model              %c", GPUModel);

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
