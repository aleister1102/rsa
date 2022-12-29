#pragma once
#include "BigInt.h"
#include "BigIntConverter.h"
#include <vector>
#include <tuple>
#include <fstream>

using std::vector;
using std::tuple;
using std::fstream;
using std::ios;

class IO
{
public:
	void clearFile(string filename);

	void readInputs(vector<tuple<BigInt, BigInt>>& testCases);
	void readBinaryInputs(vector<tuple<BigInt, BigInt>>& testCases);

	void writeOutput(string output);
	void writeOutput(fstream& fs, string output);
	void writeOutputs(BigInt a, BigInt b, BigInt res, string op);
	void writeOutputs(BigInt a, int b, BigInt res, string op);
	void writeBinaryOutputs(BigInt a, BigInt b, BigInt res, string op);

	void writeLog(string log);

	bool openFile(fstream& fs, string filename, ios::openmode mode);
	string readContent(fstream& fs);
};

static IO io;