#include <fstream>
#include "BigInt.h"
#include "BigIntIO.h"
#include "vector"
#include "tuple"
using namespace std;

vector<tuple<BigInt, BigInt>> testCases;

// Đọc dữ liệu từ file
void readInputs() {
	fstream f("input.txt", ios::in);
	string numberA, numberB;

	if (f.is_open())
	{
		while (!f.eof())
		{
			f >> numberA;
			f >> numberB;

			if (numberA == "" || numberB == "")
				continue;

			BigInt a = io.inputBin(numberA);
			BigInt b = io.inputBin(numberB);

			testCases.push_back(make_tuple(a, b));
		}
	}
	f.close();
}

void clearOutputs()
{
	fstream f("output.txt", ios::out);
	f.close();
}

// Triển khai các thao tác
void test(BigInt a, BigInt b)
{
	a% b;
#if 0
	a / b;
	a* b;
	a < b;
	a > b;
	a& b;
	a | b;
	a - b;
	a + b;
	a == b;
	a != b;
	a << 5;
	a >> 10;
#endif
}

int main()
{
	readInputs();
	clearOutputs();

	for (auto testCase : testCases)
	{
		BigInt a, b;
		tie(a, b) = testCase;

		test(a, b);
	}

	return 0;
}