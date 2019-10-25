#include "About.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleGUI.h"

About::About(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;
}


About::~About()
{
}

bool About::Start()
{
	return true;
}

update_status About::Update()
{
	return UPDATE_CONTINUE;
}

update_status About::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status About::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool About::Draw() const
{
	if (App->gui->openAbout) {
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, -1), ImVec2(1000, -1));
		ImGui::Begin("About this engine", &App->gui->openAbout);

		
		ImGui::Spacing();

		ImGui::Text("               MERCURY ENGINE v 0.1              ");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		
		ImGui::Spacing();
		ImGui::Text("Game engine by: \n Eric Canela \n and \n Jaume Avinyo");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("3rd Party Libraries used:");
		ImGui::Spacing();
		ImGui::Text("SDL         version 2.0.0");

		ImGui::Text("Assimp      version 4.0.0");
	
		ImGui::Text("DevIl       version 1.8.0");
		
		ImGui::Text("Glew        version 7.0.0");
	
		ImGui::Text("ImGui       version 1.73.b");

		ImGui::Text("MathGeoLib  version 1.4.0");

		ImGui::Text("MathGeoLib  version 1.4.0");

		ImGui::Text("mmgr        version 0, cero releases");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Move: WASD");
		ImGui::Text("Rotate: ALT+LEFT_CLICK");
		ImGui::Text("TRANSLATE: ALT+MOUSE CENTER BUTTON");
		ImGui::Text("ZOOM: MOUSE WHEEL");


		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();


		ImGui::Text("Copyright <2019> <Mercury Engine Studio>");
		ImGui::Text("");
		ImGui::TextWrapped("Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.THE SOFTWARE IS PROVIDED -AS IS-, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
	

		ImGui::End();		
	}
	return false;
}

bool About::Cleanup()
{
	return true;
}

bool About::isEnabled()
{
	return enabled;
}
