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

	BigInt* a = BigIntIO::inputBin(numberA);
	//cout << BigIntIO::outputBin(a) << endl;

	BigInt* b = BigIntIO::inputBin(numberB);
	//cout << BigIntIO::outputBin(b) << endl;

	BigInt sum = *a + *b;
	cout << BigIntIO::outputBin(&sum) << endl;

	return 0;
}