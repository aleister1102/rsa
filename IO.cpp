#include "IO.h"
#include <fstream>
#include <string>

using std::endl;
using std::ios;
using std::fstream;
using std::make_tuple;
using std::cin;

void IO::clearFile(string filename)
{
	fstream f(filename, ios::out);
	f.close();
}

void IO::readInputs(vector<tuple<BigInt, BigInt>>& testCases)
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

void IO::readBinaryInputs(vector<tuple<BigInt, BigInt>>& testCases)
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

void IO::writeOutput(string output)
{
	fstream f("output.txt", ios::app);

	f << output << endl;

	f.close();
}

void IO::writeOutput(fstream& fs, string output)
{
	fs << output;
}

void IO::writeOutputs(BigInt a, BigInt b, BigInt res, string op)
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

void IO::writeOutputs(BigInt a, int b, BigInt res, string op)
{
	fstream f("output.txt", ios::app);

	f << converter.bigIntToDecimalStr(a)
		<< op
		<< b
		<< " = "
		<< converter.bigIntToDecimalStr(res) << endl;

	f.close();
}

void IO::writeBinaryOutputs(BigInt a, BigInt b, BigInt res, string op)
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

void IO::writeLog(string log)
{
	fstream f("log.txt", ios::app);

	f << log << endl;

	f.close();
}

bool IO::openFile(fstream& fs, string filename, ios::openmode mode)
{
	fs.open(filename, mode);

	if (!fs.is_open())
	{
		io.writeOutput("[IO::openFileForRead] file: " + filename + " is not existed");
		return false;
	}
	else {
		io.writeOutput("[IO::openFileForRead] file: " + filename + " existed");
		return true;
	}
}

string IO::readContent(fstream& fs)
{
	string content;

	while (!fs.eof())
	{
		string buffer;
		getline(fs, buffer, '\n');
		content += buffer;
	}

	return content;
}