#pragma once
#include "BigInt.h"
#include <vector>
#include <fstream>
#include <tuple>

using std::string;
using std::vector;
using std::tuple;
using std::fstream;
using std::ios;

class IO
{
public:
	void readInputs(vector<tuple<BigInt, BigInt>>& testCases, int base);
	void writeOutputs(BigInt a, BigInt b, BigInt res, string op, int base);

	void writeLog(string log);
	void writeLog(string prefix, BigInt n, int base);
	void writeOutput(string output);
	void writeOutput(string prefix, BigInt n, int base);
	void writeConsole(string output);

	void clearFile(string filename);
	bool openFile(fstream& fs, string filename, ios::openmode mode);
	string readFile(fstream& fs);

public:
	tuple<string, string, string> inputKeys(int base);
	tuple<string, string> inputFilesForEncryption();
	tuple<string, string> inputFilesForDecryption();
};

static IO io;
