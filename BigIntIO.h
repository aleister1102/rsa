#pragma once
#include "BigInt.h"
#include "BigIntConverter.h"

#include <vector>
#include <tuple>

using namespace std;

class BigIntIO
{
public:
	void clearOutputs();

	void readInputs(vector<tuple<BigInt, BigInt>>& testCases);

	void writeOutputs(BigInt a, BigInt b, BigInt res, string op);
	void writeOutputs(BigInt a, int b, BigInt res, string op);
	void writeOuput(string output);
};

static BigIntIO io;