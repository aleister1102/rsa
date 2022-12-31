#include "IO.h"
#include "Utils.h"
#include "Converter.h"

using std::cin;
using std::endl;
using std::ios;
using std::fstream;
using std::string;
using std::make_tuple;

void IO::readInputs(vector<tuple<BigInt, BigInt>>& testCases, int base)
{
	fstream f("input.txt", ios::in);
	string numberA, numberB;

	if (f.is_open())
	{
		while (!f.eof())
		{
			f >> numberA >> numberB;

			if (numberA == "" || numberB == "") continue;

			BigInt a, b;
			if (base == BigIntBase::BASE_10) {
				a = converter.decimalStrToBigInt(numberA);
				b = converter.decimalStrToBigInt(numberB);
			}
			else if (base == BigIntBase::BASE_2)
			{
				a = converter.binaryStrToBigInt(numberA);
				b = converter.binaryStrToBigInt(numberB);
			}

			testCases.push_back(make_tuple(a, b));
		}
	}

	f.close();
}

void IO::writeOutputs(BigInt a, BigInt b, BigInt res, string op, int base)
{
	fstream f("output.txt", ios::app);

	if (base == BigIntBase::BASE_10) {
		f << converter.bigIntToDecimalStr(a)
			<< op
			<< converter.bigIntToDecimalStr(b)
			<< " = "
			<< converter.bigIntToDecimalStr(res)
			<< endl;
	}
	else if (base == BigIntBase::BASE_2) {
		f << converter.bigIntToBinaryStr(a)
			<< op
			<< converter.bigIntToBinaryStr(b)
			<< " = "
			<< converter.bigIntToBinaryStr(res)
			<< endl;
	}

	f.close();
}

void IO::writeLog(string log)
{
	fstream f("log.txt", ios::app);

	f << log << endl;

	f.close();
}

void IO::writeLog(string prefix, BigInt n, int base)
{
	fstream f("log.txt", ios::app);

	f << prefix;

	if (base == BigIntBase::BASE_10)
		f << converter.bigIntToDecimalStr(n);
	else if (base == BigIntBase::BASE_2)
		f << converter.bigIntToBinaryStr(n);

	f << endl;
	f.close();
}

void IO::writeOutput(string output)
{
	fstream f("output.txt", ios::app);

	f << output << endl;

	f.close();
}

void IO::writeOutput(string prefix, BigInt n, int base)
{
	fstream f("output.txt", ios::app);

	f << prefix;

	if (base == BigIntBase::BASE_10)
		f << converter.bigIntToDecimalStr(n);
	else if (base == BigIntBase::BASE_2)
		f << converter.bigIntToBinaryStr(n);

	f << endl;

	f.close();
}

void IO::writeConsole(string output)
{
	std::cout << output << endl;
}

void IO::clearFile(string filename)
{
	fstream f(filename, ios::out);
	f.close();
}

bool IO::openFile(fstream& fs, string filename, ios::openmode mode)
{
	fs.open(filename, mode);

	if (!fs.is_open())
	{
		IO::writeLog("[IO::openFile] file: " + filename + " is not existed");
		return false;
	}
	else {
		return true;
	}
}

string IO::readFile(fstream& fs)
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

tuple<string, string, string> IO::insertKeys(int base) {
	string nStr, eStr, dStr;

	if (base == BigIntBase::BASE_10)
	{
		do {
			writeConsole("[Encrypt] enter decimal public key n: "); cin >> nStr;
		} while (!isValidDecimalStr(nStr));
		do {
			writeConsole("[Encrypt] enter decimal public key e: "); cin >> eStr;
		} while (!isValidDecimalStr(eStr));
		do {
			writeConsole("[Encrypt] enter decimal private key d: "); cin >> dStr;
		} while (!isValidDecimalStr(dStr));
	}
	else if (base == BigIntBase::BASE_2) {
		do {
			writeConsole("[Encrypt] enter binary public key n: "); cin >> nStr;
		} while (!isValidBinaryStr(nStr));
		do {
			writeConsole("[Encrypt] enter binary public key e: "); cin >> eStr;
		} while (!isValidBinaryStr(eStr));
		do {
			writeConsole("[Encrypt] enter binary private key d: "); cin >> dStr;
		} while (!isValidBinaryStr(dStr));
	}

	return make_tuple(nStr, eStr, dStr);
}

tuple<string, string> IO::inputFilesForEncryption()
{
	string plainTextFile, cipherTextFile;

	do {
		writeConsole("[Encrypt] enter plain text file name: "); cin >> plainTextFile;
	} while (!isFileExisted(plainTextFile));

	writeConsole("[Encrypt] enter cipher text file name: "); cin >> cipherTextFile;

	return make_tuple(plainTextFile, cipherTextFile);
}

tuple<string, string> IO::inputFilesForDecryption()
{
	string cipherTextFile, decryptedFile;

	do {
		writeConsole("[Decrypt] enter cipher text file name: "); cin >> cipherTextFile;
	} while (!isFileExisted(cipherTextFile));

	writeConsole("[Decrypt] enter decrypted plain text file name: "); cin >> decryptedFile;

	return make_tuple(cipherTextFile, decryptedFile);
}

void IO::exportKeys(RSA rsa, int exportMethod)
{
	io.writeConsole("Exporting keys...");

	fstream fs("keys.txt", ios::out);
	if (fs.is_open() == false) return;

	string keys = rsa.getKeys();

	if (exportMethod == ExportMethod::FILE)
	{
		fs << keys;
		io.writeConsole("Finish exporting keys! Please check 'keys.txt'");
	}
	else if (exportMethod == ExportMethod::CONSOLE)
	{
		io.writeConsole(keys);
		io.writeConsole("Finish exporting keys!");
	}
	else if (exportMethod == ExportMethod::BOTH)
	{
		fs << keys;
		io.writeConsole(keys);
		io.writeConsole("Finish exporting keys! Please check 'keys.txt'");
	}

	io.writeConsole("Press any key to continue.");
	system("pause>0");
}