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

	MeshObject*	ImportMeshResource(const aiMesh* mesh, unsigned long long ID, const char* file, const char* name);

	bool	SaveMeshResource(const MeshObject*);
	MeshObject* LoadMeshResource(u64 ID);
};
#endif

