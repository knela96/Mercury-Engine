#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
//#include "MathGeoLib/include/Geometry/Sphere.h"
#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	float vertices[] = {
		-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 0.0f, 1.0f, 0.0f
	};
	
	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices * 3, vertices, GL_STATIC_DRAW);


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();


	//glLineWidth(2.0f);

	//glBegin(GL_TRIANGLES);
	//glVertex3f(0, 0, 1.0);
	//glVertex3f(1.0, 0, 1.0);
	//glVertex3f(0, 1.0, 1.0);

	//glVertex3f(0, 1.0, 1.0);
	//glVertex3f(1.0,0, 1.0);
	//glVertex3f(1.0, 1.0, 1.0);

	//glVertex3f(0, 0, 1.0);
	//glVertex3f(0, 1, 1.0);
	//glVertex3f(0, 0, 0.0);

	//glVertex3f(0, 0, 0);
	//glVertex3f(0, 1.0, 1.0);
	//glVertex3f(0, 1.0, 0);
	//glEnd();

	//glLineWidth(1.0f);

	//

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	glDisableClientState(GL_VERTEX_ARRAY);

	return UPDATE_CONTINUE;
}
