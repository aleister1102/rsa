#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

typedef unsigned short byte;

static byte ByteMask[8] = {
	0b00000001,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b10000000
};

class BigInt
{
public:
	byte* bytes;
	int byteCount;
	bool isNegative;

public:
	BigInt()
		: bytes(nullptr), byteCount(0), isNegative(false) {}
	BigInt(int value);
	BigInt(byte* bytes, int byteCount, bool isNegative)
		:bytes(bytes), byteCount(byteCount), isNegative(false) {}
	BigInt(const BigInt& other)
		: bytes(nullptr), byteCount(0), isNegative(false)
	{
		*this = other;
	}
	~BigInt() { free(bytes); }

public:
};

class Converter {
public:
	static byte stringToByte(string byteString) {
		byte result = 0;

		for (char bit : byteString) {
			// Dịch trái sang một vị trí để có vị trí trống ở cuối
			result = result << 1;

			// Thêm bit vào cuối
			result += bit - '0';
		}

		return result;
	}

	static string byteToString(byte byteNumber)
	{
		string byteString;

		for (int i = 0; i < 8; i++)
		{
			// Lấy bit thứ i
			byteString += to_string(byteNumber & 1);

			// Dịch dãy các bits sang một bit để lấy bit tiếp theo
			byteNumber >>= 1;
		}

		return byteString;
	}

	static string toBigEndian(string littleEndian)
	{
		reverse(littleEndian.begin(), littleEndian.end());
		return littleEndian;
	}

	static string removeSpaces(string str)
	{
		string result;

		for (auto c : str)
		{
			if (c != ' ')
				result += c;
		}

		return result;
	}

	static byte* binStringToBytes(string bytesString, int& byteCount)
	{
		int offset = 0, k = 0;
		int binStrLength = bytesString.length();

		byteCount = binStrLength / 8 + (binStrLength % 8 != 0);
		byte* bytes = new byte[byteCount];

		do {
			string byteString;

			if (binStrLength - 8 >= 0)
				byteString = bytesString.substr(offset, 8);
			else
				byteString = bytesString.substr(offset, binStrLength);

			byte byteNumber = Converter::stringToByte(byteString);
			bytes[k] = byteNumber;

			offset += 8;
			k += 1;
		} while (offset <= binStrLength);

		return bytes;
	}

	static string bytesToBinString(byte* byte, int byteCount)
	{
		string binString;

		for (int i = 0; i < byteCount; i++)
		{
			string byteString = Converter::byteToString(byte[i]);
			binString += byteString + " ";
		}

		return binString;
	}
};

class IO
{
public:
	static byte* inputBin(int& byteCount)
	{
		string binString;
		getline(cin, binString);
		binString = Converter::removeSpaces(binString);

		byte* bytes = Converter::binStringToBytes(binString, byteCount);
		return bytes;
	}
	static void outputBin(byte* bytes, int byteCount)
	{
		string binString = Converter::bytesToBinString(bytes, byteCount);
		cout << binString << endl;
	}
};