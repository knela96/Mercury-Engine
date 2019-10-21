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
	glGenTextures(1, &checkImage_id);
	glBindTexture(GL_TEXTURE_2D, checkImage_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	shader = new Shader();

	Load("Models/Baker_House/BakerHouse.fbx");

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

	for (GameObject* obj : gameObjects){
		if (obj != nullptr) {
			obj->CleanUp();
			obj = nullptr;
		}
	}
	gameObjects.clear();

	delete shader;
	shader = nullptr;
	return true;
}

bool ModuleImporter::LoadFile(const char* path) {
	string extension = getFileExt(path);

	for (int i = 0; i < strlen(extension.c_str()); i++) {
		extension[i] = toupper(extension[i]); //to Upper letters
	}

	if (extension == "FBX")
		Load(path);
	else if (extension == "PNG" || extension =="DDS") {
		if (App->gui->inspector->active_gameObject != nullptr) {
			if(App->gui->inspector->active_gameObject->textures.size() != 0)
				App->gui->inspector->active_gameObject->textures.pop_back();
			FileFormats format;
			if (extension == "PNG") {
				format = PNG;
			}
			else if (extension == "DDS") {
				format = DDS;
			}
			App->gui->inspector->active_gameObject->mesh->TexCoordsDSS_PNG(format);
			App->gui->inspector->active_gameObject->textures.push_back(SaveTexture(path, aiTextureType_DIFFUSE,format));
		}
	}
	return true;
}

bool ModuleImporter::Load(const char* path) {
	bool ret = true;
	LOGC("Loading Mesh File: %s", path);
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int j = 0; j < scene->mNumMeshes && ret; ++j) {
			aiMesh* new_mesh = scene->mMeshes[j];
			string str(&path[0]);
			gameObjects.push_back(ProcessMesh(new_mesh, &getRootPath(str), scene));
		}

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);

	return ret;
}

GameObject* ModuleImporter::ProcessMesh( aiMesh* mesh, string* path, const aiScene* scene) {
	
	vector<Vertex> vertices;
	vector<uint> indices;
	vector<Texture*> textures;

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
				mesh->mColors[0][i].a
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
	vector<Texture*> diffuseMaps = loadMaterialTextures(path, material, aiTextureType_DIFFUSE);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture*> specularMaps = loadMaterialTextures(path, material, aiTextureType_SPECULAR);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture*> normalMaps = loadMaterialTextures(path, material, aiTextureType_HEIGHT);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture*> heightMaps = loadMaterialTextures(path, material, aiTextureType_AMBIENT);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	LOGC("Loaded Vertices: %u", vertices.size());
	LOGC("Loaded Indices: %u", indices.size());
	LOGC("Loaded Textures: %u", textures.size());
	
	return (GameObject*)new MeshObject(vertices, indices, textures, mesh->mName.C_Str());
}

vector<Texture*> ModuleImporter::loadMaterialTextures(string* path, aiMaterial *mat, aiTextureType type)
{
	vector<Texture*> texture;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		if(path->size() != 0)
			path->append("/");
		path->append(str.C_Str());
		Texture* tex = SaveTexture(path->c_str(), type,PNG);
		if(tex != nullptr)
			texture.push_back(tex);		
	}
	return texture;
}

Texture* ModuleImporter::SaveTexture(const char* str, aiTextureType type, FileFormats format) {
	for (unsigned int j = 0; j < stored_textures.size(); j++)
	{
		if (std::strcmp(stored_textures[j]->path.c_str(), str) == 0)
		{
			return stored_textures[j];
		}
	}
	Texture* tex = new Texture();
	bool ret = false;
	if(format == PNG)
		ret = LoadTexture(str, tex->id, tex->size);
	else if(format == DDS)
		ret = loadDDS(str, tex->id, tex->size);
	if (ret){
		tex->type = type;
		tex->path = str;
		stored_textures.push_back(tex); //store to loaded textures
		return tex;
	}
	return nullptr;
}

bool ModuleImporter::LoadTexture(const char*path, uint &id, vec2 &size) {
	
	ILuint image;

	ilGenImages(1,&image);
	ilBindImage(image);

	if (!ilLoadImage(path)) {
		ilDeleteImages(1, &image);
		LOGC("TEXTURE NOT FOUND");
			return false;
	}
	else {
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (!ilutGLBindTexImage())
			LOGC("Cannot Bind Texture Image");
		LOG("generating texture, path: %s", path);
		
		long h, v, bpp, f;
		ILubyte *texdata = 0;

		v = ilGetInteger(IL_IMAGE_WIDTH);
		h = ilGetInteger(IL_IMAGE_HEIGHT);
		bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
		f = ilGetInteger(IL_IMAGE_FORMAT);
		texdata = ilGetData();
		size.x = v;
		size.y = h;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

		glTexImage2D(GL_TEXTURE_2D,0, f, v, h,0,GL_RGBA, GL_UNSIGNED_BYTE, texdata);
		glGenerateMipmap(GL_TEXTURE_2D);

		ilDeleteImages(1, &image);
	}

	return true;
}

bool ModuleImporter::loadDDS(const char* path, uint &id, vec2 &size) {
	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(path, "rb");
	if (fp == NULL)
		return 0;

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	size.x = width;
	size.y = height;

	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	glGenTextures(1, &id);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount);
	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	free(buffer);

	return true;
}

void ModuleImporter::PushObj(aiMesh * mesh)
{
	gameObjects.push_back(ProcessMesh(mesh));
}

bool ModuleImporter::Draw() {
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();


	for (int i = 0; i < gameObjects.size(); ++i) {
		if(gameObjects[i]->active)
			gameObjects[i]->Draw();
	}

	return true;
}

string ModuleImporter::getFileExt(const string& s) {

	size_t i = s.rfind('.', s.length());
	if (i != string::npos) {
		return (char*)&(s.substr(i + 1, s.length() - i));
	}

	return(s);
}

string ModuleImporter::getRootPath(const string& s) {

	string directory;
	const size_t last_slash_idx = s.rfind('/');
	if (std::string::npos != last_slash_idx)
	{
		directory = s.substr(0, last_slash_idx);
	}

	return(directory);
}