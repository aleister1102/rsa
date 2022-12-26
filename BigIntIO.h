#pragma once
#include "BigInt.h"
#include "BigIntConverter.h"

class BigIntIO
{
public:
	BigInt inputBin(string binaryString);
	string outputBin(BigInt number);
	void displayInputs(BigInt a, BigInt b, string op);
	void writeOutputs(BigInt a, BigInt b, BigInt result, string op);
	void writeOutputs(BigInt a, int b, BigInt result, string op);
};

static BigIntIO io;