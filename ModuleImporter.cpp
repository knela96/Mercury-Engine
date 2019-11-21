#include "Application.h"
#include "ModuleImporter.h"
#include "MeshObject.h"
#include "Primitive.h"
#include "SDL.h"
#include "C_Transform.h"

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

	shader = new Shader();

	//Load("Assets\\Models\\BakerHouse.fbx");

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

	for (Texture* tex : stored_textures) {
		if (tex != nullptr) {
			delete tex;
			tex = nullptr;
		}
	}
	stored_textures.clear();
	delete shader;
	shader = nullptr;
	return true;
}

bool ModuleImporter::LoadFile(const char* path) {
	string extension = getFileExt(path);
	std::string fileName = "";
	for (int i = 0; i < strlen(extension.c_str()); i++) {
		extension[i] = toupper(extension[i]); //to Upper letters
	}

	fileName = "";
	if (extension == "FBX") {
		App->filesystem->SplitFilePath(path, nullptr, &fileName);
		std::string destiny("/Assets/Models/");
		destiny.append(fileName);
		if(!App->filesystem->Exists(destiny.c_str()))
			App->filesystem->CopyFromOutsideFS(path, destiny.c_str());
		Load(destiny.c_str());
	}
	else if (extension == "PNG" || extension =="DDS") {
		App->filesystem->SplitFilePath(path, nullptr, &fileName);
		std::string destiny("/Assets/Textures/");
		destiny.append(fileName);
		if (!App->filesystem->Exists(destiny.c_str()))
			App->filesystem->CopyFromOutsideFS(path, destiny.c_str());


		char* buffer = nullptr;
		uint size = App->filesystem->Load(destiny.c_str(), &buffer);
		std::string texture_fileName = "", texture_extension = ""; std::string texture_path;
		App->filesystem->SplitFilePath(destiny.c_str(), nullptr, &texture_fileName, &texture_extension);

		texture_path = "/Assets/Textures/";
		texture_path.append(texture_fileName);

		App->material_importer->ImportTextureResource(buffer, texture_path.c_str(), size);
	}
	return true;
}

bool ModuleImporter::Load(const char* path) {
	bool ret = true;
	string FileName = getFileName(path);
	LOGC("Loading Mesh File: %s", path);

	App->filesystem->GetReadPaths();

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		string str(&path[0]);
		//App->scene_intro->root->childs.push_back(LoadHierarchy(scene->mRootNode,(aiScene*)scene, &FileName, &str,App->scene_intro->root));
		ImportMesh(scene->mRootNode, (aiScene*)scene, &FileName, &str);
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);

	return ret;
}

void ModuleImporter::ImportMesh(aiNode* node, aiScene* scene, string* FileName, string* str) {
	uint* index = node->mMeshes;



	if (index != nullptr) {
		App->mesh_importer->ImportMeshResource(scene->mMeshes[*index], &getRootPath(*str), FileName->c_str(), App->RandomNumberGenerator.GetIntRNInRange(), scene);

		aiMaterial* material = scene->mMaterials[scene->mMeshes[*index]->mMaterialIndex];
		App->material_importer->ImportMaterialResource(&getRootPath(*str), material);

	}

	for (int i = 0; i < node->mNumChildren; ++i) {
		aiNode* child = node->mChildren[i];
		ImportMesh(child, scene, FileName, str);
	}

}



GameObject* ModuleImporter::LoadHierarchy(aiNode* node, aiScene* scene, string* FileName,string* str, GameObject* parent) {
	uint* index = node->mMeshes;
	GameObject* gameObject = nullptr;
	if (index == nullptr && parent == App->scene_intro->root) {
		gameObject = new GameObject(FileName->c_str());
	}
	else {
		if(index != nullptr)
			gameObject = ProcessMesh(scene->mMeshes[*index], &getRootPath(*str), FileName->c_str(), scene);
	}

	if (gameObject != nullptr) {
		gameObject->parent = parent;

		aiVector3D translation, scaling;
		aiQuaternion rotation;
		node->mTransformation.Decompose(scaling, rotation, translation);
		
		float3 pos(translation.x, translation.y, translation.z);
		float3 scale(scaling.x, scaling.y, scaling.z);
		Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

		gameObject->transform->vposition = pos;
		gameObject->transform->vrotation = rot;
		gameObject->transform->vscale = scale;

		gameObject->transform->UpdateMatrices();		

		for (int i = 0; i < node->mNumChildren; ++i) {
			aiNode* child = node->mChildren[i];
			GameObject* go = nullptr;
			go = LoadHierarchy(child, scene, FileName, str, gameObject);
			if(go != nullptr)
				gameObject->childs.push_back(go);
		}
	}

	return gameObject;
}

GameObject* ModuleImporter::ProcessMesh( aiMesh* mesh, string* path, const char* fileName, const aiScene* scene) {

	//GameObject* gameobject = new MeshObject(/*vertices, indices, textures, mesh->mName.C_Str()*/);

	//gameobject->box.SetNegativeInfinity();
	//gameobject->box.Enclose(points, mesh->mNumVertices);

	//std::free(points);
	////saveGOinFile(gameobject);
	//return gameobject;
	return nullptr;
}

const string ModuleImporter::getFileExt(const string& s) {

	size_t i = s.rfind('.', s.length());
	if (i != string::npos) {
		return (char*)&(s.substr(i + 1, s.length() - i));
	}

	return(s);
}

string ModuleImporter::getRootPath(const string& s) {

	string directory;
	const size_t last_slash_idx = s.rfind('\\');
	if (std::string::npos != last_slash_idx)
	{
		directory = s.substr(0, last_slash_idx);
	}

	return(directory);
}

const string ModuleImporter::getFileName(const string& s) {

	string directory;
	size_t i = s.rfind('\\', s.length());
	if (i != string::npos) {
		directory = (s.substr(i + 1, s.length() - i));
	}
	string file;
	const size_t last_slash_idx = directory.rfind('.');
	if (std::string::npos != last_slash_idx)
	{
		file = directory.substr(0, last_slash_idx);
	}


	return(file);
}
