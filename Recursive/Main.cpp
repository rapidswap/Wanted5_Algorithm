#include <iostream>

// 1부터 지정한 수까지의 합을 구하는 재귀 함수.

int Sum(int number)
{
	if (number == 1)
	{
		return 1;
	}

	return number + Sum(number - 1);
}

// 거듭 제곱 재귀 함수.
int Power(int x, int y)
{
	// 종료 조건.
	if (y == 0)
	{
		return 1;
	}

	// 범위를 줄여가면서 재귀 함수 호출.
	return x * Power(x, y - 1);
}

int main()
{
	// 2의 9승 = 256
	int power = Power(2, 8);
	const int number = 5;

	
	std::cout << "Sum: "<< Sum(number)<<" Pow: "<<power;
}