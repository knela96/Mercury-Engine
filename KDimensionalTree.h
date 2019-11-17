#pragma once
#include "Application.h"
#include "MathGeoLib/include/MathGeoLib.h"

#define MAX_DIM 3
struct Node {
	float3* point;
	Node *left, *right;
};


class KDimensionalTree
{
public:
	KDimensionalTree();
	~KDimensionalTree();

	Node * newNode(float3 * arr);

	Node * insertRec(Node * root, float3 * point, uint depth);

	Node * insert(Node * root, float3 * point);

	Node * minNode(Node * x, Node * y, Node * z, int d);

	Node * findMinRec(Node * root, int d, unsigned depth);

	Node * findMin(Node * root, int d);

	bool arePointsSame(float3 * point1, float3 * point2);

	void copyPoint(float3 & p1, float3 & p2);

	Node * deleteNodeRec(Node * root, float3 * point, uint depth);

	Node * deleteNode(Node * root, float3 * point);



private:
	int visited = 0;

};

