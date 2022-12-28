#include <fstream>
#include "BigInt.h"
#include "BigIntIO.h"
#include "vector"
#include "tuple"
using namespace std;

vector<tuple<BigInt, BigInt>> testCases;

void operatorTest(BigInt a, BigInt b)
{
	a / b;
#if 0
	a% b;
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

void ioTest(BigInt a)
{
	string decStr = converter.bigIntToDecimalStr(a);
	BigInt n = converter.decimalStrToBigInt(decStr);
	string binStr = converter.bigIntToBinaryStr(n);

	io.writeOuput(binStr);
}

void test()
{
	io.readInputs(testCases);
	io.clearOutputs();

	for (auto testCase : testCases)
	{
		BigInt a, b;
		tie(a, b) = testCase;

		//operatorTest(a, b);
		//ioTest(a);
		ioTest(b);
	}
}

int main()
{
	test();

	return 0;
}