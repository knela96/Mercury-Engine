#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleGUI.h"
#include "WindowGame.h"
#include "WindowHierarchy.h"
#include "WindowEngineStats.h"
#include "ModuleInput.h"
#include "About.h"


ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleGUI::~ModuleGUI()
{
}

// Called before render is available
bool ModuleGUI::Init()
{
	LOGC("Starting GUI module");
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;
	io->DisplaySize.x = SCREEN_WIDTH;             // set the current display width
	io->DisplaySize.y = SCREEN_HEIGHT;             // set the current display height here

	int width, height;
	unsigned char* pixels = NULL;
	io->Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	//INIT WINDOWS
	game = new WindowGame(App);
	about = new About(App);
	windows.push_back(game);
	windows.push_back(new WindowHierarchy(App));
	inspector = new WindowInspector(App);
	windows.push_back(new WindowEngineStats(App));
	windows.push_back(inspector);
	windows.push_back(about);

	return true;
}

bool ModuleGUI::Start() {
	list <Module*> ::iterator it;
	for (it = windows.begin(); it != windows.end(); ++it) {
		Module* m = *it;
		if (m != nullptr)
			m->Start();
	}
	return true;
}

// PreUpdate: clear buffer
update_status ModuleGUI::PreUpdate(float dt)
{
	list <Module*> ::iterator it;
	for (it = windows.begin(); it != windows.end(); ++it) {
		Module* m = *it;
		if (m != nullptr)
			m->PreUpdate(dt);
	}
	
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::PostUpdate(float dt)
{
	list <Module*> ::iterator it;
	for (it = windows.begin(); it != windows.end(); ++it) {
		Module* m = *it;
		if (m != nullptr)
			m->PostUpdate(dt);
	}
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
bool ModuleGUI::Draw()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	//Create Windows
	App->input->quit = !CreateMenuBar();	//Create Menu Bar

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		openConsole = !openConsole;
	}


	//Show Windows FIRST BUFFERS
	//ImGui::ShowStyleEditor();

	

	//ImGui::ShowDemoWindow(&show_demo_window);
	if (openConsole)
		ShowConsole();
	if (openWindowSettings)
		ShowWindowSettings();

	list <Module*> ::iterator it;
	for (it = windows.begin(); it != windows.end(); ++it) {
		Module* m = *it;
		if (m != nullptr && m->isEnabled())
			m->Draw();
	}

	//renderig UI
	ImGui::Render();

	//view
	glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
	glClearColor(0.25f, 0.25f, 0.25f, 0.25f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGUI::CleanUp()
{
	list <Module*> ::iterator it;
	for (it = windows.begin(); it != windows.end(); ++it) {
		Module* m = *it;
		if (m != nullptr) {
			m->CleanUp();
			delete m;
			m = nullptr;
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}

bool ModuleGUI::CreateMenuBar() {
	bool ret = true;
	bool opt_fullscreen = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);


	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}
			if (ImGui::BeginMenu("Open Recent")) {
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..")) {}
			if (ImGui::MenuItem("Exit")) {
				ret = false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Game", "", openGame)) {
				openGame = !openGame;
			}
			if (ImGui::MenuItem("Console", "F1", openConsole)) {
				openConsole = !openConsole;
			}
			if (ImGui::MenuItem("Hirearchy", "", openHirearchy)) {
				openHirearchy = !openHirearchy;
			}
			if (ImGui::MenuItem("Inspector", "", openInspector)) {
				openInspector = !openInspector;
			}
			if (ImGui::MenuItem("Engine Stats", "", ShowFPS)) {
				ShowFPS = !ShowFPS;
			}
			
			if (ImGui::MenuItem("Settings",NULL, openWindowSettings)) {
				openWindowSettings = !openWindowSettings;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Mercury Engine Repository")) {
				ShellExecuteA(NULL, "open", "https://github.com/knela96/Mercury-Engine", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Found any bug?")) {
				ShellExecuteA(NULL, "open", "https://github.com/knela96/Mercury-Engine/issues", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("About this Engine", "", openAbout)) {
				openAbout = !openAbout;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}else
		LOG("Cannot create Menu Bar");
	return ret;
}

void ModuleGUI::ShowConsole() {
	//Console Code
	console.Draw("Console", &openConsole);
}

void ModuleGUI::ShowWindowSettings() {

	ImGui::Begin("Settings",&openWindowSettings);


	/*ImGui::Text("Width:      ");
	ImGui::SameLine(); ImGui::PushID("screen_width");
	ImGui::SliderInt("px", &screen_width, 800, 3840); ImGui::PopID();

	ImGui::Text("Height:     ");
	ImGui::SameLine(); ImGui::PushID("screen_height");
	ImGui::SliderInt("px", &screen_height, 600, 2160); ImGui::PopID();*/

	if (ImGui::Checkbox("Fullscreen", &fullscreen))
		App->window->SetFullscreen(fullscreen);
	ImGui::SameLine();
	if (ImGui::Checkbox("Borderless", &borderless))
		App->window->SetBorderless(borderless);
	if (ImGui::Checkbox("Resizable", &resizable))
		App->window->SetResizable(resizable);

	//windowed full screen borderless, frame cap, vsync, inputs list (q teclas pulsas), mouse position, camera fov, resolution 
	
	ImGui::Checkbox("Frame rate cap activated", &App->framerate_cap_activated);
	ImGui::Text("Frame rate: "); ImGui::SameLine();
	ImGui::SliderInt("fps",&App->framerate_cap, 10, 60);

	/*ImGui::Checkbox("depth activated", &App->renderer3D->depth_active);
	ImGui::Checkbox("cullface active", &App->renderer3D->cullface_active);
	ImGui::Checkbox("lighting active", &App->renderer3D->lighting_active);
	ImGui::Checkbox("texture active", &App->renderer3D->texture_active);
	ImGui::Checkbox("wireframe active", &App->renderer3D->wireframe_active);*/
	SDL_GetMouseState(&MouseX,&MouseY);

	ImGui::Text("Mouse Position: x=%i , y=%i ",MouseX,MouseY);

	ImGui::End();
	//TODO add here options that change all modules variables (ver power point del principio)
}
