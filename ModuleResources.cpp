#include "Application.h"
#include "ModuleResources.h"
#include "Texture_R.h"
#include "Material_R.h"
#include "Mesh_R.h"
#include "RNGenerator.h"


#include <fstream>
#include <iomanip>
ModuleResources::ModuleResources(Application * app, bool start_enabled) : Module(app,start_enabled)
{
}

ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Start()
{
	ImportMetaFiles();
	ReimportFiles();
	return true;
}

void ModuleResources::ReimportFiles() {
	std::vector<std::string> filter_ext;
	filter_ext.push_back("meta");
	filter_ext.push_back("scene");
	std::vector<std::string> list;

	App->filesystem->RecursiveGetFoldersFiles(ASSETS_FOLDER, nullptr, &filter_ext, &list);

	UpdateAssets(&list);
}

void ModuleResources::UpdateAssets(std::vector<std::string>* list) {
	for (int i = 0; i < list->size(); ++i) {
		std::string path, fileName;
		App->filesystem->SplitFilePath(list->at(i).c_str(), &path, &fileName,nullptr,true);
		LOG("Checking: [%s]", list->at(i).c_str());
		string meta_path(path + fileName + ".meta");
		if (!App->filesystem->Exists(meta_path.c_str()) || ModifiedFile(meta_path.c_str(), list->at(i).c_str()))
		{
			App->importer->LoadFile(list->at(i).c_str());
		}
	}	
}

bool ModuleResources::ModifiedFile(const char* meta_path,const char* file_path) {
	char* buffer = nullptr;
	uint size = App->filesystem->Load(meta_path, &buffer);

	if (size > 0)
	{
		json file;
		ifstream stream;
		stream.open(meta_path);
		file = json::parse(stream);
		uint64 configData = file["Date"].get<uint64>();
		stream.close();

		uint64 fileMod = App->filesystem->GetLastModTime(file_path);
		return fileMod != configData;
	}
}

void ModuleResources::ImportMetaFiles() {
	
	std::vector<std::string> filter_ext;
	filter_ext.push_back("meta");
	std::vector<std::string> list;

	App->filesystem->RecursiveGetFoldersFiles(ASSETS_FOLDER, &filter_ext, nullptr, &list);

	LoadMetaFromFile(&list);
}

void ModuleResources::LoadMetaFromFile(std::vector<std::string>* files) {
	for (uint i = 0; i < files->size(); i++)
	{
		LoadMeta(files->at(i).c_str());
	}
}

void ModuleResources::LoadMeta(const char* path) {
	json file;
	ifstream stream;
	stream.open(path);
	file = json::parse(stream);
	
	Meta meta;
	meta.id = file["ID"].get<UID>();
	meta.resource_name = file["Name"].get<string>();
	meta.type = static_cast<ResourceType>((int)(file["Type"].get<int>()));

	std::string file_path = "";
	App->filesystem->SplitFilePath(path, &file_path);
	meta.original_file = file_path + meta.resource_name;
	stream.close();

	created_resources[meta.id] = meta;

	if (meta.type == ResourceType::ObjectR)
	{
		std::string resource = "Library/GameObjects/";
		resource.append(std::to_string(meta.id));
		LoadMetaResources(resource.c_str(), meta.original_file.c_str()); //LOAD SCENE
	}
}

void ModuleResources::LoadMetaResources(const char* resource_path, const char* original_path) {
	json file;
	ifstream stream;
	stream.open(resource_path);
	file = json::parse(stream);

	int elements = file["Game Objects"]["Count"].get<int>();
	LoadElementResources(file, elements,resource_path, original_path);
}

uint ModuleResources::LoadElementResources(json& file, uint elements, const char* resource_path, const char* original_path) {
	static uint count = 0;
	char name[25];
	sprintf_s(name, 25, "Game Object %i", ++count);

	//LOAD RESOURCES HERE(ERIC)
	if (file["Game Objects"][name]["Components"].find("Mesh") != file["Game Objects"][name]["Components"].end())
	{
		Meta meta;
		meta.id = file["Game Objects"][name]["Components"]["Mesh"]["ID"].get<UID>();
		meta.type = ResourceType::MeshR;
		meta.resource_name = file["Game Objects"][name]["Components"]["Mesh"]["Name"].get<std::string>();
		meta.original_file = original_path;
		created_resources[meta.id] = meta;
	}

	if (count < elements) {
		LoadElementResources(file, elements, resource_path, original_path);
	}
	return count;
}

UID ModuleResources::ImportFile(const char * new_file_in_assets, bool force)
{
	std::string destiny = "";
	if (force) {
		std::string fileName = "";
		App->filesystem->SplitFilePath(new_file_in_assets, nullptr, &fileName);
		destiny = App->gui->filesystem->currentFolder->path;
		destiny.append("/");
		destiny.append(fileName);
		if (App->filesystem->CopyFromOutsideFS(new_file_in_assets, destiny.c_str()))
		{
			LOGC("File Succesfully Copied [%s]", destiny.c_str());
		}
		else {
			LOGC("File Couldn't be Copied [%s]", destiny.c_str());
		}
	}

	UID ret = 0; bool import_ok = false; string written_file;
	ResourceType type = GetType(new_file_in_assets);
	switch (type) {
		case ResourceType::TextureR:
		{
			char* buffer = nullptr;
			uint size = App->filesystem->Load(new_file_in_assets, &buffer);
			if (size > 0)
				App->material_importer->ImportTextureResource(buffer, new_file_in_assets, size);
			break;
		}
		case ResourceType::MeshR:
		{
			ImporSceneResource(new_file_in_assets, destiny);
			break;
		}
	}
	return ret;
}

void ModuleResources::ImporSceneResource(const char* file,std::string origin_path) {
	App->importer->Load(file);
}


ResourceType ModuleResources::GetType(const char* path)
{
	std::string ext = "";
	App->filesystem->SplitFilePath(path, nullptr, nullptr, &ext);
	App->filesystem->NormalizePath(ext);

	if (ext == "fbx")
		return ResourceType::MeshR;
	if (ext == "tga" || ext == "png" || ext == "jpg" || ext == "dds")
		return ResourceType::TextureR;
	return ResourceType::noneR;
}

UID ModuleResources::GenerateNewUID()
{
	return random.Int();
}

void ModuleResources::AddResource(Resources* resource)
{
	LoadResource(resource);
	created_resources[resource->ID] = GetMetaInfo(resource);
	SaveMetaInfo(resource);
}

Meta ModuleResources::GetMetaInfo(Resources* resource)
{
	Meta ret;

	ret.original_file = resource->original_path;
	ret.resource_name = resource->name;
	ret.type = resource->type;
	ret.id = resource->ID;

	return ret;
}

//Save .meta file in assets
void ModuleResources::SaveMetaInfo(Resources* resource)
{
	json config;
	//Getting file modification date
	uint64 modDate = App->filesystem->GetLastModTime(resource->original_path.c_str());

	//config["GameObjects"]["ID"][resource->ID];
	config["ID"] = resource->ID;
	config["Name"] = resource->name.c_str();
	config["Type"] = resource->getType();
	config["Date"] = modDate;

	std::string path, filename;
	App->filesystem->SplitFilePath(resource->original_path.c_str(), &path, &filename,nullptr,true);

	// Create the stream and open the file
	std::string fullpath = path + filename + ".meta";
	std::ofstream stream{ fullpath,std::ofstream::out };

	stream << std::setw(4) << config << std::endl;
	stream.close();
}

void ModuleResources::LoadResource(Resources* resource)
{
	if (resource == nullptr)
	{
		LOG("Error: Trying to load a null resource");
		return;
	}

	resources[resource->ID] = resource;
	switch (resource->type)
	{
		case(ResourceType::MeshR):
		{
			meshes[resource->ID] = resource;
			break;
		}
		case(ResourceType::MaterialR):
		{
			materials[resource->ID] = resource;
			break;
		}
		case (ResourceType::TextureR):
		{
			textures[resource->ID] = resource;
			break;
		}
	}
	//resource->LoadOnMemory();
}

const Resources * ModuleResources::Get(UID uid) const
{
	return nullptr;
}

Resources * ModuleResources::Get(UID ID)
{
	Resources* ret = nullptr;
	std::map<uint64, Resources*>::iterator it = resources.find(ID);
	if (it != resources.end())
	{
		ret = it->second;
	}
	else
	{
		std::map<uint64, Meta>::iterator it = created_resources.find(ID);
		if (it != created_resources.end())
		{
			switch (it->second.type)
			{
				case (ResourceType::ObjectR):
				{
					ret = App->importer->LoadObjectResource(ID);
					break;
				}
				case (ResourceType::MeshR):
				{
					ret = App->mesh_importer->LoadMeshResource(ID);
					break;
				}
				case (ResourceType::MaterialR):
				{
					//ret = App->moduleMaterials->LoadMaterialResource(ID);
					break;
				}
			}
			if (ret)
				LoadResource(ret);
		}
	}
	return ret;
}

Resources * ModuleResources::CreateNewResource(ResourceType type, UID force_uid)
{
	Resources* ret = nullptr;
	UID uid = GenerateNewUID();
	switch (type) {
		case ResourceType::TextureR: ret = (Resources*) new Texture_R(); break;
		case ResourceType::MeshR: ret = (Resources*) new Mesh_R(); break;
		case ResourceType::ObjectR: ret = (Resources*) new Mesh_R(); break;
	}
	if (ret != nullptr) {
		if (force_uid == 0)
			ret->ID = uid;
		else
			ret->ID = force_uid;
		resources[uid] = ret;
	}
	return ret;

}

Meta* ModuleResources::FindMetaResource(const char* original_file, const char* name, ResourceType type)
{
	for (std::map<uint64, Meta>::iterator it = created_resources.begin(); it != created_resources.end(); it++)
	{
		if (it->second.Compare(original_file, name, type))
		{
			return &(it->second);
		}
	}
	return nullptr;
}

uint ModuleResources::DeleteResource(UID ID)
{
	ResourceType type = created_resources[ID].type;
	uint instances = 0;

	if (resources[ID] != nullptr)
	{
		//Could break here
		instances = resources[ID]->instances;
		UnLoadResource(ID);
		RELEASE(resources[ID]);
		resources.erase(ID);
	}

	std::string resourcePath = "";
	switch (type)
	{
	case(ResourceType::MeshR):
	{
		resourcePath.append("/Library/Meshes/");
		break;
	}
	case(ResourceType::MaterialR):
	{
		resourcePath.append("/Library/Materials/");
		break;
	}
	case(ResourceType::TextureR):
	{
		resourcePath.append("/Library/Textures/");
		break;
	}
	case(ResourceType::ObjectR):
	{
		resourcePath.append("/Library/GameObjects/");
		break;
	}
	}
	resourcePath.append(std::to_string(ID));
	App->filesystem->Remove(resourcePath.c_str());

	created_resources.erase(ID);
	return instances;
}

void ModuleResources::UnLoadResource(UID ID)
{
	std::map<uint64, Resources*>::iterator it = resources.find(ID);
	if (it != resources.end())
	{
		switch (it->second->type)
		{
		case(ResourceType::MeshR):
		{
			meshes.erase(ID);
			break;
		}
		case(ResourceType::MaterialR):
		{
			materials.erase(ID);
			break;
		}
		case(ResourceType::TextureR):
		{
			textures.erase(ID);
			break;
		}
		case(ResourceType::ObjectR):
		{
			scenes.erase(ID);
			break;
		}
		}
		Resources* resource = it->second;
		resources.erase(it);
		RELEASE(resource);
	}
}