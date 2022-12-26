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

byte getLastBit(const BigInt* number)
{
	byte lastByte = getLastByte(number);
	return lastByte & ByteMask[7];
}

byte getFirstBit(const BigInt* number)
{
	byte firstByte = number->bytes[0];
	return firstByte & ByteMask[0];
}

void addPaddingBytes(BigInt* number, int amount)
{
	number->byteCount += amount;
	number->bytes = (byte*)realloc(number->bytes, number->byteCount * sizeof(byte));

	int paddingPosition = number->byteCount - amount;
	for (int i = 0; i < amount; i++)
	{
		number->bytes[paddingPosition + i] = zero;
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

void operator+=(BigInt& a, BigInt b)
{
	a = a + b;
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

	//BigIntIO::displayInputs(a, b, "-");

	// Trường hợp hai số có kích thước byte khác nhau
	if (a.byteCount != b.byteCount)
	{
		// Lấy số có kích thước byte ít hơn và cấp phát thêm vùng nhớ
		BigInt* lesserByteNumber = (a.byteCount > b.byteCount) ? &b : &a;
		addPaddingBytes(lesserByteNumber, abs((int)a.byteCount - (int)b.byteCount));
	}

	BigInt result(maxByteCount);

	// Nếu hiệu hai byte có giá trị nhỏ hơn 0
	// thì cần phải mượn 1
	bool borrow = a.bytes[0] - b.bytes[0] < 0 ? 1 : 0;
	result.bytes[0] = a.bytes[0] - b.bytes[0];

	for (int i = 1; i < maxByteCount; i++) {
		result.bytes[i] = a.bytes[i] - b.bytes[i] - (borrow ? 1 : 0);

		// Nếu hiệu hai byte và mượn 1 ở byte trước cũng nhỏ hơn 0
		// thì mượn của byte tiếp theo
		borrow = (a.bytes[i] - b.bytes[i] - (borrow ? 1 : 0)) < 0;
	}

	// Nếu sau khi tính toán mà vẫn còn borrow thì trừ 1 vào byte cuối cùng
	if (borrow)
	{
		result.bytes[result.byteCount - 1] -= 1;
	}
	// Trường hợp ngược lại, xóa byte cuối cùng do rỗng
	else
	{
		removeLastByte(&result);
	}

	return result;
}

BigInt operator-(BigInt a, int value)
{
	BigInt b(value);
	BigInt result(a.byteCount);

	result = a - b;

	return result;
}

void operator-=(BigInt& a, BigInt b)
{
	a = a - b;
}

bool operator==(BigInt a, BigInt b)
{
	//cout << "Compare " << BigIntConverter::bigIntToBinaryStr(a) << " with "
		//<< BigIntConverter::bigIntToBinaryStr(b) << endl;

	BigInt difference = a - b;

	//cout << "The difference: " << BigIntConverter::bigIntToBinaryStr(difference) << endl;

	for (int i = 0; i < difference.byteCount; i++)
	{
		if (difference.bytes[i] != 0)
			return false;
	}

	return true;
}

bool operator==(BigInt a, int value)
{
	BigInt b(value);

	return a == b;
}

bool operator!=(BigInt a, BigInt b)
{
	BigInt difference = a - b;

	for (int i = 0; i < difference.byteCount; i++)
	{
		if (difference.bytes[i] != 0)
			return true;
	}

	return false;
}

bool operator!=(BigInt a, int value)
{
	BigInt b(value);

	return a != b;
}

void fillLastBytesWithNull(BigInt* number, int index)
{
	for (int i = index; i < number->byteCount; i++)
	{
		number->bytes[i] = zero;
	}
}

void shiftByteRight(BigInt* number, int distance)
{
	for (int i = 0; i < number->byteCount - distance; i++)
	{
		number->bytes[i] = number->bytes[i + distance];
	}

	int lastBytesIndex = number->byteCount - distance;
	fillLastBytesWithNull(number, lastBytesIndex);
}

byte getBit(byte number, int index)
{
	return (number >> index) & 1;
}

void copyLowBitsToHighBits(byte a, byte& b, int amount)
{
	for (int i = 0; i < amount; i++)
	{
		// Lấy ra bit thứ i của a
		byte ithBit = getBit(a, i);

		// Gán bit đó cho bit thứ 8 - amount + i của b
		if (ithBit)
		{
			b = (1 << (8 - amount + i)) | b;
		}
		else {
			b = (~(1 << (8 - amount + i))) & b;
		}
	}
}

BigInt operator>>(BigInt& number, int steps)
{
	int byteDistance = steps / 8;
	int bitDistance = steps % 8;

	// Cấp phát thêm một byte rỗng ở đầu
	//number.byteCount++;
	//number.bytes = (byte*)realloc(number.bytes, number.byteCount * sizeof(byte));
	//number.bytes[number.byteCount - 1] = 0;
	//shiftByteLeft(&number, 1);

	// Dịch byte
	if (byteDistance)
	{
		shiftByteRight(&number, byteDistance);
	}

	// Dịch bits
	if (bitDistance) {
		for (int i = 0; i < number.byteCount; i++)
		{
			number.bytes[i] >>= bitDistance;

			if (i < number.byteCount - 1)
			{
				copyLowBitsToHighBits(number.bytes[i + 1], number.bytes[i], bitDistance);
			}
		}
	}

	//removeLastByte(&number);
	return number;
}

void operator >>= (BigInt& number, int steps) {
	number = number >> steps;
}

void fillFirstBytesWithNull(BigInt* number, int index)
{
	for (int i = index; i >= 0; i--)
	{
		number->bytes[i] = zero;
	}
}

void shiftByteLeft(BigInt* number, int distance)
{
	for (int i = number->byteCount - 1; i > distance - 1; i--)
	{
		number->bytes[i] = number->bytes[i - distance];
	}

	int firstBytesIndex = distance - 1;
	fillFirstBytesWithNull(number, firstBytesIndex);
}

void copyHighBitsToLowBits(byte a, byte& b, int amount)
{
	for (int i = 7; i > 7 - amount; i--)
	{
		// Lấy ra bit thứ i của a
		byte ithBit = getBit(a, i);

		// Gán bit đó cho bit thứ i + amount - 8 của b
		if (ithBit)
		{
			b = (1 << (i + amount - 8)) | b;
		}
		else {
			b = (~(1 << (i + amount - 8))) & b;
		}
	}
}

BigInt operator<<(BigInt& number, int steps)
{
	int byteDistance = steps / 8;
	int bitDistance = steps % 8;

	// Cấp phát thêm một byte rỗng ở cuối
	//number.byteCount++;
	//number.bytes = (byte*)realloc(number.bytes, number.byteCount * sizeof(byte));
	//number.bytes[number.byteCount - 1] = 0;

	// Dịch byte
	if (byteDistance)
	{
		shiftByteLeft(&number, byteDistance);
	}

	// Dịch bits
	if (bitDistance) {
		for (int i = number.byteCount - 1; i >= 0; i--)
		{
			number.bytes[i] <<= bitDistance;

			if (i > 0)
			{
				copyHighBitsToLowBits(number.bytes[i - 1], number.bytes[i], bitDistance);
			}
		}
	}

	return number;
}

void operator<<=(BigInt& number, int steps)
{
	number = number << steps;
}

BigInt operator &(BigInt a, BigInt b) {
	uint32_t sharedByteCount = getMaxByteCount(a.byteCount, b.byteCount);

	// Trường hợp hai số có kích thước byte khác nhau
	if (a.byteCount != b.byteCount)
	{
		// Lấy số có kích thước byte ít hơn và cấp phát thêm vùng nhớ
		BigInt* lesserByteNumber = (a.byteCount > b.byteCount) ? &b : &a;
		addPaddingBytes(lesserByteNumber, abs((int)a.byteCount - (int)b.byteCount));
	}

	BigInt result(sharedByteCount);

	for (int i = 0; i < sharedByteCount; i++)
	{
		result.bytes[i] = a.bytes[i] & b.bytes[i];
	}

	return result;
}

BigInt operator |(BigInt a, BigInt b)
{
	uint32_t sharedByteCount = getMaxByteCount(a.byteCount, b.byteCount);

	// Trường hợp hai số có kích thước byte khác nhau
	if (a.byteCount != b.byteCount)
	{
		// Lấy số có kích thước byte ít hơn và cấp phát thêm vùng nhớ
		BigInt* lesserByteNumber = (a.byteCount > b.byteCount) ? &b : &a;
		addPaddingBytes(lesserByteNumber, abs((int)a.byteCount - (int)b.byteCount));
	}

	BigInt result(sharedByteCount);

	for (int i = 0; i < sharedByteCount; i++)
	{
		result.bytes[i] = a.bytes[i] | b.bytes[i];
	}

	return result;
}

BigInt operator*(BigInt a, BigInt b)
{
	BigInt result(0);

	BigIntIO::displayInputs(a, b, "*");

	int k = 0;
	while (b != 0)
	{
		byte firstBit = getFirstBit(&b);
		BigInt add = firstBit == 1 ? a : 0;

		result += add;

		b >>= 1;
		a <<= 1;

		// Nếu đã dời hết một octet (7 lần)
		// thì cấp phát thêm cho a một byte nữa
		if (++k == 7)
		{
			addPaddingBytes(&a, 1);
			k = 0;
		}
	}

	return result;
	}