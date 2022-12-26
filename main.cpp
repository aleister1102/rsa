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

void test(BigInt a, BigInt b)
{
	BigInt res;

	// Triển khai các thao tác
	res = a + b;
	res = a - b;
#if 0
	BigInt different = a - b;
	cout << "Result: \n" << io.outputBin(different) << "\n";

	cout << io.outputBin(a) << endl;
	a << 6;
	cout << io.outputBin(a) << endl;

	BigInt product = a * b;
	cout << "Result: \n" << io.outputBin(product) << "\n";

	BigInt q = a / b;
	cout << "Result: \n" << io.outputBin(q) << "\n";
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