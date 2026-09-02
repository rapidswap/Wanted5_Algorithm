#pragma once
#include "Node.h"
#include <vector>
class QuadTree
{
public:
	QuadTree(const Bounds& bounds);
	~QuadTree();

	// 트리에 노드를 추가하는 함수.
	bool Insert(Node* node);

	// 전달한 노드와 겹치는 노드를 반환하는 함수.
	std::vector<Node*>Query(Node* queryNode);

public:
	// 트리의 최대 깊이.
	static const int maxDepth = 4;

private:
	// 루트 노드.
	Node* root = nullptr;
};

