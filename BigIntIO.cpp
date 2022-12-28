#include "BigIntIO.h"
#include <fstream>

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

			BigInt a = converter.binaryStrToBigInt(numberA);
			BigInt b = converter.binaryStrToBigInt(numberB);

			testCases.push_back(make_tuple(a, b));
		}
	}
	f.close();
}

void BigIntIO::writeOutputs(BigInt a, BigInt b, BigInt res, string op)
{
	fstream f("output.txt", ios::app);

	f << converter.bigIntToBinaryStr(a)
		<< op
		<< converter.bigIntToBinaryStr(b)
		<< " = "
		<< converter.bigIntToBinaryStr(res)
		<< endl;

	f.close();
}

void BigIntIO::writeOutputs(BigInt a, int b, BigInt res, string op)
{
	fstream f("output.txt", ios::app);

	f << converter.bigIntToBinaryStr(a)
		<< op
		<< b
		<< " = "
		<< converter.bigIntToBinaryStr(res) << endl;

	f.close();
}

void BigIntIO::writeOuput(string output)
{
	fstream f("output.txt", ios::app);

	f << output << endl;

	f.close();
}