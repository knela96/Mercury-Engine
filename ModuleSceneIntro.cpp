#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "MathGeoLib/include/Geometry/Sphere.h"


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

	float3 posv3_1 = (1, 1, 1);
	float3 posv3_2 = (2, 2, 2);

	Sphere sphere1 = Sphere(posv3_1,2.0);
	Sphere sphere2 = Sphere(posv3_2, 7.0);

	float distance = math::Distance(sphere1.pos, sphere2.pos);
	float distancepow2 = pow(distance, 2);
	float sumrads = sphere1.r + sphere2.r;
	float radpow2 = pow(sumrads, 2);

	if (distancepow2 < radpow2) {
		LOG("SPHERES INTERSECT")
	}
	/*sphere1.r = 2;
	sphere2.r = 3;

	sphere1.pos = posv3_1;
	sphere2.pos = posv3_2;*/
	
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

	return UPDATE_CONTINUE;
}

