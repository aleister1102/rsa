#pragma once
#include "BigInt.h"
#include "BigIntConverter.h"
#include <vector>
#include <tuple>

using std::vector;
using std::tuple;

class BigIntIO
{
public:
	void clearFile(string filename);

	void readInputs(vector<tuple<BigInt, BigInt>>& testCases);
	void readBinaryOutputs(vector<tuple<BigInt, BigInt>>& testCases);

	void writeOutput(string output);
	void writeOutputs(BigInt a, BigInt b, BigInt res, string op);
	void writeOutputs(BigInt a, int b, BigInt res, string op);
	void writeBinaryOutputs(BigInt a, BigInt b, BigInt res, string op);

	void writeLog(string log);
};

static BigIntIO io;