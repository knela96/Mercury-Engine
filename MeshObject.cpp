#include "MeshObject.h"
#include "Application.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "lib/Assimp/libx86/assimp.lib")


MeshObject::MeshObject()
{
}


MeshObject::~MeshObject()
{
}


bool MeshObject::Load(aiMesh* new_mesh) {
	bool ret = true;

	// copy vertices
	num_vertex = new_mesh->mNumVertices;
	vertices = new float[num_vertex * 3];

	memcpy(vertices, new_mesh->mVertices, sizeof(float) * num_vertex * 3);
	LOGC("New mesh with %d vertices", num_vertex);	

	//VERTICES
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// copy faces
	if (new_mesh->HasFaces() && ret)
	{
		num_index = new_mesh->mNumFaces * 3;
		indices = new uint[num_index]; // assume each face is a triangle

		for (uint i = 0; i < new_mesh->mNumFaces; ++i)
		{
			if (new_mesh->mFaces[i].mNumIndices != 3) {
				LOGC("WARNING, geometry face with != 3 indices!");
				ret = false;
			}
			else {
				memcpy(&indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}

	//INDICES
	glGenBuffers(1, &id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	

	return ret;
}

void MeshObject::CleanUp() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}