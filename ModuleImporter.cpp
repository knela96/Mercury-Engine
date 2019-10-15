#include "Application.h"
#include "ModuleImporter.h"
#include "MeshObject.h"
#include "Primitive.h"
#include "SDL.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "DevIL/DevIL/include/IL/ilut.h"
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

bool ModuleImporter::LoadTexture(uint Imageid,const char*path) {


	
	uint texture;

	ILuint image_id;

	ilGenImages(1, &image_id);
	ilBindImage(image_id);

	if (!ilLoadImage(path)) {
		ilDeleteImages(1, &image_id);
		LOG("texture not loaded, error ocurred")
			return false;
	}
	else {
		texture = ilutGLBindTexImage();
		LOG("generating texture, path: %s", path)

		long h, v, bpp, f;
		ILubyte *texdata = 0;

		h = ilGetInteger(IL_IMAGE_WIDTH);
		v = ilGetInteger(IL_IMAGE_HEIGHT);
		bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
		f = ilGetInteger(IL_IMAGE_FORMAT);
		texdata = ilGetData();

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, h, v, f, GL_UNSIGNED_BYTE, texdata);
		glBindTexture(GL_TEXTURE_2D, 0);

		ilBindImage(0);
		ilDeleteImage(image_id);

		return texture;
	}
	
	/*GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &Texture_id);
	glBindTexture(GL_TEXTURE_2D, Texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glBindTexture(GL_TEXTURE_2D, 0);
*/
}

ILboolean ModuleImporter::ilLoadImage(const char * FileName)
{


	return ILboolean();
}

void ModuleImporter::PushObj(aiMesh * mesh)
{
	meshes.push_back(ProcessMesh(mesh));
}

bool ModuleImporter::Draw() {
	
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	for (int i = 0; i < meshes.size(); ++i) {
		meshes[i].Draw();
	}


	//_____________________

	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, Texture_id);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
	glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);




	return true;
}