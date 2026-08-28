#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>

// 스탯 타이머 예시.
class StatTimer
{
public:
	StatTimer(const std::string& tag)
		: tag(tag)
	{
	}

	void CheckStartStat()
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		start = counter.QuadPart;
	}

	void CheckEndStat()
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		end = counter.QuadPart;
	}

	void Reset()
	{
		start = end = 0;
	}

	void PrintResult()
	{
		std::cout << "[" << tag << "] elapsed: " << GetElapsed() << "\n";
	}

private:
	float GetElapsed()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return static_cast<float>(end - start)
			/ static_cast<float>(frequency.QuadPart);
	}

private:
	std::string tag;
	int64_t start = 0;
	int64_t end = 0;
};

typedef bool (*Comparer)(int, int);
//using Comparer = bool(*)(int, int);

bool LessEqual(int a, int b) { return a <= b; }
bool GreaterEqual(int a, int b) { return a >= b; }

// 분할된 배열을 병합하면서 정렬하는 함수.
void Merge(int* array, int* leftArray, int leftArrayLength, int* rightArray, int rightArrayLength,Comparer comparer=LessEqual)
{
	// 병합처리를 위한 인덱스.
	int leftIndex = 0;
	int rightIndex = 0;
	int mergedIndex = 0;

	// 오름차순으로 정렬로 구현.
	// 두 배열의 각각의 현재 인덱스의 값을 비교해 작은 값을 array에 저장.
	while (leftIndex < leftArrayLength && rightIndex < rightArrayLength)
	{
		// 왼쪽 인덱스 기준으로 처리(왼쪽 배열의 값이 더 작은 경우).
		if (comparer(leftArray[leftIndex] , rightArray[rightIndex]))
		{
			array[mergedIndex] = leftArray[leftIndex];
			++leftIndex;
		}
		// 오른쪽 배열의 값이 더 작은 경우.
		else
		{
			array[mergedIndex] = rightArray[rightIndex];
			++rightIndex;
		}
		// 병합된 배열의 인덱스 증가 처리.
		++mergedIndex;


	}

	// 왼쪽 배열에서 남은 원소를 복사.
	while (leftIndex < leftArrayLength)
	{
		array[mergedIndex] = leftArray[leftIndex];
		++leftIndex;
		++mergedIndex;
	}

	// 오른쪽 배열에서 남은 원소를 복사.
	while (rightIndex < rightArrayLength)
	{
		array[mergedIndex] = rightArray[rightIndex];
		++rightIndex;
		++mergedIndex;
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

// 병합 정렬 함수.
void MergeSort(int* array, int length,Comparer comparer = LessEqual)
{
	// 종료 조건.
	if (length <= 1)
	{
		return;
	}

	// 반으로 나누면서 분할 진행.
	// 중간 인덱스.
	int middle = length / 2;

	// 왼쪽으로 나눌 배열의 크기.
	int leftArrayLength = middle;
	// 오른쪽으로 나눌 배열의 크기(전체 크기에서 왼쪽 배열 크기를 뺌).
	int rightArrayLength = length - middle;

	// 분할된 배열 선언.
	int* leftArray = new int[leftArrayLength] {};
	int* rightArray = new int[rightArrayLength] {};

	
	// 분할된 배열에 값 복사.
	memcpy(leftArray, array, sizeof(int) * leftArrayLength);
	memcpy(rightArray, array + middle, sizeof(int) * rightArrayLength);
	
	// 분할을 이어서 진행(재귀)
	MergeSort(leftArray, leftArrayLength,comparer);
	MergeSort(rightArray, rightArrayLength, comparer);

	// 분할이 재귀적으로 모두 끝나면 병합을 진행.
	Merge(array, leftArray, leftArrayLength, rightArray, rightArrayLength, comparer);

	// 사용한 배열 제거.
	delete[] leftArray;
	leftArray = nullptr;

	delete[] rightArray;
	rightArray = nullptr;
	
	
}

int64_t GetTime()
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return counter.QuadPart;	
}

// 시작/종료 시간을 받아 초단위의 경과 시간 반환 함수.
float GetElapsedTime(int64_t start, int64_t end)
{
	// 시간 범위 단위(주파수-정밀도).
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	return static_cast<float>(end - start) / static_cast<float>(frequency.QuadPart);
}

int main()
{
	// 배열.
	const int length = 100000;

	// 정렬할 배열 선언.
	int* original = new int[length] {};

	// 랜덤을 활용해서 배열에 값 저장.
	for (int ix = 0;ix < length;++ix)
	{
		original[ix] = RandomRange(1, length);
	}

	// 정렬에 사용할 배열.
	int* array = new int[length] {};
	memcpy(array, original, length * sizeof(int));

	// 경과시간 확인을 위한 스탯 객체.
	//int64_t start = GetTime();
	StatTimer mergeSortTimer("MergeSort");

	// 시작 시간 기록.
	mergeSortTimer.CheckStartStat();


	// 정렬.
	MergeSort(array, length);

	mergeSortTimer.CheckEndStat();
	mergeSortTimer.PrintResult();
	//int64_t end = GetTime();
	//std::cout << "Elpased Time: " << GetElapsedTime(start, end) << "\n";

	// 출력.

	auto printArray =
		[](int* array, int length)
		{
			for (int ix = 0;ix < length;++ix)
			{
				std::cout << array[ix];
				if (ix < length - 1)
				{
					std::cout << ", ";
				}
			}
			std::cout << "\n";
		};
	printArray(array,length);
	

	delete[] original;
	original = nullptr;

	delete[] array;
	array = nullptr;
}