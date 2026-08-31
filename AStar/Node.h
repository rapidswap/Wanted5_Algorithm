#pragma once

// 그리드의 좌표를 저장할 클래스.
class Position
{
public:
	Position(int x, int y)
		:x(x), y(y)
	{

	}

	bool operator==(const Position& other)const
	{
		return x == other.x && y == other.y;
	}


public:
	// 가로 세로 위치.
	int x = 0;
	int y = 0;

};

// A* 탐색에 사용되는 노드 클래스.
class Node
{
public:
	Node(const Position& position, Node* parent = nullptr)
		:position(position), parent(parent)
	{
	}


public:
	// 노드의 위치.
	Position position;

	// 노드의 비용 정보.
	float gCost = 0.0f;
	float hCost = 0.0f;
	float fCost = 0.0f;

	// 최종 경로를 역추적할 때 사용할 부모 노드.
	Node* parent = nullptr;
};