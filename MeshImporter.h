#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__
#include "Application.h"
#include "Globals.h"
#include "Module.h"

class Mesh_R;
struct aiMesh;

class MeshImporter : public Module
{
public:
	MeshImporter(Application* app, bool start_enabled = true);
	~MeshImporter();

	Mesh_R* ImportMeshResource(aiMesh* mesh, const char* path, const char* fileName, UID ID);

	bool SaveMeshResource(const Mesh_R * mesh, UID ID);

	Mesh_R* LoadMeshResource(UID ID);
};
#endif

