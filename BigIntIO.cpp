#include "BigIntIO.h"

BigInt* BigIntIO::inputBin(string binaryString)
{
	BigInt* result;

	result = BigIntConverter::binaryStrToBigInt(binaryString);

	return result;
}
string BigIntIO::outputBin(BigInt* number)
{
	string result;

	result = BigIntConverter::bigIntToBinaryStr(number);

	return result;
}

void BigIntIO::displayInputs(BigInt a, BigInt b)
{
	cout << "Input:\n";
	cout << BigIntConverter::bigIntToBinaryStr(&a) << endl;
	cout << BigIntConverter::bigIntToBinaryStr(&b) << endl;
	cout << "\n";
}

void BigIntIO::testOperatorForByte(byte a, byte b, byte result, string op)
{
	cout << BigIntConverter::byteToString(a, true) << "\n " << op << " \n";
	cout << BigIntConverter::byteToString(b, true) << endl;
	cout << "-----------------" << endl;
	cout << BigIntConverter::byteToString(result, true) << endl;
}