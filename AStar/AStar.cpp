#include "AStar.h"
#include <algorithm>


AStar::AStar()
{
}

AStar::~AStar()
{
    // 초기화.
    Clear();
}

std::vector<Position> AStar::FindPath(const Position& startPosition, const Position& goalPosition, std::vector<std::vector<int>>& grid)
{
    return std::vector<Position>();
}

void AStar::DisplayGridWithPath(std::vector<std::vector<int>>& grid, const std::vector<Position>& path)
{
}

void AStar::Clear()
{
    // 탐색 과정에서 생성했던 모든 노드 삭제 및 해제.
    for (Node*& node : allocatedNodes)
    {
        delete node; 
        node = nullptr;
    }

    allocatedNodes.clear();
    openList.clear();
    closedList.clear();

    startNode = nullptr;
    goalNode = nullptr;
}

Node* AStar::CreateNode(const Position& position, Node* parent)
{
    // 노드를 생성하고, allocatedNodes에 추가.
    Node* newNode = new Node(position, parent);
    allocatedNodes.emplace_back(newNode);



    // 생성한 노드 반환.
    return newNode;
}

std::vector<Position> AStar::ConstructPath(Node* destination)
{
    // 목표 노드로부터 부모 노드를 따라 경로 역추적.
    std::vector<Position> path;
    Node* current = destination;

    while (current)
    {
        // 현재 노드는 경로 배열에 추가.
        path.emplace_back(current);
        
        // 부모 노드로 이동해서 경로를 역추적.
        current = current->parent;
    }

    // 루프가 종료되면 path에는 반대 방향의 경로 정보가 저장됨.
    // 따라서 다시 역방향으로 뒤집기가 필요.
    std::reverse(path.begin(), path.end());

    return path;
}

float AStar::CalculateHeuristic(const Position& current, const Position& goal) const
{
    // 현재 위치와 목표 위치 사이의 차이를 계산.
    int diffX = std::abs(current.x - goal.x);
    int diffY = std::abs(current.y - goal.y);

    // 대각선 거리와 남은 직선 거리 분리.
    int diagonalDistance = std::min(diffX, diffY);
    int straightDistance = std::max(diffX, diffY)-diagonalDistance;

    // 대각선 비용.
    const float diagonalCost = 1.41421f;
    const float straightCost = 1.0f;


	return diagonalDistance * diagonalCost + straightDistance * straightCost;
}

bool AStar::IsValidGrid(const std::vector<std::vector<int>>& grid) const
{
    // 그리드가 비어있따면 유효하지 않음.
    if (grid.empty())
    {
        return false;
    }

    // 행마다 길이가 같은지 확인.
    size_t width = grid[0].size();
    for (const std::vector<int>& row : grid)
    {
        // 앞에서 구한 행의 길이와 다른 행이 나타나면 유효하지 않다고 판정.
        if (row.size() != width)
        {
            return false;
        }
    }
    
    //검사를 통과하면 유효함.
    return true;
}

bool AStar::IsInRange(int x, int y, const std::vector<std::vector<int>>& grid) const
{
    // grid의 가로 크기는 같다고 가정.
	return x >= 0 && x < static_cast<int>(grid[0].size())
		&& y >= 0 && y < static_cast<int>(grid.size());
}

Node* AStar::FindOpenNode(int x, int y) const
{
    return nullptr;
}

bool AStar::IsInClosedList(int x, int y) const
{
    return false;
}

bool AStar::IsDestination(const Node* node) const
{
    // 두 노드 모두 null이 아니고, 두 노드의 위치가 같은지 비교.
    return node && goalNode && node->position==goalNode->position;
}

void AStar::ClearVisualization(std::vector<std::vector<int>>& grid) const
{
}

void AStar::DisplayeGrid(std::vector<std::vector<int>>& grid) const
{
}
