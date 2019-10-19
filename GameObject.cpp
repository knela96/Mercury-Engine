#include "GameObject.h"
#include "Application.h"

GameObject::GameObject(MeshObject* mesh, vector<Texture> textures, string name) : mesh(mesh), textures(textures), position({ 0,0,0 }), rotation({ 0,0,0 }), scale({ 1,1,1 }), name(name) {
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

char* GameObject::getType(aiTextureType type)
{
	switch (type) {
	case aiTextureType_DIFFUSE:
		return "Diffuse Map";
		break;
	case aiTextureType_SPECULAR:
		return "Specular Map";
		break;
	case aiTextureType_NORMALS:
		return "Normal Map";
		break;
	case aiTextureType_HEIGHT:
		return "Height Map";
		break;
	}
}