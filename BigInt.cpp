#include "BigInt.h"
#include "BigIntConverter.h"
#include "BigIntIO.h"

int maxByte(int a, int b)
{
	return a > b ? a : b;
}

byte getLastByte(BigInt number)
{
	return number.bytes[number.byteCount - 1];
}

bool getFirstBit(BigInt number)
{
	byte lastByte = getLastByte(number);
	return lastByte & ByteMask[7];
}

void shiftByte(BigInt number)
{
	for (int i = 0; i < number.byteCount; i++)
	{
	}
}

void shiftOneByte(BigInt* number)
{
	try {
		for (int i = number->byteCount - 1; i > 0; i--)
		{
			number->bytes[i] = number->bytes[i - 1];
		}
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}
}

void addByte(BigInt* number, byte newByte)
{
	number->byteCount++;
	number->bytes = (byte*)realloc(number->bytes, number->byteCount);
	number->bytes[number->byteCount - 1] = newByte;
}

BigInt operator+(BigInt a, BigInt b)
{
	int maxByteCount = maxByte(a.byteCount, b.byteCount);

	// Trường hợp hai số có kích thước byte khác nhau
	if (a.byteCount != b.byteCount)
	{
		// Lấy số có kích thước byte ít hơn và cấp phát thêm vùng nhớ
		BigInt* lesserByteNumber = (a.byteCount > b.byteCount) ? &b : &a;
		addByte(lesserByteNumber, 0b00000000);
	}

	// Nếu cả hai số đều có bit đầu là 1 thì tăng số byte của kết quả lên 1
	int resultByteCount = maxByteCount + (getFirstBit(a) || getFirstBit(b));

	BigInt result;
	result.byteCount = resultByteCount;
	result.bytes = (byte*)malloc(result.byteCount * sizeof(byte));
	memset(result.bytes, 0, result.byteCount);

	// Nếu tổng hai byte có giá trị lớn hơn 255
	// thì cần phải nhớ 1
	bool carry = a.bytes[0] + b.bytes[0] > 255;
	result.bytes[0] = a.bytes[0] + b.bytes[0];

	for (int i = 1; i < maxByteCount; i++) {
		result.bytes[i] = a.bytes[i] + b.bytes[i] + (carry ? 1 : 0);

		// Nếu tổng hai byte và nhớ một ở byte trước cũng lớn hơn 255
		// thì nhớ 1 sang byte tiếp theo
		carry = (a.bytes[i] + b.bytes[i] + (carry ? 1 : 0)) > 255;
	}

	if (carry)
	{
		result.bytes[result.byteCount - 1] += 1;
	}

	return result;
}