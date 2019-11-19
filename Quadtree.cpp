#include "Application.h"
#include "Quadtree.h"
#include "GameObject.h"
#include "Gizmo.h"

Quadtree::Quadtree(AABB& box)
{
	tree_node = new QuadTreeNode(box);
}


Quadtree::~Quadtree()
{
}

void Quadtree::Draw() {
	tree_node->Draw();
}

void Quadtree::Insert(GameObject* gameObject)
{
	tree_node->Insert(gameObject);
}

void Quadtree::Remove(GameObject* gameObject)
{
	tree_node->Remove(gameObject);
}

void Quadtree::Clear()
{
	tree_node->childs.clear();
	tree_node->bucket.clear();
}

//-------------------------------------------

QuadTreeNode::QuadTreeNode(AABB& box){

	this->box = box;

}
QuadTreeNode::~QuadTreeNode() {}

QuadTreeNode* QuadTreeNode::newQuadTreeNode(QuadTreeNode * parent, uint corner)
{
	float3 minPoint, maxPoint; 
	
	minPoint.y = parent->box.minPoint.y;
	maxPoint.y = parent->box.maxPoint.y;

	switch (corner) {
	case 0:
		minPoint.x = parent->box.minPoint.x;
		maxPoint.x = (parent->box.minPoint.x + parent->box.maxPoint.x) / 2;
		minPoint.z = parent->box.minPoint.z;
		maxPoint.z = (parent->box.minPoint.z + parent->box.maxPoint.z) / 2;
		break;
	case 1:
		minPoint.x = (parent->box.minPoint.x + parent->box.maxPoint.x) / 2;
		maxPoint.x = parent->box.maxPoint.x;
		minPoint.z = parent->box.minPoint.z; 
		maxPoint.z = (parent->box.minPoint.z + parent->box.maxPoint.z) / 2;
		break;
	case 2:
		minPoint.x = parent->box.minPoint.x;
		maxPoint.x = (parent->box.minPoint.x + parent->box.maxPoint.x) / 2;
		minPoint.z = (parent->box.minPoint.z + parent->box.maxPoint.z) / 2;
		maxPoint.z = parent->box.maxPoint.z;
		break;
	case 3:
		minPoint.x = (parent->box.minPoint.x + parent->box.maxPoint.x) / 2;
		maxPoint.x = parent->box.maxPoint.x;
		minPoint.z = (parent->box.minPoint.z + parent->box.maxPoint.z) / 2;
		maxPoint.z = parent->box.maxPoint.z;
		break;
	}
	
	QuadTreeNode* node = new QuadTreeNode(AABB(minPoint,maxPoint));
	node->parent = parent;
	return node;
}
void QuadTreeNode::Insert(GameObject* gameobject) {

	if (box.Intersects(gameobject->aabb)) {
		if (childs.size() == 0) {//node childs
			bucket.push_back(gameobject);
			if (bucket.size() > QUADTREE_MAX_ITEMS) {
				Split();
				Redistribute();
			}
		}
		else {
			if (!SendToChilds(gameobject))
				bucket.push_back(gameobject);//set to this node if collides to all childs
		}
	}
}

void QuadTreeNode::Split() {
	for (int i = 0; i < 4; ++i)
		childs.push_back(newQuadTreeNode(this, i));
}

void QuadTreeNode::Redistribute() {
	for (vector<GameObject*>::iterator it = bucket.begin(); it != bucket.end();)
	{
		if (SendToChilds(*it))//send overflow item to a child
		{
			bucket.erase(it);//deletes the last inserted gameobject
		}
		else
			++it;
	}
}

bool QuadTreeNode::SendToChilds(GameObject* gameObject)
{
	bool intersections[4];

	uint count = 0;

	for (uint i = 0; i < childs.size(); i++)
	{
		if (childs[i]->box.Intersects(gameObject->aabb))
		{
			count++;
			intersections[i] = true;
		}else
			intersections[i] = false;
	}
	   
	if (count > 0 && count < 4)
	{
		for (uint i = 0; i < childs.size(); i++)
		{
			if(intersections[i])
				childs[i]->Insert(gameObject);
		}
		return true;
	}
	else if (count == 4) {
		LOGC("set to parent");
	}

	return false; //Not found / needs to be in Parent
}

bool QuadTreeNode::Remove(GameObject* gameObject)
{
	uint i = 4;
	for (std::vector<GameObject*>::iterator it = bucket.begin(); it != bucket.end(); it++)
	{
		if (*it == gameObject)
		{
			bucket.erase(it);
			RemoveChilds(gameObject,i);
			return true;
		}
	}
	if (i == 0)
		LOGC("UNSPLIT");

	i = 4;
	for (uint i = 0; i < childs.size(); i++)
	{
		if (childs[i]->Remove(gameObject))
		{
			RemoveChilds(gameObject,i);
			return true;
		}

	}



	return false;
}

void QuadTreeNode::RemoveChilds(GameObject* gameObject,uint& i)
{
	std::vector<GameObject*> childs;
	GetChilds(childs,this);

	if (childs.size() > 0) {
		for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end();)
		{
			if (*it == gameObject)
			{
				childs.erase(it);
			}
			else
				++it;
		}

		for (uint i = 0; i < childs.size(); i++)
		{
			bucket.push_back(childs[i]);
		}
		bucket.clear();
	}

	i = childs.size();
}


void QuadTreeNode::GetChilds(vector<GameObject*> &vector, QuadTreeNode* cur_node) {
	if (cur_node != this) {
		for (uint i = 0; i < bucket.size(); i++)
		{
			vector.push_back(bucket[i]);
		}
	}

	for (uint i = 0; i < childs.size(); i++)
	{
		childs[i]->GetChilds(vector,nullptr);
	}
}

void QuadTreeNode::Draw()
{
	Gizmo::DrawBox(box, Color(0.0f, 1.0f, 0.0f, 1.0f));

	for (int i = 0; i < childs.size(); ++i) {
		childs[i]->Draw();
	}
}