#include "Application.h"
#include "MeshImporter.h"
#include "ModuleImporter.h"
#include "MeshObject.h"
#include "Mesh_R.h"

#include <algorithm>



MeshImporter::MeshImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


MeshImporter::~MeshImporter()
{
}

Mesh_R* MeshImporter::ImportMeshResource(aiMesh* mesh, const char* path, const char* fileName, UID ID)
{
	Mesh_R* newmesh = new Mesh_R();
	uint size_weights = 0;
	map <uint, VertexBoneData> _map;
	map <uint, VertexBoneData> __map;

	newmesh->buffersSize[vertices_size] = mesh->mNumVertices;
	newmesh->_vertices = new float[mesh->mNumVertices * 3];
	memcpy(newmesh->_vertices, mesh->mVertices, sizeof(float) * mesh->mNumVertices * 3);

	if (mesh->HasBones()) {

		for (uint i = 0; i < mesh->mNumBones; i++) {
			size_weights += mesh->mBones[i]->mNumWeights;
		}

		newmesh->buffersSize[bones_size] = mesh->mNumBones;
		newmesh->buffersSize[weights_size] = size_weights;
		newmesh->_bones = new uint[size_weights];
		newmesh->_weights_indices = new uint[size_weights];
		newmesh->_weights = new float[size_weights];

		size_weights = 0;

		uint size_weights = 0;
		VertexBoneData data;
		for (uint i = 0; i < mesh->mNumBones; i++) {
			for (uint j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
				data = { i, mesh->mBones[i]->mWeights[j].mVertexId,mesh->mBones[i]->mWeights[j].mWeight };
				_map[size_weights++] = data;
			}
		}

		uint c = 0;
		vector<VertexBoneData*> temp;
		uint i = 0;
		for (int j = 0; j < _map.size(); ++j) {
			VertexBoneData* data = &_map[j];

			if (std::find(temp.begin(), temp.end(), data) != temp.end() && data->weights > 0)
				continue;

			__map[c] = *data;

			memcpy(&newmesh->_bones[c], &__map[c].BoneID, sizeof(uint));
			memcpy(&newmesh->_weights_indices[c], &__map[c].indices, sizeof(uint));
			memcpy(&newmesh->_weights[c], &__map[c].weights, sizeof(float));
			c++;
			temp.push_back(data);
			for (i = 0; i < _map.size(); i++)
			{
				uint index = _map.at(i).indices;
				uint id = _map.at(i).BoneID;
				if (data->indices == index && id != data->BoneID && _map.at(i).weights > 0) {
					__map[c] = _map.at(i);

					memcpy(&newmesh->_bones[c], &__map[c].BoneID, sizeof(uint));
					memcpy(&newmesh->_weights_indices[c], &__map[c].indices, sizeof(uint));
					memcpy(&newmesh->_weights[c], &__map[c].weights, sizeof(float));
					c++;
					temp.push_back(&_map.at(i));
				}
			}
		}

	}
	else {
		newmesh->buffersSize[bones_size] = 0;
		newmesh->buffersSize[weights_size] = 0;
	}

	if (mesh->HasFaces())
	{
		newmesh->buffersSize[indices_size] = mesh->mNumFaces;
		newmesh->_indices = new uint[mesh->mNumFaces * 3];
		for (uint i = 0; i < mesh->mNumFaces; i++)
		{
			memcpy(&newmesh->_indices[i * 3], &mesh->mFaces[i].mIndices[0], sizeof(uint));
			memcpy(&newmesh->_indices[i * 3 + 1], &mesh->mFaces[i].mIndices[1], sizeof(uint));
			memcpy(&newmesh->_indices[i * 3 + 2], &mesh->mFaces[i].mIndices[2], sizeof(uint));
		}
	}
	else
		newmesh->buffersSize[indices_size] = 0;


	if (mesh->HasNormals())
	{
		newmesh->buffersSize[normals_size] = mesh->mNumVertices;
		newmesh->_normals = new float[mesh->mNumVertices * 3];
		memcpy(newmesh->_normals, mesh->mNormals, sizeof(float) * mesh->mNumVertices * 3);
	}
	else
		newmesh->buffersSize[normals_size] = 0;

	//Loading mesh texture coordinates -----------
	if (mesh->HasTextureCoords(0))
	{
		newmesh->buffersSize[tex_coords_size] = mesh->mNumVertices;
		newmesh->_tex_coords = new float[mesh->mNumVertices * 2];

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			newmesh->_tex_coords[i * 2] = mesh->mTextureCoords[0][i].x;
			newmesh->_tex_coords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}
	}
	else
		newmesh->buffersSize[tex_coords_size] = 0;




	std::string _path("/Library/Meshes/");
	_path.append(to_string(ID));

	newmesh->name = fileName;

	newmesh->ID = ID;
	newmesh->resource_path = _path;
	newmesh->original_path.append(path);

	SaveMeshResource(newmesh, ID);

	return newmesh;
}

bool MeshImporter::SaveMeshResource(const Mesh_R *mesh, UID ID)
{

	uint size = mesh->original_path.size() + mesh->name.size() + sizeof(mesh->buffersSize) + (sizeof(uint) * mesh->buffersSize[indices_size] * 3) + (sizeof(float) * mesh->buffersSize[vertices_size] * 3)
		+ (sizeof(float) * mesh->buffersSize[normals_size] * 3) + (sizeof(float) * mesh->buffersSize[tex_coords_size] * 2) + sizeof(uint) * 2 +
		(sizeof(uint) * mesh->buffersSize[weights_size]) + (sizeof(uint) * mesh->buffersSize[weights_size]) + (sizeof(float) * mesh->buffersSize[weights_size]);

	char* data = new char[size];
	char* cursor = data;

	//Save paths
	uint length = mesh->original_path.size();
	memcpy(cursor, &length,sizeof(uint));
	cursor += sizeof(uint);

	memcpy(cursor, mesh->original_path.c_str(), mesh->original_path.size());
	cursor += mesh->original_path.size();

	length = mesh->name.size();
	memcpy(cursor, &length, sizeof(uint));
	cursor += sizeof(uint);

	memcpy(cursor, mesh->name.c_str(), mesh->name.size());
	cursor += mesh->name.size();

	//Save Ranges
	uint bytes = sizeof(mesh->buffersSize);
	memcpy(cursor, mesh->buffersSize, bytes);
	cursor += bytes;

	//Save Indices
	bytes = sizeof(uint) * mesh->buffersSize[indices_size] * 3;
	memcpy(cursor, mesh->_indices, bytes);
	cursor += bytes;

	//Save Vertices
	bytes = sizeof(float) * mesh->buffersSize[vertices_size] * 3;
	memcpy(cursor, mesh->_vertices, bytes);
	cursor += bytes;

	//Save Normals
	if (mesh->buffersSize[normals_size] > 0) {
		bytes = sizeof(float) *mesh->buffersSize[normals_size] * 3;
		memcpy(cursor, mesh->_normals, bytes);
		cursor += bytes;
	}

	//Save Textures
	if (mesh->buffersSize[tex_coords_size] > 0) {
		bytes = sizeof(float) * mesh->buffersSize[tex_coords_size] * 2;
		memcpy(cursor, mesh->_tex_coords, bytes);
		cursor += bytes;
	}

	if (mesh->buffersSize[weights_size] > 0) {
		bytes = sizeof(uint) * mesh->buffersSize[weights_size];
		memcpy(cursor, mesh->_bones, bytes);
		cursor += bytes;
	}

	if (mesh->buffersSize[weights_size] > 0) {
		bytes = sizeof(uint) * mesh->buffersSize[weights_size];
		memcpy(cursor, mesh->_weights_indices, bytes);
		cursor += bytes;
	}

	if (mesh->buffersSize[weights_size] > 0) {
		bytes = sizeof(float) * mesh->buffersSize[weights_size];
		memcpy(cursor, mesh->_weights, bytes);
		cursor += bytes;
	}

	uint ret = App->filesystem->Save(mesh->resource_path.c_str(), data, size);
	RELEASE_ARRAY(data);

	return true;
}

Mesh_R * MeshImporter::LoadMeshResource(UID ID)
{

	std::string path = "/Library/Meshes/";
	path.append(std::to_string(ID));

	char* buffer;
	uint size = App->filesystem->Load(path.c_str(), &buffer);


	if (size > 0)
	{
		Mesh_R* mesh = new Mesh_R();
		char * cursor = buffer;

		uint stringSize = 0;
		uint bytes = sizeof(uint);
		memcpy(&stringSize, cursor, bytes);
		cursor += bytes;

		if (stringSize > 0)
		{
			char* string = new char[stringSize + 1];
			bytes = sizeof(char) * stringSize;
			memcpy(string, cursor, bytes);
			cursor += bytes;
			string[stringSize] = '\0';
			mesh->original_path = string;
			RELEASE_ARRAY(string);
		}

		uint nameSize = 0;
		memcpy(&nameSize, cursor, sizeof(uint));
		cursor += sizeof(uint);


		if (nameSize > 0)
		{
			char* string = new char[nameSize + 1];
			bytes = sizeof(char) * nameSize;
			memcpy(string, cursor, bytes);
			cursor += bytes;
			string[nameSize] = '\0';
			mesh->name = string;
			RELEASE_ARRAY(string);
		}

		//Load Ranges
		bytes = sizeof(mesh->buffersSize);
		memcpy(&mesh->buffersSize, cursor, bytes);
		cursor += bytes;

		//Load Indices
		bytes = sizeof(uint) * mesh->buffersSize[indices_size] * 3;
		mesh->_indices = new uint[mesh->buffersSize[indices_size] * 3];
		memcpy(mesh->_indices, cursor, bytes);
		cursor += bytes;

		//Load VErtices
		bytes = sizeof(float) * mesh->buffersSize[vertices_size] * 3;
		mesh->_vertices = new float[mesh->buffersSize[vertices_size] * 3];
		memcpy(mesh->_vertices, cursor, bytes);
		cursor += bytes;

		if (mesh->buffersSize[normals_size] > 0)
		{
			bytes = sizeof(float) * mesh->buffersSize[normals_size] * 3;
			mesh->_normals = new float[mesh->buffersSize[normals_size] * 3];
			memcpy(mesh->_normals, cursor, bytes);
			cursor += bytes;
		}

		if (mesh->buffersSize[tex_coords_size] > 0)
		{
			bytes = sizeof(float) * mesh->buffersSize[tex_coords_size] * 2;
			mesh->_tex_coords = new float[mesh->buffersSize[tex_coords_size] * 2];
			memcpy(mesh->_tex_coords, cursor, bytes);
			cursor += bytes;
		}

		if (mesh->buffersSize[bones_size] > 0)
		{
			bytes = sizeof(uint) * mesh->buffersSize[weights_size];
			mesh->_bones = new uint[mesh->buffersSize[weights_size]];
			memcpy(mesh->_bones, cursor, bytes);
			cursor += bytes;
		}

		if (mesh->buffersSize[weights_size] > 0)
		{
			bytes = sizeof(uint) * mesh->buffersSize[weights_size];
			mesh->_weights_indices = new uint[mesh->buffersSize[weights_size]];
			memcpy(mesh->_weights_indices, cursor, bytes);
			cursor += bytes;

			bytes = sizeof(float) * mesh->buffersSize[weights_size];
			mesh->_weights = new float[mesh->buffersSize[weights_size]];
			memcpy(mesh->_weights, cursor, bytes);
			cursor += bytes;
		}

		mesh->ID = ID;
		mesh->resource_path = path;



		RELEASE_ARRAY(buffer);

		return mesh;
	}
	return nullptr;
}
