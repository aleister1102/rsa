#include "BigIntIO.h"
#include <fstream>

BigInt BigIntIO::inputBin(string binaryString)
{
	BigInt result;

	result = converter.binaryStrToBigInt(binaryString);

	return result;
}
string BigIntIO::outputBin(BigInt number)
{
	string result;

	result = converter.bigIntToBinaryStr(number);

	return result;
}

void BigIntIO::displayInputs(BigInt a, BigInt b, string op)
{
	cout << "Input for " << op << "\n";
	cout << converter.bigIntToBinaryStr(a) << endl;
	cout << converter.bigIntToBinaryStr(b) << endl;
	cout << "\n";
}

void BigIntIO::writeOutputs(BigInt a, BigInt b, BigInt result, string op)
{
	fstream f("output.txt", ios::app);
	f << io.outputBin(a) << op << io.outputBin(b) << " = " << io.outputBin(result) << endl;
	f.close();
}

void BigIntIO::writeOutputs(BigInt a, int b, BigInt result, string op)
{
	fstream f("output.txt", ios::app);
	f << io.outputBin(a) << op << b << " = " << io.outputBin(result) << endl;
	f.close();
}