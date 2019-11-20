#include "Application.h"
#include "MeshImporter.h"
#include "ModuleImporter.h"
#include "MeshObject.h"



MeshImporter::MeshImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


MeshImporter::~MeshImporter()
{
}

MeshObject * MeshImporter::ImportMeshResource(const aiMesh * mesh, unsigned long long ID, const char * file, const char * name)
{
	//vector<Vertex> vertices;
	//vector<uint> indices;
	//vector<Texture*> textures;

	//math::float3* points = (float3*)malloc(sizeof(float3) * mesh->mNumVertices);

	////LOAD MATERIAL TEXTURES
	//aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	//aiColor3D color(1.f, 1.f, 1.f);
	//material->Get(AI_MATKEY_COLOR_DIFFUSE, color);

	//for (uint i = 0; i < mesh->mNumVertices; ++i)
	//{
	//	Vertex vertex;
	//	if (mesh->HasFaces())
	//	{
	//		vertex.Position = {
	//			mesh->mVertices[i].x,
	//			mesh->mVertices[i].y,
	//			mesh->mVertices[i].z
	//		};
	//		points[i].Set(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
	//	}

	//	if (mesh->HasNormals())
	//	{
	//		vertex.Normal = {
	//			mesh->mNormals[i].x,
	//			mesh->mNormals[i].y,
	//			mesh->mNormals[i].z
	//		};
	//	}
	//	if (mesh->HasVertexColors(0)) {
	//		vertex.Colors = {
	//			mesh->mColors[0][i].r,
	//			mesh->mColors[0][i].g,
	//			mesh->mColors[0][i].b,
	//			mesh->mColors[0][i].a
	//		};
	//	}
	//	else {
	//		vertex.Colors = {
	//			color.r,
	//			color.g,
	//			color.b,
	//			1.0f
	//		};
	//	}

	//	if (mesh->mTextureCoords[0])
	//	{
	//		vertex.TexCoords = {
	//			mesh->mTextureCoords[0][i].x,
	//			mesh->mTextureCoords[0][i].y
	//		};
	//	}
	//	else
	//		vertex.TexCoords = { 0.0f,0.0f };

	//	vertices.push_back(vertex);
	//}

	//for (uint i = 0; i < mesh->mNumFaces; i++)
	//{
	//	aiFace* face = &mesh->mFaces[i];

	//	for (uint j = 0; j < face->mNumIndices; j++)
	//		indices.push_back(face->mIndices[j]);
	//}

	//if (mesh->mName.length == 0)
	//	mesh->mName = fileName;

	//MeshObject* gameobject = new MeshObject(vertices, indices, textures, mesh->mName.C_Str());

	//saveGOinFile(gameobject);
	return nullptr;
}

bool MeshImporter::SaveMeshResource(const MeshObject *)
{
	return false;
}

MeshObject * MeshImporter::LoadMeshResource(u64 ID)
{
	return nullptr;
}
