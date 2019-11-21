#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__
#include "Application.h"
#include "Globals.h"
#include "Module.h"

class MeshObject;
struct aiMesh;

class MeshImporter : public Module
{
public:
	MeshImporter(Application* app, bool start_enabled = true);
	~MeshImporter();

	MeshObject * ImportMeshResource(aiMesh * mesh, string * path, const char * fileName, uint ID, const aiScene * scene = nullptr);

	bool SaveMeshResource(const MeshObject * mesh, uint ID);

	MeshObject* LoadMeshResource(u64 ID);
};
#endif

