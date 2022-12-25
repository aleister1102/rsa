#include "BigInt.h"
#include "BigIntConverter.h"
#include "BigIntIO.h"

int getMaxByteCount(int a, int b)
{
	return a > b ? a : b;
}

byte getLastByte(const BigInt* number)
{
	return number->bytes[number->byteCount - 1];
}

void shiftOneByte(const BigInt* number)
{
	try {
		for (int i = number->byteCount - 1; i > 0; i--)
		{
			number->bytes[i] = number->bytes[i - 1];
		}
	}
	catch (exception e)
	{
		cout << e.what() << "\n";
	}
}

void addPaddingBytes(BigInt* number, int amount)
{
	number->byteCount += amount;
	number->bytes = (byte*)realloc(number->bytes, number->byteCount * sizeof(byte));

	int paddingPosition = number->byteCount - amount;
	for (int i = 0; i < amount; i++)
	{
		number->bytes[paddingPosition + i] = ByteMask[0];
	}
}

void removeLastByte(BigInt* number)
{
	byte lastByte = getLastByte(number);

	if (lastByte) return;
	else
	{
		number->byteCount -= 1;
		number->bytes = (byte*)realloc(number->bytes, number->byteCount * sizeof(byte));
	}
}

BigInt operator+(BigInt a, BigInt b)
{
	uint32_t maxByteCount = getMaxByteCount(a.byteCount, b.byteCount);

	// Trường hợp hai số có kích thước byte khác nhau
	if (a.byteCount != b.byteCount)
	{
		// Lấy số có kích thước byte ít hơn để chèn thêm các byte 0
		BigInt* lesserByteNumber = (a.byteCount > b.byteCount) ? &b : &a;
		addPaddingBytes(lesserByteNumber, abs((int)a.byteCount - (int)b.byteCount));
	}

	BigInt result(maxByteCount + 1);

	// Nếu tổng hai byte có giá trị lớn hơn 255
	// thì cần phải nhớ 1
	bool carry = a.bytes[0] + b.bytes[0] > 255 ? 1 : 0;
	result.bytes[0] = a.bytes[0] + b.bytes[0];

	for (int i = 1; i < maxByteCount; i++) {
		result.bytes[i] = a.bytes[i] + b.bytes[i] + (carry ? 1 : 0);

		// Nếu tổng hai byte và nhớ một ở byte trước cũng lớn hơn 255
		// thì nhớ 1 sang byte tiếp theo
		carry = (a.bytes[i] + b.bytes[i] + (carry ? 1 : 0)) > 255;
	}

	// Nếu sau khi tính toán mà vẫn còn carry thì cộng vào byte cuối cùng
	if (carry)
	{
		result.bytes[result.byteCount - 1] += 1;
	}
	// Trường hợp ngược lại, xóa byte cuối cùng do rỗng
	else
	{
		removeLastByte(&result);
	}

	return result;
}

BigInt operator+(BigInt a, int value)
{
	BigInt b(value);

	BigInt result(a.byteCount);

	result = a + b;

	return result;
}

void twoComplement(BigInt& number)
{
	for (int i = 0; i < number.byteCount; i++)
	{
		// Lật bit
		number.bytes[i] = ~number.bytes[i];
	}

	number = number + 1;
}

BigInt operator-(BigInt a, BigInt b)
{
	uint32_t maxByteCount = getMaxByteCount(a.byteCount, b.byteCount);

	twoComplement(b);

	// Trường hợp hai số có kích thước byte khác nhau
	if (a.byteCount != b.byteCount)
	{
		// Lấy số có kích thước byte ít hơn và cấp phát thêm vùng nhớ
		BigInt* lesserByteNumber = (a.byteCount > b.byteCount) ? &b : &a;
		addPaddingBytes(lesserByteNumber, abs((int)a.byteCount - (int)b.byteCount));
	}

	BigInt result(maxByteCount);

	result = a + b;

	return result;
}