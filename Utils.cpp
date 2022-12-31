#include "Utils.h"
#include "IO.h"

using std::ios;
using std::string;
using std::fstream;

bool isValidBinaryStr(string binStr)
{
	for (char c : binStr)
	{
		if (c - '0' > 1)
			return false;
	}

	return true;
}

bool isValidDecimalStr(string decStr)
{
	for (char c : decStr)
	{
		if (!isdigit(c))
			return false;
	}

	return true;
}

bool isFileExisted(string filename)
{
	fstream fs(filename, ios::in);
	bool isExisted = fs.is_open();
	if (!isExisted) io.writeConsole("File '" + filename + "' is not existed");
	return isExisted;
}

string reverseString(string str)
{
	reverse(str.begin(), str.end());
	return str;
}

string removeSpaces(string str)
{
	string res;

	for (auto c : str)
	{
		if (c != ' ')
			res += c;
	}

	return res;
}