#include <iostream>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <Windows.h>
#include <algorithm>

// 배열의 원소 개수를 구하는 매크로.
#define ArraySize(array) (sizeof(array)/sizeof(array[0]))

// 현재 노드를 기준(루트)로 서브 트리를 최대(또는 최소)힙으로 만드는 함수.
void Heapify(int* array, int length, int index, std::function<bool(int, int)> comparer)
{
	while (true)
	{
		// 가장 큰(또는 작은) 값을 가진 인덱스.
		int selected = index;

		// 왼쪽 자손 인덱스.
		int leftChild = index * 2 + 1;

		// 오른쪽 자손 인덱스.
		int rightChild = index * 2 + 2;

		// 왼쪽 자손이 현재 노드 보다 크면(또는 작으면) 인덱스 교환.
		//if (leftChild<length && array[leftChild]>array[selected])
		if (leftChild<length && comparer(array[leftChild],array[selected]))
		{
			selected = leftChild;
		}

		// 오른쪽 자손이 현재 노드 보다 크면(도는 작으면) 인덱스 교환.
		//if (rightChild<length && array[rightChild]>array[selected])
		if (rightChild<length && comparer(array[rightChild],array[selected]))
		{
			selected = rightChild;
		}

		// 현재 노드가 컸다면, 합이 유지됐기 떄문에 종료.
		if (selected == index)
		{
			break;
		}
		
		// 값 교환.
		std::swap(array[index], array[selected]);

		// 값을 교환한 위치에서 다시 자손 노드들과 비교.
		index = selected;
	}
} 

// 현재 시간 가져오기.
int64_t GetTime()
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return counter.QuadPart;
}

// 경과 시간 계산 함수.
float GetElapsed(int64_t start, int64_t end)
{
	// 해상도(주파수).
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	return static_cast<float>(end - start) / frequency.QuadPart;
}

void HeapSort(int* array, int length,std::function<bool(int,int)> comparer=std::greater<int>())
{



	// 힙 구성.
	const int lastParentIndex = length / 2 - 1;
	for (int ix = lastParentIndex; ix >= 0;--ix)
	{
		Heapify(array, length, ix,comparer);
	}

	// 루트의 최대 값을 배열의 뒤쪽부터 정렬.
	for (int ix = length - 1;ix > 0;--ix)
	{
		// 루트 (0번 인덱스)와 정렬되지 않은 범위의 마지막 요소 교환.
		std::swap(array[0], array[ix]);

		// 교환을 한 뒤로는 다시 힙으로 만들어야 함.
		Heapify(array, ix, 0, comparer);
	}
}

// 랜덤에 시드 값 적용하는 함수.
void SRand()
{
	srand(static_cast<uint32_t>(time(nullptr)));
}

// min에서 max 사이의 정수를 반환하는 랜덤 함수.
int RandomRange(int min, int max)
{
	int diff = (max - min) + 1;
	return ((diff * rand()) / (RAND_MAX + 1)) + min;
}

void PrintArray(int* array, int length)
{
	for (int ix = 0;ix < length;++ix)
	{
		std::cout << array[ix];

		if (ix < length-1)
		{
			std::cout << ", ";
		}
	}
	std::cout << "\n";
}


int main()
{
	//int array[] = { 5, 2, 8, 4, 1, 7, 3, 6,	9, 10, 15, 13, 14, 12, 17, 16 };

	// 배열 원소 개수.
	//const int length = ArraySize(array);

	SRand();

	const int length = 10000000;
	int* array = new int[length]();
	int* array2 = new int[length]();
	memcpy(array2, array, sizeof(int) * length);

	for (int ix = 0;ix < length;++ix)
	{
		array[ix] = RandomRange(1,60000);
	}

	int64_t start = GetTime();

	HeapSort(array, length);

	int64_t end = GetTime();
	
	std::cout << "경과 시간: " << GetElapsed(start, end) << "\n";

	start = GetTime();
	std::sort(array2, array2 + length);
	end = GetTime();
	std::cout << "경과 시간: " << GetElapsed(start, end) << "\n";


	//PrintArray(array, length);
	delete[] array;
	array = nullptr;
	std::cin.get();
}