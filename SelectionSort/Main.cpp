#include <iostream>

// 교환 함수.
void Swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

// 로직 주입을 위한 함수 포인터 타입 선언.
using Comparer = bool(*)(int, int);

// 비교 함수.
bool Less(int a, int b) { return a < b; }
// bool Greater(int a, int b) { return a > b; }
// 함수 객체(Functor)
template<typename T>
struct Greater
{
	// 호출 연산자 오버로딩.
	bool operator()(const T& a, const T& b) { return a > b; }
};

// 선택 정렬.
void SelectionSort(int* array, const int length, Comparer comparer = Less)
{

	if (length <= 1)
	{
		return;
	}

	for (int ix = 0;ix < length-1;++ix)
	{
		int valueIndex = ix;

		for (int jx = ix + 1;jx < length;++jx)
		{
			//if (array[valueIndex] > array[jx])
			if(comparer(array[valueIndex],array[jx]))
			{
				valueIndex = jx;
			}
		}
		if (ix != valueIndex)
		{
			Swap(array[ix], array[valueIndex]);
		}
	}
}

void PrintArray(int* array, const int length)
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
}

int main()
{
	// 자료 집합.
	int array[] = { 5, 2, 8, 4, 1, 7, 3, 6, 9, 10, 15, 13, 14, 12, 17, 16 };
	const int length = _countof(array);

	// 정렬 전 출력.
	PrintArray(array, length);

	// 정렬 후 출력.
	//SelectionSort(array, length,Less);
	SelectionSort(array, length,
		[](int a, int b)
		{
			return a > b;
		});
	PrintArray(array, length);

}