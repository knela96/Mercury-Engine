#include "Application.h"
#include "ModuleImporter.h"
#include "MeshObject.h"
#include "Primitive.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "lib/Assimp/libx86/assimp.lib")


ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;
}


ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Start(){
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	Load("warrior.fbx");
	return true;
}

update_status ModuleImporter::PreUpdate(float dt){

	return UPDATE_CONTINUE;
}

update_status ModuleImporter::PostUpdate(float dt){

	return UPDATE_CONTINUE;
}


bool ModuleImporter::CleanUp()
{ 
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}

bool ModuleImporter::Load(const char* path) {
	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int j = 0; j < scene->mNumMeshes && ret; ++j) {
			aiMesh* new_mesh = scene->mMeshes[j];

			MeshObject* mObject = new MeshObject();
			ret = mObject->Load(new_mesh);
			meshes.push_back(mObject);
		}

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);

	return ret;
}

bool ModuleImporter::Draw() {
	
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	for (int i = 0; i < meshes.size(); ++i) {
		MeshObject* mObject = meshes[i];

		// enable vertex arrays
		/*glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);*/
		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, mObject->id_vertex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mObject->id_index);

		// before draw, specify vertex and index arrays with their offsets
		/*glNormalPointer(GL_FLOAT, 0, (void*)sizeof(mObject->vertices));
		glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(mObject->vertices) + sizeof(mObject->normals)));*/
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glDrawElements(GL_TRIANGLES, mObject->num_index, GL_UNSIGNED_INT, NULL); //Render objects

		glPopMatrix();
		glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vertices buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind indices buffer

		glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
		//glDisableClientState(GL_COLOR_ARRAY);
		//glDisableClientState(GL_NORMAL_ARRAY);
	}
	return true;
}