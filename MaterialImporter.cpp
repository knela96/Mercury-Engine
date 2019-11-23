#include "Application.h"
#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "GameObject.h"
#include "DevIL/include/IL/ilut.h"
#include "Material_R.h"
#include "Texture_R.h"

#pragma comment (lib, "lib/DevIL/lib/x86/Release/DevIL.lib")
#pragma comment (lib, "lib/DevIL/lib/x86/Release/ILU.lib")
#pragma comment (lib, "lib/DevIL/lib/x86/Release/ILUT.lib")


MaterialImporter::MaterialImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


MaterialImporter::~MaterialImporter()
{
}

Material_R* MaterialImporter::ImportMaterialResource(string* path, aiMaterial* mat, std::string* mat_name,UID id)
{

	Material_R* material = new Material_R();

	// 1. diffuse maps
	getMaterialTextures(path, *material,mat, aiTextureType_DIFFUSE);
	// 2. specular maps
	getMaterialTextures(path, *material, mat, aiTextureType_SPECULAR);
	// 3. normal maps
	getMaterialTextures(path, *material, mat, aiTextureType_HEIGHT);
	// 4. height maps
	getMaterialTextures(path, *material, mat, aiTextureType_AMBIENT);


	aiColor4D color;
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);

	material->color = Color(color.r, color.g, color.b, color.a);
	material->ID = id;

	material->name = mat_name->c_str();
	material->original_path = *path;

	SaveMaterialResource(material);

	return material;
}

void MaterialImporter::getMaterialTextures(string* path, Material_R &material, aiMaterial* mat, aiTextureType type)
{

	std::string texture_fileName = "", texture_extension = "";
	std::string texture_file;
	std::string texture_path;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		App->filesystem->SplitFilePath(str.C_Str(), nullptr, &texture_fileName, &texture_extension);

		texture_path = "/Assets/Textures/";
		texture_path.append(texture_fileName);

		char* buffer = nullptr;
		uint size = App->filesystem->Load(texture_path.c_str(), &buffer);

		if (size > 0)
		{
			uint64 tex_id = ImportTextureResource(buffer, texture_path.c_str(), size);
			if (tex_id != 0) {
				switch (type) {
				case aiTextureType_DIFFUSE:
					material.tex_ID_D = tex_id;
					break;
				case aiTextureType_SPECULAR:
					material.tex_ID_S = tex_id;
					break; 
				case aiTextureType_HEIGHT:
					material.tex_ID_H = tex_id;
					break;
				case aiTextureType_AMBIENT:
					material.tex_ID_AO = tex_id;
					break;
				}

			}
		}
	}
}

uint64 MaterialImporter::ImportTextureResource(char* buffer,const char* path, uint size) {
	uint64 ID = App->RandomNumberGenerator.GetIntRNInRange();
	std::string full_path("/Library/Textures/");
	full_path.append(std::to_string(ID));

	if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size))
	{
		ilEnable(IL_FILE_OVERWRITE);

		ILuint size;
		ILubyte* data;

		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
		size = ilSaveL(IL_DDS, nullptr, 0);

		if (size > 0)
		{
			data = new ILubyte[size];
			if (ilSaveL(IL_DDS, data, size) > 0)
			{
				App->filesystem->Save(full_path.c_str(), data, size);
				RELEASE_ARRAY(data);
			}
		}
	}
	return ID;
}

Texture_R* MaterialImporter::LoadTextureResource(uint64 ID)
{
	std::string full_path = "/Library/Textures/";
	full_path.append(std::to_string(ID));

	char* buffer = nullptr;
	uint size = App->filesystem->Load(full_path.c_str(), &buffer);

	Texture_R* tex = nullptr;
	if (size > 0)
	{
		tex = new Texture_R();
		tex->ID = ID;
		ILuint ImageName;
		ilGenImages(1, &ImageName);
		ilBindImage(ImageName);

		ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size);
		tex->tex_id = ilutGLBindTexImage();
		tex->resource_path = full_path;

		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		long h, v, bpp, f;
		ILubyte *texdata = 0;

		v = ilGetInteger(IL_IMAGE_WIDTH);
		h = ilGetInteger(IL_IMAGE_HEIGHT);
		bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
		f = ilGetInteger(IL_IMAGE_FORMAT);
		texdata = ilGetData();
		tex->size.x = v;
		tex->size.y = h;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &tex->tex_id);
		glBindTexture(GL_TEXTURE_2D, tex->tex_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, f, v, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata);
		glGenerateMipmap(GL_TEXTURE_2D);

		ilDeleteImages(1, &ImageName);
		RELEASE_ARRAY(buffer);
	}

	return tex;
}

std::string* MaterialImporter::SaveMaterialResource(Material_R* material)
{
	uint size = sizeof(uint64) * 4 + sizeof(float) * 4 + sizeof(uint) + material->name.size();
	
	char* data = new char[size];
	char* cursor = data;

	//Store Material Name
	uint bytes = sizeof(uint);
	uint name_size = material->name.size();
	memcpy(cursor, &name_size, bytes);
	cursor += bytes;
	bytes = material->name.size();
	if (bytes > 0) {
		memcpy(cursor, material->name.c_str(), bytes);
		cursor += bytes;
	}

	//Save Diffuse
	bytes = sizeof(uint64);
	memcpy(cursor, &material->tex_ID_D, bytes);
	cursor += bytes;
	//Save Specular
	bytes = sizeof(uint64);
	memcpy(cursor, &material->tex_ID_S, bytes);
	cursor += bytes;
	//Save Height
	bytes = sizeof(uint64);
	memcpy(cursor, &material->tex_ID_H, bytes);
	cursor += bytes;
	//Save AO
	bytes = sizeof(uint64);
	memcpy(cursor, &material->tex_ID_AO, bytes);
	cursor += bytes;
	//Save Color
	float color[4]{ material->color.r, material->color.g, material->color.b, material->color.a };
	bytes = sizeof(float) * 4;
	memcpy(cursor, color, bytes);
	cursor += bytes;

	std::string path = ("/Library/Materials/");
	path.append(std::to_string(material->ID));

	uint ret = App->filesystem->Save(path.c_str(), data, size);

	RELEASE_ARRAY(data);
	
	LoadMaterialResource(material->ID);

	return &path;
}

Material_R * MaterialImporter::LoadMaterialResource(u64 ID)
{
	std::string path = "/Library/Materials/";
	path.append(std::to_string(ID));

	char* buffer;
	uint size = App->filesystem->Load(path.c_str(), &buffer);

	if (size > 0)
	{
		Material_R* material = new Material_R();
		char* cursor = buffer;

		uint name_size = 0;
		uint bytes = sizeof(uint);
		memcpy(&name_size, cursor, bytes);
		cursor += bytes;
		if (name_size > 0) {
			char* name = new char[name_size + 1];
			bytes = sizeof(char) * name_size;
			memcpy(name, cursor, bytes); 
			cursor += bytes;

			name[name_size] = '\0';
			material->name = name;
			RELEASE_ARRAY(name);
		}

		bytes = sizeof(uint64);
		memcpy(&material->tex_ID_D, cursor, bytes);
		cursor += bytes;

		memcpy(&material->tex_ID_S, cursor, bytes);
		cursor += bytes;

		memcpy(&material->tex_ID_H, cursor, bytes);
		cursor += bytes;

		memcpy(&material->tex_ID_AO, cursor, bytes);
		cursor += bytes;

		bytes = sizeof(float) * 4;
		float color[4];
		memcpy(color, cursor, bytes);
		cursor += bytes;
		material->color = Color(color[0], color[1], color[2], color[3]);

		material->resource_path = path;
		material->ID = ID;

		RELEASE_ARRAY(buffer);

		return material;
	}
	return nullptr;
}

bool MaterialImporter::LoadCheckers()
{
	//CREATE CHEKERS TEXTURE
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


	return false;
}


Texture* MaterialImporter::SaveTexture(const char* str, aiTextureType type) {
	Texture* tex = new Texture();
	bool ret = false;
	ret = LoadTexture(str, tex->id, tex->size);
	if (ret) {
		tex->type = type;
		tex->path = str;
		LOGC("Loaded Texture: %s", str);
		return tex;
	}
	else
		LOGC("Textures not found");
	return nullptr;
}

bool MaterialImporter::LoadTexture(const char*path, uint &id, vec2 &size) {

	ILuint image;

	ilGenImages(1, &image);
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

		glTexImage2D(GL_TEXTURE_2D, 0, f, v, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata);
		glGenerateMipmap(GL_TEXTURE_2D);

		ilDeleteImages(1, &image);
	}

	return true;
}



