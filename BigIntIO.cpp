#include "BigIntIO.h"
#include <fstream>

using std::endl;
using std::ios;
using std::fstream;
using std::make_tuple;

void BigIntIO::clearOutputs()
{
	fstream f("output.txt", ios::out);
	f.close();
}

void BigIntIO::readInputs(vector<tuple<BigInt, BigInt>>& testCases)
{
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

			BigInt a = converter.decimalStrToBigInt(numberA);
			BigInt b = converter.decimalStrToBigInt(numberB);

			testCases.push_back(make_tuple(a, b));
		}
	}
	f.close();
}

void BigIntIO::writeOutputs(BigInt a, BigInt b, BigInt res, string op)
{
	fstream f("output.txt", ios::app);

	f << converter.bigIntToDecimalStr(a)
		<< op
		<< converter.bigIntToDecimalStr(b)
		<< " = "
		<< converter.bigIntToDecimalStr(res)
		<< endl;

	f.close();
}

void BigIntIO::writeOutputs(BigInt a, int b, BigInt res, string op)
{
	fstream f("output.txt", ios::app);

	f << converter.bigIntToDecimalStr(a)
		<< op
		<< b
		<< " = "
		<< converter.bigIntToDecimalStr(res) << endl;

	f.close();
}

void BigIntIO::writeOutput(string output)
{
	fstream f("output.txt", ios::app);

	f << output << endl;

	f.close();
}