#pragma once
#include <cstdlib>
#include <iostream>
#include <string>

typedef unsigned char byte;

static byte zero;

static byte ByteMask[8] = {
	byte(0b00000001),
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
	uint32_t byteCount;

public:
	BigInt() : bytes(nullptr), byteCount(0) {}

	BigInt(uint32_t initByteCount) {
		byteCount = initByteCount;
		bytes = (byte*)malloc(byteCount * sizeof(byte));
		memset(bytes, 0, byteCount * sizeof(byte));
	}

	// TODO: giảm số lần gọi constructor nhiều nhất có thể
	BigInt(const BigInt& other) : bytes(nullptr), byteCount(0)
	{
		*this = other;
	}

	BigInt(int value) : bytes(nullptr), byteCount(0)
	{
		*this = value;
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
				memcpy(bytes, other.bytes, byteCount);
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
		bytes = (byte*)malloc(byteCount * sizeof(byte));
		if (bytes)
			bytes[0] = value;
		return *this;
	}

public:
	int getIntValue();
	bool isPositive();
	bool isNegative();
	bool isOdd();
	bool isEven();
	bool isZero();
};

byte getLastByte(const BigInt& n);

void removeLastBytesIfNull(BigInt& n, int preserve = 1);

void addMoreBytes(BigInt& n, int amount);

BigInt abs(BigInt n);

void division(BigInt a, BigInt b, BigInt& q, BigInt& r);

BigInt twoComplement(BigInt n);

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

BigInt operator &(BigInt a, BigInt b);

BigInt operator |(BigInt a, BigInt b);

// Chỉ xét trong phạm vi số byte lớn nhất của 2 số
// Ví dụ với số 1 byte thì 0111 1111 sẽ là số dương
// còn 1000 0000 sẽ là số âm
bool operator < (BigInt a, BigInt b);
bool operator <= (BigInt a, BigInt b);

bool operator > (BigInt a, BigInt b);
bool operator >= (BigInt a, BigInt b);

BigInt operator *(BigInt a, BigInt b);

BigInt operator~(BigInt a);

BigInt operator/(BigInt a, BigInt b);
BigInt operator%(BigInt a, BigInt b);
