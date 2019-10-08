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

	
	/*float vertices[] = {
		-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 0.0f, 1.0f, 0.0f
	};*/

	//GLfloat vertices[] = {
	// .5f, .5f, .5f,  -.5f, .5f, .5f,  -.5f,-.5f, .5f,  .5f,-.5f, .5f,   // v0,v1,v2,v3 (front)
	// .5f, .5f, .5f,   .5f,-.5f, .5f,   .5f,-.5f,-.5f,  .5f, .5f,-.5f,   // v0,v3,v4,v5 (right)
	// .5f, .5f, .5f,   .5f, .5f,-.5f,  -.5f, .5f,-.5f, -.5f, .5f, .5f,   // v0,v5,v6,v1 (top)
	//-.5f, .5f, .5f,  -.5f, .5f,-.5f,  -.5f,-.5f,-.5f, -.5f,-.5f, .5f,   // v1,v6,v7,v2 (left)
	//-.5f,-.5f,-.5f,   .5f,-.5f,-.5f,   .5f,-.5f, .5f, -.5f,-.5f, .5f,   // v7,v4,v3,v2 (bottom)
	// .5f,-.5f,-.5f,  -.5f,-.5f,-.5f,  -.5f, .5f,-.5f,  .5f, .5f,-.5f    // v4,v7,v6,v5 (back)
	//};
	//GLuint indices[] = {
	// 0, 1, 2,   2, 3, 0,    // v0-v1-v2, v2-v3-v0 (front)
	// 4, 5, 6,   6, 7, 4,    // v0-v3-v4, v4-v5-v0 (right)
	// 8, 9,10,  10,11, 8,    // v0-v5-v6, v6-v1-v0 (top)
	//12,13,14,  14,15,12,    // v1-v6-v7, v7-v2-v1 (left)
	//16,17,18,  18,19,16,    // v7-v4-v3, v3-v2-v7 (bottom)
	//20,21,22,  22,23,20     // v4-v7-v6, v6-v5-v4 (back)
	//};

	//glGenBuffers(1, (GLuint*) &(vertex_id));
	//glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),0, GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);


	//glGenBuffers(1, (GLuint*) &(index_id));
	//glBindBuffer(GL_ARRAY_BUFFER, index_id);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();

	//glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);

	//glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glBindBuffer(GL_FRAMEBUFFER, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLfloat l = 1;

	GLfloat vertices[] =
	{
		 l, l, l,  -l, l, l,  -l,-l, l,  l,-l, l,   // v0,v1,v2,v3 (front)
		 l, l, l,   l,-l, l,   l,-l,-l,  l, l,-l,   // v0,v3,v4,v5 (right)
		 l, l, l,   l, l,-l,  -l, l,-l, -l, l, l,   // v0,v5,v6,v1 (top)
		-l, l, l,  -l, l,-l,  -l,-l,-l, -l,-l, l,   // v1,v6,v7,v2 (left)
		-l,-l,-l,   l,-l,-l,   l,-l, l, -l,-l, l,   // v7,v4,v3,v2 (bottom)
		 l,-l,-l,  -l,-l,-l,  -l, l,-l,  l, l,-l    // v4,v7,v6,v5 (back)
	};

	// normal array
	GLfloat normals[] =
	{
		 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,  // v0,v1,v2,v3 (front)
		 1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,  // v0,v3,v4,v5 (right)
		 0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,  // v0,v5,v6,v1 (top)
		-1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  // v1,v6,v7,v2 (left)
		 0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,  // v7,v4,v3,v2 (bottom)
		 0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1   // v4,v7,v6,v5 (back)
	};

	GLuint indices[] =
	{
		 0, 1, 2,   2, 3, 0,    // v0-v1-v2, v2-v3-v0 (front)
		 4, 5, 6,   6, 7, 4,    // v0-v3-v4, v4-v5-v0 (right)
		 8, 9,10,  10,11, 8,    // v0-v5-v6, v6-v1-v0 (top)
		12,13,14,  14,15,12,    // v1-v6-v7, v7-v2-v1 (left)
		16,17,18,  18,19,16,    // v7-v4-v3, v3-v2-v7 (bottom)
		20,21,22,  22,23,20     // v4-v7-v6, v6-v5-v4 (back)
	};

	GLfloat colors[] =
	{
		 1, 1, 1,   1, 1, 0,   1, 0, 0,   1, 0, 1,  // v0,v1,v2,v3 (front)
		 1, 1, 1,   1, 0, 1,   0, 0, 1,   0, 1, 1,  // v0,v3,v4,v5 (right)
		 1, 1, 1,   0, 1, 1,   0, 1, 0,   1, 1, 0,  // v0,v5,v6,v1 (top)
		 1, 1, 0,   0, 1, 0,   0, 0, 0,   1, 0, 0,  // v1,v6,v7,v2 (left)
		 0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0,  // v7,v4,v3,v2 (bottom)
		 0, 0, 1,   0, 0, 0,   0, 1, 0,   0, 1, 1   // v4,v7,v6,v5 (back)
	};


	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals) + 288, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);                             // copy vertices starting from 0 offest
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);                // copy normals after vertices
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), 288, colors);  // copy colours after normals
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// bind VBOs with IDs and set the buffer offsets of the bound VBOs
		// When buffer object is bound with its ID, all pointers in gl*Pointer()
		// are treated as offset instead of real pointer.
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);


	// enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	// before draw, specify vertex and index arrays with their offsets
	glNormalPointer(GL_FLOAT, 0, (void*)sizeof(vertices));
	glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(vertices) + sizeof(normals)));
	glVertexPointer(3, GL_FLOAT, 0, 0);
	//glTranslatef(X, Y, Z);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	// it is good idea to release VBOs with ID 0 after use.
	// Once bound with 0, all pointers in gl*Pointer() behave as real
	// pointer, so, normal vertex array operations are re-activated
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return UPDATE_CONTINUE;
}
