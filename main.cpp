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
	cout << BigIntIO::outputBin(a) << endl;

	//BigInt b = BigIntIO::inputBin(numberB);
	//cout << BigIntIO::outputBin(b) << endl;

	//BigInt sum = a + b;
	//cout << "Result: \n" << BigIntIO::outputBin(&sum) << "\n";

	//BigInt different = a - b;
	//cout << "Result: \n" << BigIntIO::outputBin(&different) << "\n";

	a >> 3;
	cout << BigIntIO::outputBin(a) << endl;

	a << 3;
	cout << BigIntIO::outputBin(a) << endl;

	return 0;
}