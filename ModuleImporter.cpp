#include "Application.h"
#include "ModuleImporter.h"
#include "MeshObject.h"
#include "Primitive.h"
#include "SDL.h"

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
	//Load("warrior.fbx");

	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
	return true;
}

update_status ModuleImporter::PreUpdate(float dt){

	return UPDATE_CONTINUE;
}

update_status ModuleImporter::PostUpdate(float dt){

	return UPDATE_CONTINUE;
}

update_status ModuleImporter::Update(float dt) {
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

			meshes.push_back(ProcessMesh(new_mesh));
		}

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);

	return ret;
}

MeshObject ModuleImporter::ProcessMesh(aiMesh* new_mesh) {
	
	vector<Vertex> vertices;
	vector<uint> indices;
	vector<Texture> textures;

	for (uint i = 0; i < new_mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		if (new_mesh->HasFaces())
		{
			vertex.Position = { 
				new_mesh->mVertices[i].x, 
				new_mesh->mVertices[i].y, 
				new_mesh->mVertices[i].z 
			};
		}
		if (new_mesh->HasNormals())
		{
			vertex.Normal = { 
				new_mesh->mNormals[i].x,
				new_mesh->mNormals[i].y, 
				new_mesh->mNormals[i].z 
			};
		}
		if (new_mesh->HasVertexColors(0)) {
			vertex.Colors = { 
				new_mesh->mColors[0][i].r,
				new_mesh->mColors[0][i].g,
				new_mesh->mColors[0][i].b,
				new_mesh->mColors[0][i].a,
			};
		}
		else
			vertex.Colors = { 1.0f,1.0f,1.0f,1.0f };
		if (new_mesh->mTextureCoords[0])
		{
			vertex.TexCoords = { 
				new_mesh->mTextureCoords[0][i].x, 
				new_mesh->mTextureCoords[0][i].y 
			};
		}
		else
			vertex.TexCoords = { 0.0f,0.0f };

		vertices.push_back(vertex);
	}

	for (uint i = 0; i < new_mesh->mNumFaces; i++)
	{
		aiFace* face = &new_mesh->mFaces[i];

		for (uint j = 0; j < face->mNumIndices; j++)
			indices.push_back(face->mIndices[j]);
	}

	//LOAD TEXTURES

	return MeshObject(vertices, indices, textures);
}

void ModuleImporter::LoadTexture(uint Imageid) {

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &Imageid);
	glBindTexture(GL_TEXTURE_2D, Imageid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
}

bool ModuleImporter::Draw() {
	
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	for (int i = 0; i < meshes.size(); ++i) {
		meshes[i].Draw();
	}
	return true;
}