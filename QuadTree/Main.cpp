#include "QuadTree.h"
#include <iostream>

class Level
{
public:
	Level() = default;
	~Level()
	{
		for (Node*& node : nodes)
		{
			SafeDelete(node);
		}
		nodes.clear();
	}

	// 레벨에 노드 추가하는 함수.
	void AddNewNode(const Bounds& bounds)
	{
		nodes.emplace_back(new Node(bounds));
	}

	// 레벨의 모든 노드를 반환하는 함수.
	const std::vector<Node*>& GettAllNodes() const { return nodes; }

private:
	// 레벨에 배치된 모든 노드(액터, 게임오브젝트).
	std::vector<Node*> nodes;
};

int main()
{
	Level level;
	level.AddNewNode(Bounds(60, 80));
	level.AddNewNode(Bounds(50, 50));
	level.AddNewNode(Bounds(90, 120));
	level.AddNewNode(Bounds(150, 150));
	level.AddNewNode(Bounds(10, 10));

	// 쿼드 트리 생성.
	QuadTree tree(Bounds(0, 0, 200, 200));

	// 레벨의 모든 노드를 트리에 추가.
	for (Node* node : level.GettAllNodes())
	{
		tree.Insert(node);
	}
	
	std::cout << "노드 삽입 완료\n";
	
	// 겹치는 영역 구하는 기능 테스트.
	Node testNode({ 45,45,20,40 });
	std::vector<Node*> intersects = tree.Query(&testNode);

	// 검사 성공 실패.
	if (intersects.empty())
	{
		std::cout << "겹치는 노드를 찾지 못했습니다.\n";
	}
	else
	{
		std::cout << "겹치는 노드를 " << intersects.size() << "개 찾았습니다.\n";

		for (Node* const node : intersects)
		{
			std::cout << node->GetBounds();
		}
	}
}
