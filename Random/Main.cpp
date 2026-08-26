#include <iostream>
#include <cstdlib>
#include <ctime>

// 범위를 지정해서 특정 범위의 난수를 반환하는 함수.
int RandomRange(int min, int max)
{
	// 나머지 연산을 활용한 방법.
	//const int diff = (max - min) + 1;
	//
	//return (rand() % diff) + min;

	// 나누기 연산을 활용한 방법.
	const int diff = (max - min) + 1;
	return ((rand() * diff) / (RAND_MAX + 1)) + min;
}

// 부동소수 난수 생성 함수.
float RandomRange(float min, float max)
{
	// 0.0 ~ 1.0 사이의 부동소수 난수 구하기.
	float percent = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	// 전달 받은 구간으로 변환.
	float diff = (max - min);
	return (percent * diff) + min;
}

int main()
{
	int64_t seed=time(nullptr);
	
	// 종자값(seed).
	// 종자값을 가변적으로 받음.
	srand(static_cast<uint32_t>(seed));
	std::cout << "time: " << seed<<"\n";
	const int count = 10;
	for (int ix = 0; ix < count;++ix)
	{
		std::cout << "Random Number: " << RandomRange(1.0f,5.0f) << "\n";
	}
}