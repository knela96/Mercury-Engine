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
	//string extension = getFileExt(path);
	//std::string fileName = "";
	//for (int i = 0; i < strlen(extension.c_str()); i++) {
	//	extension[i] = toupper(extension[i]); //to Upper letters
	//}

	//fileName = "";
	//if (extension == "FBX") {
	//	App->filesystem->SplitFilePath(path, nullptr, &fileName);
	//	std::string destiny("/Assets/Models/");
	//	destiny.append(fileName);
	//	if(!App->filesystem->Exists(destiny.c_str()))
	//		App->filesystem->CopyFromOutsideFS(path, destiny.c_str());
	//	Load(destiny.c_str());
	//}
	//else if (extension == "PNG" || extension =="DDS") {
	//	App->filesystem->SplitFilePath(path, nullptr, &fileName);
	//	std::string destiny("/Assets/Textures/");
	//	destiny.append(fileName);
	//	if (!App->filesystem->Exists(destiny.c_str()))
	//		App->filesystem->CopyFromOutsideFS(path, destiny.c_str());


	//	char* buffer = nullptr;
	//	uint size = App->filesystem->Load(destiny.c_str(), &buffer);
	//	std::string texture_fileName = "", texture_extension = ""; std::string texture_path;
	//	App->filesystem->SplitFilePath(destiny.c_str(), nullptr, &texture_fileName, &texture_extension);

	//	texture_path = "/Assets/Textures/";
	//	texture_path.append(texture_fileName);

	//	App->material_importer->ImportTextureResource(buffer, texture_path.c_str(), size);
	//}
	return true;
}

Mesh_R* ModuleImporter::Load(const char* path,std::string original_file) {
	bool ret = true;
	string FileName = getFileName(path);
	LOGC("Loading Mesh File: %s", path);
	
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		ImportMesh(scene->mRootNode, (aiScene*)scene, &FileName, &original_file);



		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);

	return nullptr;
}

void ModuleImporter::ImportMesh(aiNode* node, aiScene* scene, string* FileName, string* str) {
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		Mesh_R* resourceMesh = nullptr;
		std::string name(scene->mMeshes[i]->mName.C_Str());

		if (!std::strcmp(name.c_str(), "")) {
			name.append(FileName->c_str());
			name.append("_");
			name.append(to_string(i));
		}

		Meta* meta = App->resources->FindMetaResource(str->c_str(), name.c_str(), ResourceType::MeshR);
		UID id = (meta == nullptr) ? App->resources->GenerateNewUID() : meta->id;

		resourceMesh = App->mesh_importer->ImportMeshResource(scene->mMeshes[i], str, name.c_str(), id); //Import the mesh
		App->resources->AddResource(resourceMesh);

		

		Material_R* resourceMaterial = nullptr;
		aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
		aiString matName;
		material->Get(AI_MATKEY_NAME, matName);

		meta = App->resources->FindMetaResource(str->c_str(), matName.C_Str(), ResourceType::MaterialR);

		std::string matname = matName.C_Str();
		resourceMaterial = App->material_importer->ImportMaterialResource(str, material, &matname);
		App->resources->AddResource((Resources*)resourceMaterial);

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
