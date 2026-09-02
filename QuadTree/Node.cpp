#include "Node.h"
#include "QuadTree.h"

Node::Node(const Bounds& bounds, int depth)
    :bounds(bounds),depth(depth)
{
}

Node::~Node()
{
    Clear();
}

bool Node::Insert(Node* node)
{
    // 예외 처리.
    if (!node)
    {
        return false;
    }

    // 현재 영역에 포함되지 않으면 삽입처리 안함.
    if (!bounds.Contains(node->GetBounds()))
    {
        return false;
    }

    // 객체가 사입될 4분면 확인.
    NodeIndex result = TestRegion(node->GetBounds());

    // 여러 사분면에 걸쳐있거나 더이상 나눌 수(분할)없으면 현재 노드에 저장.
    if (result == NodeIndex::Staraddling || !Subdivide())
    {
        points.emplace_back(node);
        return true;
    }
    
    // 완전히 포함되는 자식 노드에 삽입.
    if (result == NodeIndex::TopLeft)
    {
        return topLeft->Insert(node);
    }
    if (result == NodeIndex::TopRight)
    {
        return topRight->Insert(node);
    }
    if (result == NodeIndex::BottomLeft)
    {
        return bottomLeft->Insert(node);
    }
    if (result == NodeIndex::BottomRight)
    {
        return bottomRight->Insert(node);
    }

    return false;
}

void Node::Query(const Bounds& queryBounds, std::vector<Node*>& results) const
{
    // 검사 영역과 겹치지 않으면 더이상 검사 안함.
    if (!bounds.Intersects(queryBounds))
    {
        return;
    }

    // 현재 노드에 저장된 객체를 실제 영역과 비교.
    for (Node* const point : points)
    {
        if (point && point->GetBounds().Intersects(queryBounds))
        {
            results.emplace_back(point);
        }
    }

    // 분할된 경우라면 자식 노드까지 검사.
    if (IsDivided())
    {
        topLeft->Query(queryBounds, results);
        topRight->Query(queryBounds, results);
        bottomLeft->Query(queryBounds, results);
        bottomRight->Query(queryBounds, results);
    }
}

void Node::Clear()
{
    // points에 추가된 노드는 외부에 존재하는 노드.
    // 따라서 이 범위에서 메모리 관리하지 않음.

    // 리스트 정리.
    points.clear();

    //자식 노드는 직접 관리.
    ClearChildren();
}

bool Node::Subdivide()
{
    // 최대 깊이 확인.
    if (depth >= QuadTree::maxDepth)
    {
        return false;
    }

    // 분할됐으면 반환.
    if (IsDivided())
    {
        return true;
    }
    // 영역을 나누기 위한 값.
    int x = bounds.GetX();
    int y = bounds.GetY();
    int halfWidth = bounds.GetWidth() / 2;
    int halfHeight = bounds.GetHeight() / 2;

    //4 분할을 담당하는 객체 생성.
	topLeft = new Node(Bounds(x, y, halfWidth, halfHeight), depth + 1);
    topRight = new Node(Bounds(x + halfWidth, y, halfWidth, halfHeight),depth + 1);
    bottomLeft = new Node(Bounds(x, y + halfHeight, halfWidth, halfHeight), depth + 1);
    bottomRight = new Node(Bounds(x+halfWidth, y + halfHeight, halfWidth, halfHeight), depth + 1);

    // 아직 분할되지 않았으면 분할 진행.
    return true;
}

bool Node::IsDivided() const
{
    // 자손 노드 4개 중 하나라도 null이 아니면 분할됐다고 판단.
    
    return topLeft!=nullptr;
}

Node::NodeIndex Node::TestRegion(const Bounds& targetBounds) const
{
    // 현재 노드의 영역을 벗어난 경우 확인.
    if (!bounds.Contains(targetBounds))
    {
        return NodeIndex::OutOfArea;
    }

    // 전달된 영역이 어느 사분면에 포함되는지 확인.
	std::vector<Node::NodeIndex> quads = GetQuads(targetBounds);

    // 반환받은 배열의 요소 개수가 1개라면, 4분면 중 1개 영역에 완전 포함.
    if (quads.size() == 1)
    {
        return quads[0];
    }

    // 여러 영역에 겹쳤다고반환.
    return NodeIndex::Staraddling;
}

std::vector<Node::NodeIndex> Node::GetQuads(const Bounds& targetBounds) const
{
    // 겹치는 영역을 포함할 배열.
    std::vector<NodeIndex> quads;

    // 영역 계산에 필요한 변수.
    int x = bounds.GetX();
    int y = bounds.GetY();
    int halfWidth = bounds.GetWidth() / 2;
    int halfHeight = bounds.GetHeight() / 2;
    int centerX = x + halfWidth;
    int centerY = y + halfHeight;

    // 왼쪽 영역에 완전히 포함되는지 확인.
    bool left = targetBounds.GetX() >= x && targetBounds.GetXMax() <= centerX;
    // 왼쪽 영역에 완전히 포함되는지 확인.
    bool right = targetBounds.GetX() >= centerX && targetBounds.GetXMax() <= bounds.GetXMax();

    // 위쪽 영역에 완전히 포함되는지 확인.
    bool top = targetBounds.GetY() >= y && targetBounds.GetYMax() <= centerY;

    // 아래쪽 영역에 완전히 포함되는지 확인.
    bool bottom = targetBounds.GetY() >= centerY && targetBounds.GetYMax() <= bounds.GetYMax();

    if (top && left)
    {
        quads.emplace_back(NodeIndex::TopLeft);
    }
    if (top && right)
    {
        quads.emplace_back(NodeIndex::TopRight);
    }
    if (bottom && left)
    {
        quads.emplace_back(NodeIndex::BottomLeft);
    }
    if (bottom && right)
    {
        quads.emplace_back(NodeIndex::BottomRight);
    }

    return quads;
}

void Node::ClearChildren()
{
    // 분할이 진행됐는지 확인.
    if (IsDivided())
    {
        SafeDelete(topLeft);
        SafeDelete(topRight);
        SafeDelete(bottomLeft);
        SafeDelete(bottomRight);
    }
}
