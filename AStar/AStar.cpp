#include "AStar.h"
#include <algorithm>
#include <iostream>

#define NOMINMAX
#include <Windows.h>

AStar::AStar()
{
}

AStar::~AStar()
{
    // 초기화.
    Clear();
}

// 최적화? 병목을 찾아 적절한 해결방법을 적용.
// -> 안해도 되는 계산을 찾아서 안하게 만드는 것.
std::vector<Position> AStar::FindPath(const Position& startPosition, const Position& goalPosition, std::vector<std::vector<int>>& grid)
{
    // 이전에 탐색한 결과 초기화.
    Clear();

    // 예외처리.
    if (!IsValidGrid(grid))
    {
        return std::vector<Position>(); //{}; STL -> initiallize_list
    }

    // 시작위치/목표위치가 grid 기준에서 문제 없는 위치 값인지 확인.
    if (!IsInRange(startPosition.x, startPosition.y, grid) || !IsInRange(goalPosition.x, goalPosition.y, grid))
    {
        return std::vector<Position>();
    }

    // 시작위치/목표위치가 이동 불가하면 종료.
    if (grid[startPosition.y][startPosition.x] == static_cast<int>(TileType::Wall)
        || grid[goalPosition.y][goalPosition.x] == static_cast<int>(TileType::Wall))
    {
        return std::vector<Position>();
    }

    // 이전 탐색 과정의 시각화 제거(기존에 방문 처리한 값이 있으면 제거).
    ClearVisualization(grid);

    // 탐색 시작.
    // 시작/목표 노드 생성.
    startNode = CreateNode(startPosition);
    goalNode = CreateNode(goalPosition);


    // 시작 노드의 비용 계산 및 openList에 추가해 탐색 시작.
    startNode->gCost = 0.0f;
    startNode->hCost = CalculateHeuristic(startPosition, goalPosition);
    startNode->fCost = startNode->gCost + startNode->hCost;

    openList.emplace_back(startNode);

    // 편의를 위해 사전 비용 설정.
    const float diagonalCost = 1.41421f;
    const std::vector<Direction> directions =
    {   {0,-1,1.0f},{0,1,1.0f},                     // 상하
        {-1,0,1.0f},{1,0,1.0f},                     // 좌우
        {-1,-1,diagonalCost},                       // 좌상단 방향
        {1,-1,diagonalCost},                        // 우상단 방향 
        {-1,1,diagonalCost},                        // 좌하단 방향
        {1,1,diagonalCost} };                       // 우하단 방향

    // openList가 빌 때까지 탐색 반복.
    while (!openList.empty())
    {
        // openList에서 fCost가 가장 작은 노드를 선택.
        Node* currentNode = openList[0];
        for (Node* node : openList)
        {
            // 더 작은 비용의 노드 검색.
            if (node->fCost < currentNode->fCost ||
                (node->fCost == currentNode->fCost && node->hCost < currentNode->hCost))
            {
                currentNode = node;
            }
        }

        // 목표 노드인지 확인.
        if (IsDestination(currentNode))
        {
            // 이동 경로 제작 후 반환.
            return ConstructPath(currentNode);
        }

        // 현재 노드를 openList에서 제거.
        // 방문 처리를 위해.
        auto iterator = std::find(openList.begin(), openList.end(),currentNode);
        // 검색에 성공했는지 확인.
        if (iterator != openList.end())
        {
            // openList에서 제거.
            openList.erase(iterator);
        }

        // 탐색을 마친 노드를 closedList에 추가.
        closedList.emplace_back(currentNode);

        // 현재 위치를 기준으로 주변(8방향)의 이웃노드를 탐색.
        for (const Direction& direction : directions)
        {
            // 현재 노드를 기준으로 인접한 노드의 좌표 계산.
            // 새로운 좌표(위치) = 현재위치 + 이동방향.
            int newX = currentNode->position.x + direction.x;
            int newY = currentNode->position.y + direction.y;

            // 예외 처리.
            if (!IsInRange(newX, newY, grid))
            {
                continue;
            }

            // 새로운 위치가 장애물인지 확인.
            if (grid[newY][newX] == static_cast<int>(TileType::Wall))
            {
                continue;
            }

            // 대각선 이동 시 장애물을 통과하는지 확인.
            if (IsDiagonalBlocked(currentNode->position, direction, grid))
            {
                continue;
            }

            // 이미 방문한 곳이라면 건너뛰기.
            if (IsInClosedList(newX, newY))
            {
                continue;
            }

            // 현재 노드를 거쳐서 새로운 위치로 가는데 드는 비용 계산.
            float newGCost = currentNode->gCost + direction.cost;

            // 이미 openList에 있는데 비용면에서 더 나은지 확인.
            Node* openNode = FindOpenNode(newX, newY);
            if (openNode)
            {
                // 비용 비교.
                if (newGCost < openNode->gCost)
                {
                    openNode->gCost = newGCost;
                    openNode->fCost = openNode->gCost+openNode->hCost;
                    openNode->parent = currentNode;
                }

                continue;
            }

            // 이웃 노드 생성 및 openList 추가.
            Node* neighborNode = CreateNode(Position(newX, newY),currentNode);


            // 새로운 노드의 비용 계산.
            neighborNode->gCost = newGCost;
            neighborNode->hCost = CalculateHeuristic(neighborNode->position, goalNode->position);
            neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

            // 새로운 노드를 openList에 추가.
            openList.emplace_back(neighborNode);

            // 옵션: 시각화를 위한 처리.
            if (grid[newY][newX] == static_cast<int>(TileType::Ground))
            {
                grid[newY][newX] = static_cast<int>(TileType::Visited);
            }

            // grid 그리기.
            DisplayGrid(grid);

            // 스레드 재우기(애니메이션 처럼 단순하게 프레임을 만들기 위해).
            DWORD delay = static_cast<DWORD>(0.1f * 1000);
            Sleep(delay);


        }

    }

    // 빈 경로 반환(탐색 실패).
    return std::vector<Position>();
}

void AStar::DisplayGridWithPath(std::vector<std::vector<int>>& grid, const std::vector<Position>& path)
{
    // 기존에 시각화를 위해 사용했던 값 복구.
    ClearVisualization(grid);

    // 맵 그리기.
    DisplayGrid(grid);

    static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    int green = FOREGROUND_GREEN;

    // 이동 경로 그리기.
    for (const Position& position : path)
    {
        // 경올 위치의 타일 값 읽기.
        int value = grid[position.y][position.x];
        if (value == static_cast<int>(TileType::Start) || value == static_cast<int>(TileType::Goal))
        {
            continue;
        }
        
        // 콘솔 좌표.
        COORD cursorPosition;
        cursorPosition.X = static_cast<short>(position.x * 2);
        cursorPosition.Y = static_cast<short>(position.y);
        
        // 커서 이동.
        SetConsoleCursorPosition(handle, cursorPosition);

        // 텍스트 색 지정.
        SetConsoleTextAttribute(handle, green);

        // 글자 출력.
        std::cout << "* ";

        // 스레드 재우기.
        DWORD delay = static_cast<DWORD>(0.1f * 1000);
        Sleep(delay);

    }

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
        path.emplace_back(current->position);
        
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
    // 옥타일(8방향) 비용 계산법.
    // 대각선 이동 허용시 주의사항.
    // -> 대각선 형태의 장애물을 뚫고 가지 못하게 막아야 함.

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

bool AStar::IsDiagonalBlocked(const Position& current, const Direction& direction, const std::vector<std::vector<int>>& grid) const
{
    // 이동하려는 방향에 장애물이 있는지 확인
    // 대각선 성분만 판단.
    // 대각선 성분이 아니라면 판단할 필요 없음.
    // 대각선 방향의 x,y 성분은 모두 0이 아니기 때문.
    if (direction.x == 0 || direction.y == 0)
    {
        return false;
    }

    // 대각선으로 이동하려는 새로운 위치의  x성분과 y성분을 분해.
    int sideX = current.x + direction.x;
    int sideY = current.y + direction.y;



    return grid[current.y][sideX] ==static_cast<int>(TileType::Wall) || grid[sideY][current.x] == static_cast<int>(TileType::Wall);
}

Node* AStar::FindOpenNode(int x, int y) const
{
    // 같은 좌표의 노드를 OpenList에서 찾기.
    for (Node* node : openList)
    {
        // 좌표 비교.
        if (node->position == Position(x, y))
        {
            return node;
        }
    }

    return nullptr;
}

bool AStar::IsInClosedList(int x, int y) const
{
    // 같은 좌표가 ClosedList에 있는지 확인.
    for (Node* node : closedList)
    {
        if (node->position == Position(x, y))
        {
            return true;
        }
    }
    return false;
}

bool AStar::IsDestination(const Node* node) const
{
    // 두 노드 모두 null이 아니고, 두 노드의 위치가 같은지 비교.
	return node && goalNode && node->position == goalNode->position;
}

void AStar::ClearVisualization(std::vector<std::vector<int>>& grid) const
{
    // 탐색 후보 표시해둔 것을 다시 원상 복구.
    // 탐색 후보로 표시해뒀다는 건 원래 이동 가능한 위치였다는 것을 의미.
    for (std::vector<int>& row : grid)
    {
        for (int& value : row)
        {
            if (value == static_cast<int>(TileType::Visited))
            {
                // 숫자로 0.
                value = static_cast<int>(TileType::Ground);
            }
        }
    }
}

void AStar::DisplayGrid(std::vector<std::vector<int>>& grid) const
{
    // 커서 원점(0,0)으로 이동.
    static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    static COORD origin = { 0,0 };
    SetConsoleCursorPosition(handle, origin);

    // 글자 색.
    
    int red = FOREGROUND_RED; // 1 << 0
    int green = FOREGROUND_GREEN; // 1 << 1
    int blue = FOREGROUND_BLUE; // 1 << 2
    int white = red + green + blue;

    // grid 순회하면서 글자 그리기.
    for (int y = 0;y < static_cast<int>(grid.size());++y)
    {
        for (int x = 0;x < static_cast<int>(grid[y].size());++x)
        {
            // 타일값에 따라 글자 색상 및 글자 지정해서 출력.
            if (grid[y][x] == static_cast<int>(TileType::Start))
            {
                SetConsoleTextAttribute(handle, red);
                std::cout << "S ";
            }
            else if (grid[y][x] == static_cast<int>(TileType::Goal))
            {
                SetConsoleTextAttribute(handle, red);
                std::cout << "G ";
            }
            else if (grid[y][x] == static_cast<int>(TileType::Wall))
            {
                SetConsoleTextAttribute(handle, white);
                std::cout << "1 ";
            }
            else if (grid[y][x] == static_cast<int>(TileType::Visited))
            {
                SetConsoleTextAttribute(handle, green);
                std::cout << "+ ";
            }
            else
            {
                SetConsoleTextAttribute(handle, white);
                std::cout << "0 ";
            }
        }
        // 한 라인(행) 출력이 마무리되면 개행 출력.
        std::cout << "\n";

    }
}
