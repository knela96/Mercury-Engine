#include "Application.h"
#include "ModuleImporter.h"
#include "MeshObject.h"
#include "Primitive.h"
#include "SDL.h"
#include "C_Transform.h"
#include "Material_R.h"
#include "Texture_R.h"
#include "C_MeshInfo.h"
#include "Mesh_R.h"
#include "Animator.h"
#include "Keyframe.h"
#include "Animation.h"

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

	shader = new Shader();

	//Load("Assets/Models/Street environment_V01.fbx");
	Load("Assets/Models/Spider.fbx");

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

	for (int i = 0; i < strlen(extension.c_str()); i++) {
		extension[i] = toupper(extension[i]); //to Upper letters
	}

	if (extension == "FBX")
		Load(path);
	else if (extension == "PNG" || extension =="DDS" || extension == "TGA") {
		if (App->gui->inspector->active_gameObject != nullptr) {
			if(App->gui->inspector->active_gameObject->textures.size() != 0)
				App->gui->inspector->active_gameObject->textures.pop_back();
			App->gui->inspector->active_gameObject->textures.push_back(SaveTexture(path, aiTextureType_DIFFUSE));
		}
	}

	return true;
}

bool ModuleImporter::Load(const char* path) {
	Resources* resource = nullptr;
	uint64 newID = 0;
	std::string name = "";

	App->filesystem->SplitFilePath(path, nullptr, &name);

	Meta* meta = App->resources->FindMetaResource(path, name.c_str(), ResourceType::ObjectR);
	UID id;
	if (meta == nullptr)
		id = App->resources->GenerateNewUID();
	else
		id = meta->id;

	resource = ImportObject(path, &id);
	if (resource)
	{
		App->resources->AddResource(resource);
	}

	return true;
}



Resources* ModuleImporter::LoadObjectResource(UID id) {
	std::string full_path = ("Library/GameObjects/");
	full_path.append(std::to_string(id));

	Resources* object = nullptr;

	json file;
	ifstream stream;
	stream.open(full_path);
	file = json::parse(stream);

	object = new Resources(ResourceType::ObjectR);
	object->ID = id;
	object->resource_path = full_path.c_str();
	object->original_path = file["Game Objects"]["Source"].get<std::string>();
	object->name = file["Game Objects"]["Name"].get<std::string>();

	stream.close();

	return object;
}


Resources* ModuleImporter::ImportObject(const char* path, UID* id) {

	Resources* resource = nullptr; const aiScene* scene = nullptr;
	string s("/");
	s.append(path);
	if(App->filesystem->Exists(s.c_str()))
		scene = aiImportFileEx(path, aiProcessPreset_TargetRealtime_MaxQuality, App->filesystem->GetAssimpIO());
	if (scene)
	{
		LOG("Starting scene load %s", path);
		std::string name = "";
		App->filesystem->SplitFilePath(path, nullptr, &name);
		vector<aiMesh*> boned_meshes;
		GameObject* rootNode = LoadHierarchy(scene->mRootNode, (aiScene*)scene, path, App->scene_intro->root,&boned_meshes);
		//Import Scene Bones HERE
		int size = 0;
		Joint* joints = ImportMeshBones(&boned_meshes, path, name.c_str(),rootNode, size);
		Timer timer;
		timer.Start();
		Animator* anim = (Animator*)App->scene_intro->root->AddComponent(C_Animator);
		anim->Animations = ImportAnimations(scene);
		anim->myAnimatedBody = new AnimatedObject(App->scene_intro->root, joints, size);

		LOGC("Loading Anims:%f", timer.ReadTicks());

		json config;
		SaveGameObjectConfig(config, rootNode);
		config["Game Objects"]["Name"] = name;
		config["Game Objects"]["Source"] = path;

		std::string full_path = "Library/GameObjects/";
		full_path.append(std::to_string(*id));

		ofstream stream;
		stream.open(full_path);
		stream << setw(4) << config << endl;
		stream.close();

		std::string fileName;
		App->filesystem->SplitFilePath(path, nullptr, &fileName);
		resource = new Resources(ResourceType::ObjectR);
		resource->ID = *id;
		resource->resource_path = full_path.c_str();
		resource->original_path = path;
		resource->name = fileName;
	}
	return resource;
}

vector<Animation*> ModuleImporter::ImportAnimations(const aiScene *scene) {
	vector<Animation*> AnimList;
	
	scene->mNumAnimations;
	//if(scene.HasAnimations){} clog aqui loko

	//HERE WE SELECT 1 ANIM AND SET ITS GENERAL PARAMETERS
	for (int i = 0; i < scene->mNumAnimations; i++) {
		Animation *anim = new Animation();
		anim->setLenght(scene->mAnimations[i]->mDuration);
		anim->keyFrameCount = scene->mAnimations[i]->mNumChannels;
		anim->name = scene->mAnimations[i]->mName.C_Str();
	
		//NOW WE ASSIGN TO ALL BONES ALL THEIR KEYFRAMES
		for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++) {     //numchanels = bone number and j is which bone is
			for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++) {
				//HERE WE ASSIGN ALL <POS> KEYFRAMES TO THE BONE THAT CORRESPONDS TO "j"
				Keyframe* key = nullptr;
				std::map<uint, Keyframe*>::iterator it = anim->keyframes_list.find((uint)k);
				if (it != anim->keyframes_list.end()){
					key = it->second;
				}
				else {
					key = new Keyframe();
				}

				JointTransform* transform = new JointTransform();
				transform->Position = vec3(scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.x,
											scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.y, 
											scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.z);
				transform->Rotation = Quat(scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.x,
											scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.y,
											scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.z,
											scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.w);
				transform->Scale = vec3(scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.x,
											scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.y,
											scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.z);


				key->pose[scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str()] = transform;
				key->TimePosition = k;

				anim->keyframes_list[k] = key;
			}
		}
		anim->keyframes.push_back(anim->keyframes_list.at(anim->keyframes_list.size() - 1));

		Keyframe* prev = nullptr;
		Keyframe* next = nullptr;
		int c = 0;
		bool reset = false;
		bool found_empty = false;

		for (std::map<std::string, JointTransform*>::iterator _it = anim->keyframes_list[0]->pose.begin(); _it != anim->keyframes_list[0]->pose.end(); ++_it) {
			std::string name = _it->first;
			for (int i = 0; i < anim->keyFrameCount; ++i) {
				std::map<uint, Keyframe*>::iterator it = anim->keyframes_list.find(i);
				if (it != anim->keyframes_list.end()) {
					std::map<std::string, JointTransform*>::iterator __it = anim->keyframes_list[i]->pose.find(name);
					if (__it != anim->keyframes_list[i]->pose.end()) {
						prev = anim->keyframes_list[i];
					}
					else {
						next = FindNextFrame(i, name, anim->keyframes_list);

						if (next != nullptr) {
							c = 0;

							InterpolateKeyFrames(prev, next, true, name, anim->keyframes_list);
							next = nullptr;
						}
						else {
							InterpolateKeyFrames(prev, anim->keyframes_list.at(anim->keyframes_list.size() - 1), false, name, anim->keyframes_list);
							break;
						}
					}
				}
			}
		}

		AnimList.push_back(anim);
	}
	return AnimList;
}

Keyframe* ModuleImporter::FindNextFrame(uint index, string& name, std::map<uint, Keyframe*>& map) {
	for (int i = index; i < map.size(); ++i) {
		JointTransform* trans = map[i]->pose[name];
		if (trans != nullptr) {
			return map[i];
		}
	}
	return nullptr;
}

void ModuleImporter::InterpolateKeyFrames(Keyframe* prevFrame, Keyframe* nextFrame, bool found, string& name, std::map<uint, Keyframe*>& map)
{
	JointTransform* prev = prevFrame->pose[name];
	JointTransform* next = nextFrame->pose[name];
	
	uint steps = nextFrame->TimePosition - prevFrame->TimePosition;

	int index = 1;

	for (int i = prevFrame->TimePosition + 1; i <= nextFrame->TimePosition; ++i) {
		JointTransform* currentTransform = nullptr;
		if (map[i]->pose[name] != next && found) {
			currentTransform = &currentTransform->Interpolate(prev, next, index++ / steps);
		}
		else {
			currentTransform = prev;
		}
		map[i]->pose[name] = currentTransform;

	}

}

void ModuleImporter::SaveGameObjectConfig(json& config, GameObject* gameObjects)
{
	uint count = 0;
	App->scene_intro->SaveAllScene(gameObjects, config, count);
	config["Game Objects"]["Count"] = count;
}

//void ModuleImporter::ImportAnim(aiAnimation * animations, aiScene * scene, string * FileName, string * str)
//{
//
//
//
//}


GameObject* ModuleImporter::LoadHierarchy(aiNode* node, aiScene* scene,const char* str, GameObject* parent, vector<aiMesh*>* boned_meshes) {

	std::string name = node->mName.C_Str();
	static const char* transformNodes[5] = {
		"$AssimpFbx$_PreRotation", "$AssimpFbx$_Rotation", "$AssimpFbx$_PostRotation",
		"$AssimpFbx$_Scaling", "$AssimpFbx$_Translation" };

	for (int i = 0; i < 5; i++)
	{
		if (name.find(transformNodes[i]) != string::npos && node->mNumChildren > 0)
		{
			node = node->mChildren[0];
			name = node->mName.C_Str();
			i = -1;
		}
	}

	GameObject* gameObject = nullptr;
	//if (node->mNumMeshes == 0)
	gameObject = new GameObject(node->mName.C_Str());

	for (int i = 0; i < node->mNumMeshes; i++)
	{
		//ACCUMULATE TRANSFORM
		if (gameObject != nullptr) {
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
			gameObject->ID = App->RandomNumberGenerator.GetIntRNInRange();
		}

		aiMesh* newMesh = scene->mMeshes[node->mMeshes[i]];

		if (!strcmp(newMesh->mName.C_Str(), ""))
			newMesh->mName = node->mName;

		//IMPORT STUFF HERE (ERIC)
		//Import Mesh
		UID MeshID = ImportResourceMesh(newMesh, str, newMesh->mName.C_Str());
		if (MeshID != 0) {
			C_MeshInfo* mesh = (C_MeshInfo*)gameObject->AddComponent(Component_Type::Mesh_Info);
			mesh->id = MeshID;
			mesh->resource_name.append(newMesh->mName.C_Str());
		}

		if (newMesh->HasBones()) {
			boned_meshes->push_back(newMesh);
		}

		//Import mesh material
		/*aiMaterial* material = scene->mMaterials[newMesh->mMaterialIndex];
		aiString matName;
		material->Get(AI_MATKEY_NAME, matName);
		uint64 rMaterial = App->moduleResources->ImportRMaterial(material, path, matName.C_Str());
		if (rMaterial != 0)
		{
			C_Material* cMaterial = new C_Material(nullptr);
			cMaterial->SetResource(rMaterial);
			child->AddComponent(cMaterial);
		}*/


	}

	gameObject->parent = parent;

	for (int i = 0; i < node->mNumChildren; ++i) {
		aiNode* child = node->mChildren[i];
		GameObject* go = nullptr;
		go = LoadHierarchy(child, scene, str, gameObject,boned_meshes);
		if (go != nullptr)
			gameObject->childs.push_back(go);
	}

	return gameObject;
}

UID ModuleImporter::ImportResourceMesh(aiMesh* newMesh, const char* str, const char* fileName) {
	UID id = 0;
	uint64 newID = 0;
	Mesh_R* resource = nullptr;
	uint instances = 0;
	Meta* meta = App->resources->FindMetaResource(str, fileName, ResourceType::MeshR);

	if (meta != nullptr)
	{
		newID = meta->id;
		instances = App->resources->DeleteResource(newID);
	}
	else
	{
		newID = App->RandomNumberGenerator.GetIntRNInRange();
	}

	resource = App->mesh_importer->ImportMeshResource(newMesh, str, fileName, newID);
	if (resource)
	{
		App->resources->AddResource(resource);
		resource->instances = instances;
		id = resource->ID;
	}

	return id;
}

UID ModuleImporter::ImportResourceBones(aiMesh* newMesh, const char* str, const char* fileName) {
	UID id = 0;
	uint64 newID = 0;
	Mesh_R* resource = nullptr;
	uint instances = 0;
	Meta* meta = App->resources->FindMetaResource(str, fileName, ResourceType::MeshR);

	if (meta != nullptr)
	{
		newID = meta->id;
		instances = App->resources->DeleteResource(newID);
	}
	else
	{
		newID = App->RandomNumberGenerator.GetIntRNInRange();
	}

	/*resource = ImportMeshBones(newMesh, str, fileName);*/
	if (resource)
	{
		App->resources->AddResource(resource);
		resource->instances = instances;
		id = resource->ID;
	}

	return id;
}


GameObject* ModuleImporter::ProcessMesh( aiMesh* mesh, string* path, const char* fileName, const aiScene* scene) {

	vector<Vertex> vertices;
	vector<uint> indices;
	vector<Texture*> textures;

	math::float3* points = (float3*)malloc(sizeof(float3) * mesh->mNumVertices);

	//LOAD MATERIAL TEXTURES
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	aiColor3D color(1.f, 1.f, 1.f);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, color);

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
			points[i].Set(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
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
		else {
			vertex.Colors = {
				color.r,
				color.g,
				color.b,
				1.0f
			};
		}

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

	LOGC("Loaded Model: %s", mesh->mName.C_Str());
	LOGC("Loaded Vertices: %u", vertices.size());
	LOGC("Loaded Indices: %u", indices.size());
	LOGC("Loaded Textures: %u", textures.size());

	if (mesh->mName.length == 0)
		mesh->mName = fileName;
	GameObject* gameobject = nullptr;
	//GameObject* gameobject = new MeshObject(vertices, indices, textures, mesh->mName.C_Str());
	gameobject->box.SetNegativeInfinity();
	gameobject->box.Enclose(points, mesh->mNumVertices);

	std::free(points);
	return gameobject;
}

Joint* ModuleImporter::ImportMeshBones(vector<aiMesh*> * newMesh, const char * str, const char * fileName, GameObject* root, int& size)
{
	Joint* root_joint = nullptr;
	int count = 0;
	std::map<std::string, aiBone*> bones;
	vector<Joint*> joints;
	for (int i = 0; i < newMesh->size(); ++i) {
		count = 0;
		CollectGameObjectNames(newMesh->at(i), bones, count);

		size += count;
	}

	LoadHierarchyJoints(root, &bones, root_joint, joints);
	return root_joint;
}

void ModuleImporter::LoadHierarchyJoints(GameObject* gameobject, std::map<std::string, aiBone*>* bones, Joint*& joint, vector<Joint*>& joints) {
	for (int i = 0; i < gameobject->childs.size(); ++i) {
		std::map<std::string, aiBone*>::iterator bone_it = bones->find(gameobject->childs[i]->name);
		if (bone_it != bones->end())
		{
			aiBone* bone = bone_it->second;
			Joint* child = new Joint();
			child->name = bone_it->first;
			child->index = joints.size();

			/*child->InverseBindTransform = mat4x4(
			bone->mOffsetMatrix.a1, bone->mOffsetMatrix.b1, bone->mOffsetMatrix.c1, bone->mOffsetMatrix.d1,
			bone->mOffsetMatrix.a2, bone->mOffsetMatrix.b2, bone->mOffsetMatrix.c2, bone->mOffsetMatrix.d2,
			bone->mOffsetMatrix.a3, bone->mOffsetMatrix.b3, bone->mOffsetMatrix.c3, bone->mOffsetMatrix.d3,
			bone->mOffsetMatrix.a4, bone->mOffsetMatrix.b4, bone->mOffsetMatrix.c4, bone->mOffsetMatrix.d4
		).inverse();*/
			child->InverseBindTransform = mat4x4(
				bone->mOffsetMatrix.a1, bone->mOffsetMatrix.a2, bone->mOffsetMatrix.a3, bone->mOffsetMatrix.a4,
				bone->mOffsetMatrix.b1, bone->mOffsetMatrix.b2, bone->mOffsetMatrix.b3, bone->mOffsetMatrix.b4,
				bone->mOffsetMatrix.c1, bone->mOffsetMatrix.c2, bone->mOffsetMatrix.c3, bone->mOffsetMatrix.c4,
				bone->mOffsetMatrix.d1, bone->mOffsetMatrix.d2, bone->mOffsetMatrix.d3, bone->mOffsetMatrix.d4).inverse();
			
			if (joints.size() > 0) {
				joint->children.push_back(child);
				joints.push_back(child);
				LoadHierarchyJoints(gameobject->childs[i], bones, child, joints);
			}
			else {
				joint = child;
				joints.push_back(joint);
				LoadHierarchyJoints(gameobject->childs[i], bones, joint, joints);
			}
		}
		else
			LoadHierarchyJoints(gameobject->childs[i], bones, joint, joints);
	}
}

void ModuleImporter::CollectGameObjectNames(aiMesh* mesh, std::map<std::string, aiBone*>& map, int& count)
{
	if (count < mesh->mNumBones) {
		map[mesh->mBones[count]->mName.C_Str()] = mesh->mBones[count];
		CollectGameObjectNames(mesh, map, ++count);
	}
}

vector<Texture*> ModuleImporter::loadMaterialTextures(string* path, aiMaterial *mat, aiTextureType type)
{
	path->erase();
	path->append("Assets\\Textures");
	vector<Texture*> texture;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		if(path->size() != 0)
			path->append("\\");
		path->append(str.C_Str());
		LOGC("Searching %s", str.C_Str());
		Texture* tex = SaveTexture(path->c_str(), type);
		if(tex != nullptr)
			texture.push_back(tex);
	}
	return texture;
}

Texture* ModuleImporter::SaveTexture(const char* str, aiTextureType type) {
	for (unsigned int j = 0; j < stored_textures.size(); j++)
	{
		if (std::strcmp(stored_textures[j]->path.c_str(), str) == 0)
		{
			return stored_textures[j]; //check if texture is already loaded
		}
	}
	Texture* tex = new Texture();
	bool ret = false;
	ret = LoadTexture(str, tex->id, tex->size);
	if (ret){
		tex->type = type;
		tex->path = str;
		LOGC("Loaded Texture: %s", str);
		stored_textures.push_back(tex); //store to loaded textures
		return tex;
	}else
		LOGC("Textures not found");
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
