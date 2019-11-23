#pragma once
#include "Application.h"
#include "MathGeoLib/include/MathGeoLib.h"


#define QUADTREE_MAX_ITEMS 1

class GameObject;
class QuadTreeNode;

class Quadtree
{
public:
	Quadtree(AABB& box);
	~Quadtree();

	void Draw();

	void Insert(GameObject * gameObject);

	void Remove(GameObject * gameObject);

	void Clear();

private:
	QuadTreeNode* tree_node = nullptr;

};

class QuadTreeNode {
	friend class Quadtree;
public:

	QuadTreeNode(AABB& box);
	~QuadTreeNode();
	void Insert(GameObject * col);

	void Split();

	void Redistribute();

	bool SendToChilds(GameObject * gameObject);

	bool Remove(GameObject * gameObject);

	void RemoveChilds(GameObject * gameObject, uint & i);

	void GetChilds(std::vector<GameObject*>& vector, QuadTreeNode * cur_node);

	void Draw();

	QuadTreeNode* newQuadTreeNode(QuadTreeNode* parent, uint corner);

	std::vector<GameObject*> bucket;
private:
	AABB box;
	//std::vector<GameObject*> bucket;
	QuadTreeNode* parent = nullptr;
	std::vector<QuadTreeNode*> childs;
};