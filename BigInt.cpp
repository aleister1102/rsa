#include "BigInt.h";
#include "BigIntConverter.h"
#include "BigIntIO.h"


uint32_t getMaxByteCount(uint32_t a, uint32_t b)
{
	return a > b ? a : b;
}

byte getLastByte(const BigInt& n)
{
	return n.bytes[n.byteCount - 1];
}

byte getFirstByte(const BigInt& n)
{
	return n.bytes[0];
}

byte getLastBit(const BigInt& n)
{
	byte lastByte = getLastByte(n);
	return lastByte & ByteMask[7];
}

byte getFirstBit(const BigInt& n)
{
	byte firstByte = getFirstByte(n);
	return firstByte & ByteMask[0];
}

bool BigInt::isPositive()
{
	byte lastBit = getLastBit(*this);
	bool res = !this->isZero() && lastBit == zero; // khác zero và có bit cuối là 0
	return  res;
}

bool BigInt::isNegative()
{
	byte lastBit = getLastBit(*this);
	bool res = !this->isZero() && lastBit != zero; // khác 0 và có bit cuối khác 0
	return res;
}

bool BigInt::isOdd()
{
	byte firstBit = getFirstBit(*this);
	return firstBit != zero;
}

bool BigInt::isEven()
{
	byte firstBit = getFirstBit(*this);
	return firstBit == zero;
}

bool BigInt::isZero()
{
	BigInt temp = *this;
	removeLastBytesIfNull(temp);

	bool res = false;
	if (temp.byteCount == 1 && temp.bytes[0] == zero)
		res = true;
	return res;
}

/// Lấy ra giá trị 4 byte giá trị của một số bất kỳ
int BigInt::getIntValue()
{
	int value = 0;
	BigInt absNum = abs(*this);

	/// Nếu kích thước lớn hơn 4 byte thì chỉ lấy 4 byte
	uint32_t valueBytesCount = absNum.byteCount > 4 ? 4 : absNum.byteCount;

	for (uint32_t i = 0; i < valueBytesCount; i++) {
		value += absNum.bytes[i] * ((int)pow(256, i)); /// mỗi byte sẽ có cơ số là 256
	}

	return value;
}

byte getBit(byte n, int index)
{
	return (n >> index) & ByteMask[0];
}

void setBit(byte& n, int index, byte bit)
{
	if (bit)
	{
		n = (1 << index) | n;
	}
	else {
		n = (~(1 << index)) & n;
	}
}

void swap(BigInt& a, BigInt& b) {
	BigInt temp = a;
	a = b;
	b = temp;
}

void addMoreBytes(BigInt& n, int amount)
{
	n.byteCount += amount;
	auto newMem = (byte*)realloc(n.bytes, n.byteCount * sizeof(byte));
	n.bytes = newMem ? newMem : nullptr;

	int paddingPosition = n.byteCount - amount;
	for (int i = 0; i < amount; i++)
	{
		if (n.bytes)
			n.bytes[paddingPosition + i] = zero;
	}
}

// REFACTOR: kiểm tra số byte rỗng và realloc 1 lần duy nhất
void removeLastBytesIfNull(BigInt& n, int preserve)
{
	byte lastByte = getLastByte(n);

	while (lastByte == zero && n.byteCount > preserve) // preserve là số byte tối thiểu được phép giữ lại
	{
		n.byteCount -= 1;
		auto newMem = (byte*)realloc(n.bytes, n.byteCount * sizeof(byte));
		n.bytes = newMem ? newMem : nullptr;

		lastByte = getLastByte(n);
	}
}

void removeLastBytes(BigInt& n, uint32_t amount)
{
	while (amount != 0 && n.byteCount - amount > 0)
	{
		n.byteCount -= 1;
		auto newMem = (byte*)realloc(n.bytes, n.byteCount * sizeof(byte));
		n.bytes = newMem ? newMem : nullptr;
	}
}

void removeExceedingByte(BigInt& n)
{
	int32_t exceedingByteCount = n.byteCount - MAXBYTE * 2;

	if (exceedingByteCount > 0)
	{
		n.byteCount -= exceedingByteCount;
		auto newMem = (byte*)realloc(n.bytes, n.byteCount * sizeof(byte));
		n.bytes = newMem ? newMem : nullptr;
	}
}

void fillLastBytesWithNull(BigInt& n, int amount)
{
	int end = n.byteCount < amount ? 0 : n.byteCount - amount;

	for (int i = n.byteCount - 1; i >= end; i--)
	{
		n.bytes[i] = 0;
	}
}

void fillFirstBytesWithNull(BigInt& n, int amount)
{
	int end = n.byteCount < amount ? n.byteCount : amount;
	for (int i = 0; i < end; i++)
	{
		n.bytes[i] = 0;
	}
}

uint32_t shareByteCount(BigInt& a, BigInt& b)
{
	uint32_t extraByteAmount = a.byteCount;

	// Trường hợp hai số có kích thước byte khác nhau
	if (a.byteCount != b.byteCount)
	{
		// Lấy số có kích thước byte ít hơn và cấp phát thêm vùng nhớ
		BigInt* lesserByteNumber = (a.byteCount > b.byteCount) ? &b : &a;
		extraByteAmount = abs((int32_t)a.byteCount - (int32_t)b.byteCount);
		addMoreBytes(*lesserByteNumber, extraByteAmount);
	}

	return extraByteAmount;
}

BigInt twoComplement(BigInt n)
{
	if (n.isZero()) return n;
	BigInt res = ~(n);
	res += 1;
	return res;
}

// TODO: đảm bảo rằng không cần cắt bớt byte rỗng ở cuối
BigInt operator+(BigInt a, BigInt b)
{
	uint32_t maxByteCount = getMaxByteCount(a.byteCount, b.byteCount);
	BigInt res(maxByteCount + 1); // cấp phát dư 1 byte

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

	// Nếu sau khi cộng tất cả các byte mà vẫn còn nhớ
	// thì cộng sang byte dư ra
	if (carry)
	{
		res.bytes[res.byteCount - 1] += 1;
	}

	// TODO: bỏ bớt byte thừa khi
	removeExceedingByte(res);

	//io.writeOutputs(a, b, res, " + ");

	return res;
}

BigInt operator+(BigInt a, int value)
{
	BigInt result(a.byteCount);
	BigInt b = value;

	result = a + b;

	return result;
}

void operator+=(BigInt& a, BigInt b)
{
	a = a + b;
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

	// Cách 2: trừ cho bù 2 của b
	//BigInt res = a + twoComplement(b);

	//io.writeOutputs(a, b, res, " - ");

	return res;
}

BigInt operator-(BigInt a, int value)
{
	BigInt res(a.byteCount);
	BigInt b = value;

	res = a - b;
	return res;
}

void operator-=(BigInt& a, BigInt b)
{
	a = a - b;
}

bool operator==(BigInt a, BigInt b)
{
	// Kiểm tra xem hiệu của hai số có khác 0 hay không
	BigInt difference = a - b;
	bool res = true;

	for (int i = 0; i < difference.byteCount; i++)
	{
		if (difference.bytes[i] != 0)
		{
			res = false;
			break;
		}
	}

	//io.writeOutputs(a, b, res, " == ");

	return res;
}

bool operator==(BigInt a, int value)
{
	BigInt b = value;
	bool res = a == b;
	return res;
}

bool operator!=(BigInt a, BigInt b)
{
	bool res = !(a == b);

	//io.writeOutputs(a, b, res, " != ");
	return res;
}

bool operator!=(BigInt a, int value)
{
	BigInt b = value;

	return a != b;
}

void shiftByteLeft(BigInt& n, int distance)
{
	for (int i = n.byteCount - 1; i >= distance - 1; i--)
	{
		n.bytes[i] = n.bytes[i - distance];
	}

	fillFirstBytesWithNull(n, distance);
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

BigInt operator<<(BigInt n, int steps)
{
	int byteDistance = steps / 8;
	int bitDistance = steps % 8;
	BigInt res = n;

	// Thêm byte để có vị trí dịch bit
	addMoreBytes(res, byteDistance + (bitDistance ? 1 : 0));

	if (byteDistance)
	{
		shiftByteLeft(res, byteDistance);
	}

	if (bitDistance) {
		for (int i = res.byteCount - 1; i >= 0; i--)
		{
			res.bytes[i] <<= bitDistance;

			if (i > 0)
			{
				copyHighBitsToLowBits(res.bytes[i - 1], res.bytes[i], bitDistance);
			}
		}
	}

	//io.writeOutputs(n, steps, res, " << ");
	return res;
}

void shiftByteRight(BigInt& n, int distance)
{
	for (int i = 0; i < n.byteCount - distance; i++)
	{
		n.bytes[i] = n.bytes[i + distance];
	}

	fillLastBytesWithNull(n, distance);
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

BigInt operator>>(BigInt n, int steps)
{
	BigInt res = n;
	int byteDistance = steps / 8;
	int bitDistance = steps % 8;

	if (byteDistance)
	{
		shiftByteRight(res, byteDistance);
	}

	if (bitDistance) {
		for (int i = 0; i < res.byteCount; i++)
		{
			res.bytes[i] >>= bitDistance;

			if (i < res.byteCount - 1)
			{
				copyLowBitsToHighBits(res.bytes[i + 1], res.bytes[i], bitDistance);
			}
		}
	}

	//io.writeOutputs(n, steps, res, " >> ");
	return res;
}

void operator >>= (BigInt& number, int steps) {
	number = number >> steps;
}

void operator<<=(BigInt& number, int steps)
{
	number = number << steps;
}

BigInt operator &(BigInt a, BigInt b) {
	uint32_t maxByteCount = getMaxByteCount(a.byteCount, b.byteCount);
	BigInt res(maxByteCount);

	shareByteCount(a, b);

	for (int i = 0; i < maxByteCount; i++)
	{
		res.bytes[i] = a.bytes[i] & b.bytes[i];
	}

	//io.writeOutputs(a, b, res, " & ");
	return res;
}

BigInt operator |(BigInt a, BigInt b)
{
	uint32_t maxByteCount = getMaxByteCount(a.byteCount, b.byteCount);
	BigInt res(maxByteCount);

	shareByteCount(a, b);

	for (int i = 0; i < maxByteCount; i++)
	{
		res.bytes[i] = a.bytes[i] | b.bytes[i];
	}

	//io.writeOutputs(a, b, res, " | ");
	return res;
}

bool operator<(BigInt a, BigInt b) {
	shareByteCount(a, b);

	BigInt different = a - b;

	bool res = different.isNegative() && a != b;

	//io.writeOutputs(a, b, res, " < ");
	return res;
}

bool operator<=(BigInt a, BigInt b)
{
	BigInt difference = a - b;

	bool res = difference.isNegative() || a == b;

	//io.writeOutputs(a, b, res, " <= ");
	return res;
}

bool operator>(BigInt a, BigInt b) {
	shareByteCount(a, b);

	BigInt different = a - b;

	bool res = different.isPositive() && a != b;

	//io.writeOutputs(a, b, res, " > ");
	return res;
}

bool operator>=(BigInt a, BigInt b) {
	shareByteCount(a, b);

	BigInt different = a - b;

	bool res = different.isPositive() || a == b;

	//io.writeOutputs(a, b, res, " >= ");
	return res;
}

BigInt operator*(BigInt a, BigInt b)
{
	BigInt res = 0;
	BigInt q = a;
	BigInt p = b;

	// Sắp xếp thứ tự sao cho b sẽ là số nhỏ hơn
	if (a < b)
		swap(a, b);

	// Thuật toán russian peasant
	while (p != 0)
	{
		if (p.isOdd())
			res += q;

		q <<= 1; // nhân đôi q
		p >>= 1; // chia đôi q
	}

	//io.writeOutputs(a, b, res, " * ");
	return res;
}

BigInt operator~(BigInt n) {
	BigInt res = n;

	for (int i = 0; i < res.byteCount; i++)
	{
		// Lật bit
		res.bytes[i] = ~res.bytes[i];
	}

	return res;
}

uint32_t getBitLength(BigInt n) {
	removeLastBytesIfNull(n);
	uint32_t count = (n.byteCount * 8);

	// Trừ bớt các bit 0 ở byte cuối
	byte lastByte = n.bytes[n.byteCount - 1];
	for (int i = 7; i >= 0; i--) {
		if (getBit(lastByte, i) != 0) break;
		count--;
	}

	return count;
}

BigInt abs(BigInt n)
{
	BigInt res = n;
	if (res.isNegative()) res = twoComplement(n);
	return res;
}

void division(BigInt a, BigInt b, BigInt& q, BigInt& r)
{
	if (b == 0) return;

	uint32_t maxByteCount = getMaxByteCount(a.byteCount, b.byteCount);

	//* Nếu hai số khác dấu thì kết quả sẽ là âm
	bool isResultNegative = a.isNegative() != b.isNegative();
	bool isDivided = false;

	q = 0;
	r = 0;
	a = abs(a);
	b = abs(b);

	//* Lấy chênh lệch độ dài bit của hai số
	int32_t bBitLength = getBitLength(b);
	int32_t deltaBitLength = getBitLength(a) - bBitLength;

	//* Lặp đến khi độ dài bit của hai số giống nhau (chênh lệch = 0)
	while (deltaBitLength > 1)
	{
		deltaBitLength -= 1;
		BigInt one = 1;

		//* Cộng vào q ở vị trí delta giá trị 1 (delta giảm dần đến 0)
		q = q + (one << deltaBitLength);

		//* Dịch vị trí của b sang trái delta vị trí và trừ vào số bị chia (số a)
		a = a - (b << deltaBitLength);

		deltaBitLength = getBitLength(a) - bBitLength;

		isDivided = true;
	}

	//* Chia lần cuối cùng (kết quả nằm ở vị trí 0)
	while (a >= b)
	{
		//* Cộng giá trị 1 vào thương rồi trừ số bị chia cho số chia
		q = q + 1;
		a = a - b;

		isDivided = true;
	}

	//* Phần dư
	r = a;

	//* Lấy bù 2 nếu là số âm
	if (isResultNegative)
	{
		q = twoComplement(q);

		//* Nếu có thực hiện phép chia thì chuyển số dư thành dạng bù 2
		if (isDivided)
			r = twoComplement(a);
	}

	int32_t paddingByteCount = maxByteCount - q.byteCount;
	int32_t excessByteCount = r.byteCount - maxByteCount;

	// WARN: kiểm soát việc cấp phát thêm byte
	//* Thêm byte đệm nếu thương số có ít hơn maxByteCount byte
	if (paddingByteCount > 0)
	{
		//addMoreBytes(q, paddingByteCount);
	}
	//* Xóa byte thừa nếu số dư có nhiều hơn maxByteCount byte
	else if (excessByteCount > 0)
	{
		//removeLastBytes(r, excessByteCount);
	}
}

BigInt operator/(BigInt a, BigInt b)
{
	BigInt q, r;

	division(a, b, q, r);

	//io.writeOutputs(a, b, q, " / ");
	return q;
}

BigInt operator%(BigInt a, BigInt b)
{
	BigInt q, r;

	division(a, b, q, r);

	//io.writeOutputs(a, b, r, " % ");
	return r;
}