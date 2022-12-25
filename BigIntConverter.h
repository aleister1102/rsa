#pragma once
#include "BigInt.h"
using namespace std;

class BigIntConverter;
class BigIntConverter
{
public:
	static string reverseString(string littleEndian);
	static string removeSpaces(string str);
	static byte stringToByte(string str);
	static string byteToString(byte number, bool isReversed);
	static BigInt* binaryStrToBigInt(string str);
	static string bigIntToBinaryStr(BigInt* number);
};
