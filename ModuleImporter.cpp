#include "Application.h"
#include "ModuleImporter.h"
#include "MeshObject.h"
#include "Primitive.h"
#include "SDL.h"


#include "DevIL/include/IL/ilut.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "lib/Assimp/libx86/assimp.lib")

#pragma comment (lib, "lib/DevIL/lib/x86/Release/DevIL.lib")
#pragma comment (lib, "lib/DevIL/lib/x86/Release/ILU.lib")
#pragma comment (lib, "lib/DevIL/lib/x86/Release/ILUT.lib")


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

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		LOGC("ERROR CREATING VERTEX SHADER\n %s", infoLog);
	}

	// link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		LOGC("ERROR LINKING PROGRAM SHADERS\n %s",infoLog);
	}
	glDeleteShader(vertexShader);
	

	Load("BakerHouse.fbx");

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

			meshes.push_back(ProcessMesh(new_mesh,scene));
		}

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);

	return ret;
}

MeshObject ModuleImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
	
	vector<Vertex> vertices;
	vector<uint> indices;
	vector<Texture> textures;

	for (uint i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		if (mesh->HasFaces())
		{
			vertex.Position = { 
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			};
		}
		if (mesh->HasNormals())
		{
			vertex.Normal = { 
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			};
		}
		if (mesh->HasVertexColors(0)) {
			vertex.Colors = { 
				mesh->mColors[0][i].r,
				mesh->mColors[0][i].g,
				mesh->mColors[0][i].b,
				mesh->mColors[0][i].a,
			};
		}
		else
			vertex.Colors = { 1.0f,1.0f,1.0f,1.0f };
		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords = { 
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			};
		}
		else
			vertex.TexCoords = { 0.0f,0.0f };

		vertices.push_back(vertex);
	}

	for (uint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace* face = &mesh->mFaces[i];

		for (uint j = 0; j < face->mNumIndices; j++)
			indices.push_back(face->mIndices[j]);
	}

	//LOAD MATERIAL TEXTURES
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// 1. diffuse maps
	vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	//Texture texture;
	//LoadTexture("image.png", texture.id);
	//texture.type = aiTextureType_DIFFUSE;
	//texture.path = "image.png";
	//stored_textures.push_back(texture); //store to loaded textures
	//textures.push_back(texture);

	return MeshObject(vertices, indices, textures);
}

vector<Texture> ModuleImporter::loadMaterialTextures(aiMaterial *mat, aiTextureType type)
{
	vector<Texture> texture;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < stored_textures.size(); j++)
		{
			if (std::strcmp(stored_textures[j].path.c_str(), str.C_Str()) == 0)
			{
				texture.push_back(stored_textures[j]);
				skip = true;
				break;
			}
		}
		if (!skip){
			Texture texture;
			LoadTexture(str.C_Str(), texture.id);
			texture.type = type;
			texture.path = str.C_Str();
			stored_textures.push_back(texture); //store to loaded textures
		}
	}
	return texture;
}

uint ModuleImporter::LoadTexture(const char*path, uint &id) {
	
	ILuint image;


	ilGenImages(1,&image);
	ilBindImage(image);

	if (!ilLoadImage(path)) {
		ilDeleteImages(1, &image);
		LOG("texture not loaded, error ocurred")
			return false;
	}
	else {
		id = ilutGLBindTexImage();
		LOG("generating texture, path: %s", path);
		
		long h, v, bpp, f;
		ILubyte *texdata = 0;

		h = ilGetInteger(IL_IMAGE_WIDTH);
		v = ilGetInteger(IL_IMAGE_HEIGHT);
		bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
		f = ilGetInteger(IL_IMAGE_FORMAT);
		texdata = ilGetData();

		/*ilBindImage(id);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);*/

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, f, v, h, GL_RGB, GL_UNSIGNED_BYTE, texdata);
		glBindTexture(GL_TEXTURE_2D, 0);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, h, v,0, f, GL_UNSIGNED_BYTE, texdata);
		//glGenerateMipmap(GL_TEXTURE_2D);		
		//gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, h, v, f, GL_UNSIGNED_BYTE, texdata);
		//glBindTexture(GL_TEXTURE_2D, 0);

		ilBindImage(0);
		ilDeleteImage(image);
	}

	return true;
}

bool ModuleImporter::CreateTexture() {

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
	glBindTexture(GL_TEXTURE_2D, 0);*/

	return true;
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

	/*
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
	glDisable(GL_TEXTURE_2D);*/




	return true;
}