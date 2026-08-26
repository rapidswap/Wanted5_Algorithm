#include <iostream>

// 재귀를 활용한 이진 탐색.
int BinarySearchRecursive(const int* array, const int target, int start, int end)
{
	// 종료 조건.
	// 시작 인덱스가 종료 인덱스보다 커지면 검색 실패.
	if (start > end)
	{
		return -1;
	}
	// 중간 인덱스.
	int middle =(start+end) / 2;

	// 찾았는지 확인.
	if (array[middle] == target)
	{
		return middle;
	}

	// 배열의 값이 검색하려는 값보다 큰 경우.
	if (array[middle] > target)
	{
		return BinarySearchRecursive(array, target, start, middle-1);
	}

	// 중간 값보다 크면 오른쪽 범위를 탐색.
	return BinarySearchRecursive(array, target, middle+1, end);
}

// 반복적인(반복문) 방법으로 이진 탐색을 처리하는 함수.
int BinarySearchInterative(const int* array, const int length, const int target)
{
	int start = 0;
	int end = length-1;

	while (start < end)
	{
		// 중간 인덱스.
		int middle = (start + end) / 2;
		if (array[middle] == target)
		{
			return middle;
		}

		if (array[middle] < target)
		{
			start = middle + 1;
		}
		else
		{
			end = middle - 1;
		}
	}
	return -1;
}

int main()
{
	const int array[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const int target = 10;
	const int length = sizeof(array) / sizeof(array[0]);

	// 인덱스 검색.
	//int result = BinarySearchRecursive(array, target, 0, length - 1);
	int result = BinarySearchInterative(array, length, target);

	if (result != -1)
	{
		std::cout << "검색 성공. Index: " << result << "\n";
	}
	else
	{
		std::cout << "검색 실패.\n";
	}

}