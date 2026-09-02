#include "QuadTree.h"

QuadTree::QuadTree(const Bounds& bounds)
    :root(new Node(bounds))
{
}

QuadTree::~QuadTree()
{
    // 루트 노드 삭제.
    SafeDelete(root);
}

bool QuadTree::Insert(Node* node)
{
    if (!node)
    {
        return false;
    }


    return root->Insert(node);
}

std::vector<Node*> QuadTree::Query(Node* queryNode)
{
    if (!queryNode)
    {
        return {};
    }

    // 노드의 영역과 겹치는 노드 검색.
    std::vector<Node*> intersects;
    root->Query(queryNode->GetBounds(), intersects);

    // 검사한 결과 반환.
    return intersects;
}
