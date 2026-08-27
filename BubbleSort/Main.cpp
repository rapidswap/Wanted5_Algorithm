#include <iostream>
#include <functional>

// 교환 함수.
//void swap(int& a, int& b)
//{
//	int temp=a;
//	a = b;
//	b = temp;
//}

// 10^20^20 = 10
// a = 10 ^ 20
// b = 20 ^ 10 ^ 20
// b = 10
// a = 10 ^ 20 ^ 10
// a = 20
// 임시 변수 없이 교환 함수.
void swap(int& a, int& b)
{
	a = a ^ b;
	b = b ^ a;
	a = a ^ b;
}

// 로직(기능)을 주입할 때 함수 포인터 활용.
using Comparer = bool(*)(int, int);
//typedef bool(*Comparer)(int, int);

// 비교 함수.
// 내림차순.
bool Less(int a, int b)
{
	return a < b;
}

// 오름차순.
bool Greater(int a, int b)
{
	return a > b;
}

// 버블 정렬.
//void BubbleSort(int* array, const int length,Comparer comparer= Greater)
void BubbleSort(int* array, const int length,std::function<bool(int,int)> comparer = Greater)
{
	// 예외 처리.
	if (length <= 1)
	{
		return;
	}

	// 이중 루프를 활용해서 정렬 처리.
	for (int ix = 0;ix < length - 1;++ix)
	{
		for (int jx = 0;jx < length - 1 - ix;++jx)
		{
			// 두 개씩 비교.
			// 오름차순.
			//if (array[jx] > array[jx + 1])
			if(comparer(array[jx],array[jx+1]))
			{
				// 교환.
				swap(array[jx], array[jx + 1]);
			}
		}
	}
}

// 배열 출력  함수.
void PrintArray(int* array, int length)
{
	for (int ix = 0;ix < length;++ix)
	{
		std::cout << array[ix];
		//마지막이 아니면 콤마 출력.
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

	// 데이터 수.
	//const int length = sizeof(array) / sizeof(array[0]);
	const int length = _countof(array);

	// 정렬 전 출력.
	std::cout << "정렬 전 배열: ";
	PrintArray(array, length);

	// 정렬 처리.
	std::cout << "정렬 후 배열: ";
	BubbleSort(array, length,
		[](int a, int b) /* -> bool*/
		{
			return a < b;
		});


	PrintArray(array,length);

}