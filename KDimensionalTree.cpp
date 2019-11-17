#include "KDimensionalTree.h"
#include "Application.h"



KDimensionalTree::KDimensionalTree()
{
	Node *root = NULL;

	float3 points[] = { {30, 40, 10}, {5, 25,20}, {70, 70,30},
					  {10, 12,40}, {50, 30,50}, {35, 45,60} };

	int n = sizeof(points) / sizeof(points[0]);

	for (int i = 0; i < n; i++)
		root = insert(root, &points[i]);

	root = deleteNode(root, &points[0]);
	LOGC("%f-%f-%f",root->point[0],root->point[1], root->point[2]);
}


KDimensionalTree::~KDimensionalTree()
{
}

Node* KDimensionalTree::newNode(float3* arr)
{
	Node* node = new Node();

	node->point = arr;

	node->left = nullptr;
	node->right = nullptr;
	return node;
}

Node* KDimensionalTree::insertRec(Node *root, float3* point, uint depth)
{

	if (root == nullptr)
		return newNode(point);

	uint current_d = depth % MAX_DIM;

	if (point->At(current_d) < (root->point->At(current_d)))
		root->left = insertRec(root->left, point, depth + 1);
	else
		root->right = insertRec(root->right, point, depth + 1);

	return root;
}

Node* KDimensionalTree::insert(Node *root, float3* point)
{
	return insertRec(root, point, 0);
}

Node* KDimensionalTree::minNode(Node *x, Node *y, Node *z, int d)
{
	Node *res = x;
	if (y != NULL && y->point->At(d) < res->point->At(d))
		res = y;
	if (z != NULL && z->point->At(d) < res->point->At(d))
		res = z;
	return res;
}

Node* KDimensionalTree::findMinRec(Node* root, int d, uint depth)
{
	if (root == nullptr)
		return nullptr;

	uint cd = depth % MAX_DIM;

	if (cd == d)
	{
		if (root->left == nullptr)
			return root;
		return findMinRec(root->left, d, depth + 1);
	}

	return minNode(root,
		findMinRec(root->left, d, depth + 1),
		findMinRec(root->right, d, depth + 1), d);
}

Node* KDimensionalTree::findMin(Node* root, int d)
{
	return findMinRec(root, d, 0);
}

Node* KDimensionalTree::deleteNodeRec(Node *root, float3* point, uint depth)
{
	if (root == nullptr)
		return nullptr;

	uint current_d = depth % MAX_DIM;

	if (root->point != point)
	{
		if (root->right != nullptr)
		{
			Node *min = findMin(root->right, current_d);
			root->point = min->point;
			root->right = deleteNodeRec(root->right, min->point, depth + 1);
		}
		else if (root->left != NULL)
		{
			Node *min = findMin(root->left, current_d);
			root->point = min->point;
			root->right = deleteNodeRec(root->left, min->point, depth + 1);
		}
		else
		{
			delete root;
			return NULL;
		}
		return root;
	}

	// Recursively search point
	if (point->At(current_d) < root->point->At(current_d))
		root->left = deleteNodeRec(root->left, point, depth + 1);
	else
		root->right = deleteNodeRec(root->right, point, depth + 1);
	return root;
}

Node* KDimensionalTree::deleteNode(Node *root, float3* point)
{
	return deleteNodeRec(root, point, 0);
}