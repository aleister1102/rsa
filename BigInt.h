#pragma once
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;
typedef unsigned char byte;

static byte ByteMask[8] = {
	0b00000001,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b10000000 };

// Lớp số nguyên lớn không dấu
class BigInt
{
public:
	byte* bytes;
	int byteCount;

public:
	BigInt()
		: bytes(nullptr), byteCount(0) {}
	BigInt(byte* bytes, int byteCount)
		: bytes(bytes), byteCount(byteCount) {}
	BigInt(const BigInt& other)
		: bytes(nullptr), byteCount(0)
	{
		*this = other;
	}
	~BigInt() { free(bytes); }

public:
	BigInt& operator=(const BigInt& other)
	{
		if (this != &other)
		{
			free(bytes);
			byteCount = other.byteCount;

			// Sao chép từng byte
			if (byteCount > 0)
			{
				bytes = (byte*)malloc(byteCount * sizeof(byte));
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
};

BigInt operator + (BigInt a, BigInt b);