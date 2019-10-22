#include "GameObject.h"
#include "Application.h"
#include "MeshObject.h"

GameObject::GameObject(MeshObject* mesh, vector<Texture*> textures, string name) : mesh(mesh), textures(textures), position({ 0,0,0 }), rotation({ 0,0,0 }), scale({ 1,1,1 }), name(name) {
	face_normals = false;
	vertex_normals = false;
}

GameObject::~GameObject() {}

void GameObject::drawChilds() {
	for(int i = 0; i < childs.size(); ++i){
		childs[i]->Draw();
		childs[i]->drawChilds();
	}
}

void GameObject::CleanUp() {
	mesh->CleanUp();
	delete mesh;
	mesh = nullptr;
}

char* GameObject::getType(aiTextureType type)
{
	switch (type) {
	case aiTextureType_DIFFUSE:
		return "Diffuse_Map";
		break;
	case aiTextureType_SPECULAR:
		return "Specular_Map";
		break;
	case aiTextureType_NORMALS:
		return "Normal_Map";
		break;
	case aiTextureType_HEIGHT:
		return "Height_Map";
		break;
	}
}