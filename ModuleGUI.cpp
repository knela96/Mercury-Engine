#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleGUI.h"


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
	 // Build and load the texture atlas into a texture
	 // (In the examples/ app this is usually done within the ImGui_ImplXXX_Init() function from one of the demo Renderer)
	int width, height;
	unsigned char* pixels = NULL;
	io->Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	///ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();
	return true;
}

// PreUpdate: clear buffer
update_status ModuleGUI::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update(float dt)
{

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	//Create Windows
	CreateMenuBar();	//Create Menu Bar


	//Show Windows
	ImGui::ShowDemoWindow(&show_demo_window);
	if (openConsole)
		ShowConsole();
		
	if (openWindowSettings)
		ShowWindowSettings();



	test_io = io;


	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleGUI::PostUpdate(float dt)
{
	// RENDERING


	//renderig UI
	ImGui::Render();

	//view
	glViewport(0, 0, (int)test_io->DisplaySize.x, (int)test_io->DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGUI::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}

void ModuleGUI::CreateMenuBar() {

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
			if (ImGui::MenuItem("Console", "F1", openConsole)) {
				openConsole = !openConsole;
			}
			if (ImGui::MenuItem("Settings")) {
				openWindowSettings = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About..")) {
				ShellExecuteA(NULL,"open","www.google.com",NULL,NULL,SW_SHOWNORMAL);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}else
		LOG("Cannot create Menu Bar");
}

void ModuleGUI::ShowConsole() {
	//Console Code
	console.Draw("Console", &openConsole);
}

void ModuleGUI::ShowWindowSettings() {

	ImGui::Begin("Settings",&openWindowSettings);

	ImGui::Text("Width: ");
	ImGui::SameLine();
	ImGui::SliderInt("px", &screen_width, 800, 2160);
	if (ImGui::Checkbox("Fullscreen", &fullscreen))
		App->window->SetFullscreen(fullscreen);
	ImGui::SameLine();
	if (ImGui::Checkbox("Borderless", &borderless))
		App->window->SetBorderless(borderless);
	ImGui::End();
}
