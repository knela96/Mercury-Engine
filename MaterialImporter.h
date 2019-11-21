#ifndef __MATERIALIMPORTER_H__
#define __MATERIALIMPORTER_H__
#include "Application.h"
#include "Globals.h"
#include "Module.h"

class Material_R;
class Texture_R;
struct aiMaterial;
enum aiTextureType;

class MaterialImporter : public Module
{
public:
	MaterialImporter(Application* app, bool start_enabled = true);
	~MaterialImporter();

	void ImportMaterialResource(string * path, aiMaterial * material);
	void getMaterialTextures(string * path, Material_R & material, aiMaterial * mat, aiTextureType type);

	bool SaveMaterialResource(Material_R * material);

	Material_R* LoadMaterialResource(u64 ID);

	uint64 ImportTextureResource(char* buffer,const char* path, uint size);

	Texture_R * LoadTextureResource(uint64 ID);

	bool LoadCheckers();

	Texture * SaveTexture(const char * str, aiTextureType type);

	bool LoadTexture(const char * path, uint & id, vec2 & size);

private:
	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	uint checkImage_id;
};
#endif

