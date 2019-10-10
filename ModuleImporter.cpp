#include "ModuleImporter.h"
#include "Application.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "lib/Assimp/libx86/assimp.lib")


ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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

bool ModuleImporter::Draw(){
	return true;
}


// Called before quitting
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

			// copy vertices
			m.num_vertex = new_mesh->mNumVertices;
			m.vertex = new float[m.num_vertex * 3];
			memcpy(m.vertex, new_mesh->mVertices, sizeof(float) * m.num_vertex * 3);
			LOG("New mesh with %d vertices", m.num_vertex);

			// copy faces
			if (new_mesh->HasFaces() && ret)
			{
				m.num_index = new_mesh->mNumFaces * 3;
				m.index = new uint[m.num_index]; // assume each face is a triangle
				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
						ret = false;
					}
					else {
						memcpy(&m.index[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}
		}

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);


	m;

	return ret;
}