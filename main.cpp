#include <fstream>
#include "BigInt.h"
#include "BigIntIO.h"
using namespace std;

int main()
{
	fstream f("input.txt", ios::in);
	string numberA, numberB;
	if (f.is_open())
	{
		f >> numberA;
		f >> numberB;
	}
	f.close();

	BigInt a = BigIntIO::inputBin(numberA);
	BigInt b = BigIntIO::inputBin(numberB);

#if 0
	BigInt different = a - b;
	cout << "Result: \n" << BigIntIO::outputBin(different) << "\n";

	BigInt sum = a + b;
	cout << "Result: \n" << BigIntIO::outputBin(sum) << "\n";

	cout << BigIntIO::outputBin(a) << endl;
	a << 6;
	cout << BigIntIO::outputBin(a) << endl;

#endif
	BigInt product = a * b;
	cout << "Result: \n" << BigIntIO::outputBin(product) << "\n";

	return 0;
}