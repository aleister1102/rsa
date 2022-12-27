#pragma once
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;
typedef unsigned char byte;

static byte ZERO = 0b00000000;

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

static byte HighBitByteMask[8] = {
	0b10000000,
	0b11000000,
	0b11100000,
	0b11110000,
	0b11111000,
	0b11111100,
	0b11111110,
	0b11111111,
};

static byte LowBitByteMask[8] = {
	0b00000001,
	0b00000011,
	0b00000111,
	0b00001111,
	0b00011111,
	0b00111111,
	0b01111111,
	0b11111111
};

// Lớp số nguyên lớn không dấu
class BigInt
{
public:
	byte* bytes;
	uint16_t byteCount;

public:
	BigInt()
		: bytes(nullptr), byteCount(0) {}

	BigInt(uint16_t initByteCount) {
		byteCount = initByteCount;
		auto mem = (byte*)malloc(byteCount * sizeof(byte));
		bytes = mem ? mem : nullptr;
		memset(bytes, 0, byteCount);
	}

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

			if (byteCount > 0)
			{
				bytes = (byte*)malloc(byteCount * sizeof(byte));
				if (bytes == nullptr) return *this;

				// Sao chép từng byte
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

	BigInt& operator=(int value)
	{
		free(bytes);
		byteCount = 1;
		bytes = (byte*)malloc(byteCount);
		if (bytes)
			bytes[0] = value;
	}

public:
};

BigInt operator + (BigInt a, BigInt b);
BigInt operator + (BigInt a, int value);

void operator +=(BigInt& a, BigInt b);

BigInt operator - (BigInt a, BigInt b);
BigInt operator - (BigInt a, int value);

void operator -=(BigInt& a, BigInt b);

bool operator == (BigInt a, BigInt b);
bool operator == (BigInt a, int value);

bool operator != (BigInt a, BigInt b);
bool operator != (BigInt a, int value);

BigInt operator >> (BigInt a, int steps);
void operator >>= (BigInt& a, int steps);

BigInt operator << (BigInt a, int steps);
void operator <<= (BigInt& a, int steps);

bool operator>(BigInt a, BigInt b);

BigInt operator &(BigInt a, BigInt b);

BigInt operator |(BigInt a, BigInt b);

BigInt operator *(BigInt a, BigInt b);

BigInt operator/(BigInt a, BigInt b);
