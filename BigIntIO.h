#pragma once
#include "BigInt.h"
#include "BigIntConverter.h"

class BigIntIO
{
public:
	static BigInt* inputBin(string binaryString);
	static string outputBin(BigInt* number);
	static void displayInputs(BigInt a, BigInt b);
	static void testOperatorForByte(byte a, byte b, byte result, string op);
};
