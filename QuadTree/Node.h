#pragma once

#include "Bounds.h"
#include <vector>

// 메모리 삭제 함수.
template<typename T>
void SafeDelete(T*& t)
{
	if (t)
	{
		delete t;
		t = nullptr;
	}
}

// 쿼드 트리의 사용될 노드 클래스.
class Node
{

	// 영역 구분을 위한 열거형.
	enum class NodeIndex
	{
		// 경계선에 걸치지 않고 완전 포함.
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		Staraddling,	// 경계선에 걸치는 경우.
		OutOfArea		// 영역을 벗어난 경우.
	};

public:
	Node(const Bounds& bounds, int depth = 0);
	~Node();

	// 노드에 삽입할 때 사용할 함수.
	bool Insert(Node* node);

	// 검색 영역과 실제로 겹치는 노드 목록을 반환하는 함수.
	void Query(const Bounds& queryBounds, std::vector<Node*>& results) const;

	// 정리.
	void Clear();

	// Getter.
	const Bounds& GetBounds() const { return bounds; }
	const std::vector<Node*>& Points() const { return points; }

	// 자식 노드.
	Node* TopLeft() { return topLeft; }
	Node* TopRight() { return topRight; }
	Node* BottomLeft() { return bottomLeft; }
	Node* BottomRight() { return bottomRight; }

private:
	// 영역을 4분할 할 때 사용할 함수.
	// 최대 깊이에 도달해서 분할에 실패하면 false 반환.
	bool Subdivide();

	// 영역이 분할됐는지 여부를 확인할 때 사용할 함수.
	bool IsDivided() const;

	// 전달한 영역과 겹치는 영역을 반환하는 함수.
	NodeIndex TestRegion(const Bounds& targetBounds) const;

	// 전달한 영역이 포함되거나 겹치는 영역을 모두 구할 때 사용할 함수.
	std::vector<NodeIndex> GetQuads(const Bounds& targetBounds) const; 
	
	// 자식 노드 정리 함수.
	void ClearChildren();

private:
	// 현재 노드의 깊이.
	int depth = 0;

	// 현재 노드의 영역.
	Bounds bounds;

	// 현재 영역에 포함된 노드.
	std::vector<Node*> points;

	// 자식 노드.
	Node* topLeft = nullptr;
	Node* topRight = nullptr;
	Node* bottomLeft = nullptr;
	Node* bottomRight = nullptr;
};