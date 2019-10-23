#ifndef ENGINE_STATS_H
#define ENGINE_STATS_H
#include "Module.h"
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>

class WindowEngineStats : public Module
{
public:
	WindowEngineStats(Application* app, bool start_enabled = true);
	~WindowEngineStats();

	bool Start();
	bool Draw();
	bool Cleanup();
	bool isEnabled();

private:
	bool enabled;

	float FPS_array[50];
	float RAM_array[50];
	float beforeLastFramefps = 0;
	float values_offset = 0;


private:

	Engine::SystemInfo SysInfo;

	Engine::SoftwareInfo Softwareinfo_;
	Engine::MemoryHardware MemoryHardware_;
	Engine::GPUHardware GPUHardware_;
	Engine::ProcessorHardware ProcessorHardware_;

	//Software
	std::string CompilerVersion;
	std::string CppVersion;
	std::string SDLVersion;
	std::string VSCompilerVersion;
	std::string WindowsVersion;
	std::string CompilationTime;

	//HardWare
	uint32 RamSize;
	uint32 MemoryLoaded;
	uint64 PhysicalMemory;
	uint64 VirtualMemory;
	std::string CPUBrand;
	uint CPUCores;
	std::string CPUArchitecture;
	float RamSizeSDL;
	int GPUUsedRam;
	int GPUTotalRam;
	const GLubyte* GPUModel;
};
#endif
