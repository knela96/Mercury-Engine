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

UID ModuleResources::Find(const char * file_in_assets) const
{
	return u64();
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
	Mesh_R* resource = nullptr;

	//Load Mesh
	App->importer->Load(file, origin_path);
	if (resource)
	{
		AddResource(resource);
	}
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
	config["Game Objects"]["ID"]["ID"] = resource->ID;
	config["Game Objects"]["ID"]["Name"] = resource->name.c_str();
	config["Game Objects"]["ID"]["Type"] = resource->getType();
	config["Game Objects"]["ID"]["Date"] = modDate;

	std::string path, filename;
	App->filesystem->SplitFilePath(resource->original_path.c_str(), &path, &filename,nullptr,true);

	// Create the stream and open the file
	std::string fullpath = path + filename + ".meta";
	std::ofstream stream{ fullpath,std::ofstream::out };
	//stream.open(fullpath);
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

Resources * ModuleResources::Get(UID uid)
{
	return nullptr;
}

Resources * ModuleResources::CreateNewResource(ResourceType type, UID force_uid)
{
	Resources* ret = nullptr;
	UID uid = GenerateNewUID();
	switch (type) {
		case ResourceType::TextureR: ret = (Resources*) new Texture_R(); break;
		case ResourceType::MeshR: ret = (Resources*) new Mesh_R(); break;
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
