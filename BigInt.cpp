#include "BigInt.h";
#include "BigIntConverter.h"
#include "BigIntIO.h"

uint32_t getMaxByteCount(int a, int b)
{
	return a > b ? a : b;
}

byte getLastByte(const BigInt* n)
{
	return n->bytes[n->byteCount - 1];
}

byte getFirstByte(const BigInt* n)
{
	return n->bytes[0];
}

byte getLastBit(const BigInt* n)
{
	byte lastByte = getLastByte(n);
	return lastByte & ByteMask[7];
}

byte getFirstBit(const BigInt* n)
{
	byte firstByte = getFirstByte(n);
	return firstByte & ByteMask[0];
}

void addPaddingBytes(BigInt* n, int amount)
{
	n->byteCount += amount;
	auto newMem = (byte*)realloc(n->bytes, n->byteCount * sizeof(byte));
	n->bytes = newMem ? newMem : nullptr;

	int paddingPosition = n->byteCount - amount;
	for (int i = 0; i < amount; i++)
	{
		if (n->bytes)
			n->bytes[paddingPosition + i] = ZERO;
	}
}

void removeLastByteIfNull(BigInt* n, bool checkNull = true)
{
	byte lastByte = getLastByte(n);

	if (checkNull && lastByte == 0)
	{
		n->byteCount -= 1;
		auto newMem = (byte*)realloc(n->bytes, n->byteCount * sizeof(byte));
		n->bytes = newMem ? newMem : nullptr;
	}
}

void shareByteCount(BigInt& a, BigInt& b)
{
	// Trường hợp hai số có kích thước byte khác nhau
	if (a.byteCount != b.byteCount)
	{
		// Lấy số có kích thước byte ít hơn và cấp phát thêm vùng nhớ
		BigInt* lesserByteNumber = (a.byteCount > b.byteCount) ? &b : &a;
		addPaddingBytes(lesserByteNumber, abs((int)a.byteCount - (int)b.byteCount));
	}
}

BigInt operator+(BigInt a, BigInt b)
{
	uint32_t maxByteCount = getMaxByteCount(a.byteCount, b.byteCount);
	BigInt res(maxByteCount);

	shareByteCount(a, b);

	// Nếu tổng hai byte có giá trị lớn hơn 255
	// thì cần phải nhớ 1
	bool carry = a.bytes[0] + b.bytes[0] > 255 ? 1 : 0;
	res.bytes[0] = a.bytes[0] + b.bytes[0];

	for (int i = 1; i < maxByteCount; i++) {
		res.bytes[i] = a.bytes[i] + b.bytes[i] + (carry ? 1 : 0);

		// Nếu tổng hai byte và nhớ một ở byte trước cũng lớn hơn 255
		// thì nhớ 1 sang byte tiếp theo55
		carry = (a.bytes[i] + b.bytes[i] + (carry ? 1 : 0)) > 255;
	}

	//io.writeOutputs(a, b, res, " + ");

	return res;
}

BigInt operator+(BigInt a, int value)
{
	BigInt result(a.byteCount);
	BigInt b(value);

	result = a + b;

	return result;
}

void operator+=(BigInt& a, BigInt b)
{
	a = a + b;
}

void twoComplement(BigInt& n)
{
	for (int i = 0; i < n.byteCount; i++)
	{
		// Lật bit
		n.bytes[i] = ~n.bytes[i];
	}

	n = n + 1;
}

BigInt operator-(BigInt a, BigInt b)
{
	uint32_t maxByteCount = getMaxByteCount(a.byteCount, b.byteCount);
	BigInt res(maxByteCount);

	shareByteCount(a, b);

	// Cách 1: trừ từng byte
	//
	// Nếu hiệu hai byte có giá trị nhỏ hơn 0
	// thì cần phải mượn 1
	bool borrow = (a.bytes[0] - b.bytes[0] < 0) ? 1 : 0;
	res.bytes[0] = a.bytes[0] - b.bytes[0];

	for (int i = 1; i < maxByteCount; i++) {
		res.bytes[i] = a.bytes[i] - b.bytes[i] - (borrow ? 1 : 0);

		// Nếu hiệu hai byte và mượn 1 ở byte trước cũng nhỏ hơn 0
		// thì mượn của byte tiếp theo
		borrow = (a.bytes[i] - b.bytes[i] - (borrow ? 1 : 0)) < 0;
	}

	//io.writeOutputs(a, b, res, " - ");

	return res;
}

BigInt operator-(BigInt a, int value)
{
	BigInt res(a.byteCount);
	BigInt b(value);

	res = a - b;
	return res;
}

void operator-=(BigInt& a, BigInt b)
{
	a = a - b;
}

bool operator==(BigInt a, BigInt b)
{
	BigInt difference = a - b;

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
		number->bytes[i] = ZERO;
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

void setBit(byte& number, int index, byte bit)
{
	if (bit)
	{
		number = (1 << index) | number;
	}
	else {
		number = (~(1 << index)) & number;
	}
}

void copyLowBitsToHighBits(byte& a, byte& b, int amount)
{
	for (int i = 0; i < amount; i++)
	{
		// Lấy ra bit thứ i của a
		byte ithBit = getBit(a, i);

		// Gán bit đó cho bit thứ 8 - amount + i của b
		setBit(b, 8 - amount + i, ithBit);
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
		number->bytes[i] = ZERO;
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
		setBit(b, i + amount - 8, ithBit);
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
	io.displayInputs(a, b, "*");

	BigInt result(0);

	int k = 0;
	while (b != 0)
	{
		byte firstBit = getFirstBit(&b);

		if (firstBit)
			result += a;
		else
			result += 0;

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

uint16_t getBitCount(const BigInt* bigInt) {
	uint16_t bitCount = (bigInt->byteCount * 8);

	for (int i = 7; i >= 0; i--) {
		if (getBit(bigInt->bytes[bigInt->byteCount - 1], i) == 1) break;
		bitCount--;
	}
	return bitCount;
}

bool isLesserThanZero(const BigInt* number)
{
	byte lastBit = getLastBit(number);
	bool result = lastBit == 0 ? false : true;
	return result;
}

BigInt operator/(BigInt a, BigInt b)
{
	io.displayInputs(a, b, "/");

	uint32_t maxByteCount = getMaxByteCount(a.byteCount, b.byteCount);

	BigInt r(maxByteCount);
	int k = b.byteCount * 8;

	while (k > 0)
	{
		// Dịch trái chuỗi [r, a]

		// Lấy bit cuối của a
		byte lastBitA = getLastBit(&a);

		//cout << "Before shift: " <<
		//	io.outputBin(r) << " " <<
		//	io.outputBin(a) << endl;

		a << 1;
		r << 1;

		// Gán cho bit đầu của r
		setBit(r.bytes[0], 0, lastBitA);

		cout << "After shift: " <<
			io.outputBin(r) << " " <<
			io.outputBin(a) << endl;

		r -= b;
		cout << "After subtract: " << io.outputBin(r) << endl;

		// Nếu r < 0
		if (isLesserThanZero(&r))
		{
			// Khôi phục r và gán bit đầu của a là 0
			r += b;
			setBit(a.bytes[0], 0, 0);

			cout << "Restore: " << io.outputBin(r) << endl;
		}
		else
		{
			// Gán bit đầu của a là 1
			setBit(a.bytes[0], 0, 1);
		}

		k -= 1;
	}

	return a;
}