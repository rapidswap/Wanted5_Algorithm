#include <iostream>
#include <cstring>

// 문자열에서 특정 패턴 검색하는 함수.
int FindPatternIndex(const char* text, const char* pattern)
{
	const int textLength = static_cast<int>(strlen(text));
	const int patternLength = static_cast<int>(strlen(pattern));

	// 문자열에서 패턴 찾기.
	for (int ix = 0;ix <= textLength - patternLength;++ix)
	{
		// 검색됐을 때 반환할 패턴 인덱스.
		int patternIndex = 0;

		while (patternIndex < patternLength && text[ix + patternIndex] == pattern[patternIndex])
		{
			++patternIndex;
		}

		if (patternIndex == patternLength)
		{
			return ix;
		}

	}
	return -1;
}


// 최대값 찾기.
int FindMaxValue(const int array[], const int length)
{
	// 최대값 저장할 변수.
	int maxValue = array[0];

	// 배열 순회하면서 저장된 값보다 크면 교체.
	for (int ix = 1;ix < length;++ix)
	{
		if (maxValue < array[ix])
		{
			maxValue = array[ix];
		}
	}
	return maxValue;
}

// 배열에서 원하는 값 찾아서 인덱스 반환하는 함수.
// 배열이 함수로 전달되면 배열이 아니라 포인터(주소).
int FindValueIndex(const int array[], const int length, const int target)
{
	// 배열을 순회하면서 값 찾기.
	for (int ix = 0;ix < length;++ix)
	{
		// 배열의 우너솔가 찾느 값인지 확인.
		if (array[ix] == target)
		{
			return ix;
		}
	}
	// 배열을 모두 찾아봤는데 검색에 실패했으면 -1 반환.
	return -1;
}


int main()
{
	// 배열.
	const int array[] = { 1,3,4,5,7,9,11,13,15 };
	const int length = sizeof(array) / sizeof(array[0]);
	const int target = 11;

	int result = FindValueIndex(array, length, target);
	
	if (result != -1)
	{
		std::cout << "검색 성공, 인덱스: " << result << "\n";
	}
	else
	{
		std::cout << "검색 실패.\n";
	}

	// 최대값 검색.
	int maxValue = FindMaxValue(array, length);

	std::cout << "MaxValud: " << maxValue << "\n";

	int patternIndex = FindPatternIndex("kanghyeongjin", "nag");
	if (patternIndex != -1)
	{
		std::cout << "패턴 인덱스: " << patternIndex << "\n";
	}
	else
	{
		std::cout << "패턴 인덱스가 없습니다.\n";
	}

}