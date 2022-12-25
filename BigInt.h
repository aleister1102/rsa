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
	~BigInt() { delete bytes; }

	BigInt& operator=(const BigInt& other) {
		if (this != &other) {
			delete bytes;
			byteCount = other.byteCount;
			isNegative = other.isNegative;

			// Sao chép từng byte
			if (byteCount > 0)
			{
				bytes = new byte[byteCount];
				for (int i = 0; i < byteCount; i++)
				{
					bytes[i] = other.bytes[i];
				}
			}
			else
				bytes = nullptr;
		}
		return *this;
	}

public:
};

class Converter {
public:
	static string reverseString(string littleEndian)
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

	static byte stringToByte(string str)
	{
		byte result = 0;

		for (char bit : str) {
			// Dịch trái sang một vị trí để có vị trí trống ở cuối
			result = result << 1;

			// Thêm bit vào cuối
			result += bit - '0';
		}

		return result;
	}

	static string byteToString(byte number)
	{
		string result;

		for (int i = 0; i < 8; i++)
		{
			// Lấy bit thứ i
			result += to_string(number & 1);

			// Dịch dãy các bits sang một bit để lấy bit tiếp theo
			number >>= 1;
		}

		return result;
	}

	static BigInt binaryStrToBigInt(string str)
	{
		int offset = 0;
		int length = str.length();
		int byteCount = length / 8 + (length % 8 != 0);

		// Khởi tạo một số BigInt mới
		BigInt result;
		result.bytes = new byte[byteCount];
		result.byteCount = byteCount;

		// Tách các octets
		for (int i = 0; i < byteCount; i++)
		{
			string byteStr;

			if (length - 8 >= 0)
				byteStr = str.substr(offset, 8);
			else
				byteStr = str.substr(offset, length);

			result.bytes[i] = Converter::stringToByte(byteStr);

			offset += 8;
			length -= 8;
		}

		return result;
	}

	static string bigIntToBinaryStr(BigInt number)
	{
		string result;

		for (int i = 0; i < number.byteCount; i++)
		{
			string str = Converter::byteToString(number.bytes[i]);
			string bigEndian = Converter::reverseString(str);
			result += bigEndian + " ";
		}

		return result;
	}
};

class IO
{
public:
	static BigInt inputBin(const char* binaryString)
	{
		BigInt result;

		result = Converter::binaryStrToBigInt(binaryString);

		return result;
	}
	static string outputBin(BigInt number)
	{
		string result;

		result = Converter::bigIntToBinaryStr(number);

		return result;
	}
};