#include "BigIntRandom.h"
#include "BigIntConverter.h"
#include "BigIntIO.h"

using std::cout;
using std::endl;

BigInt BigIntRandom::next(uint32_t bitCount)
{
	//cout << "Random number...." << endl;

	if (bitCount < 8) return BigInt(); // tối thiểu phải là 8 bit

	uint32_t byteCount = bitCount / 8;
	BigInt res(byteCount);

	// Random mỗi byte giá trị từ 0 đến 255
	for (int i = 0; i < res.byteCount; i++)
	{
		res.bytes[i] = rand() % 256;
	}

	res = abs(res);
	return res;
}

BigInt BigIntRandom::next(BigInt n)
{
	//cout << "Random number smaller than n..." << endl;

	uint32_t maxByteCount = n.byteCount;
	BigInt res(maxByteCount);

	// Random n - 1 byte đầu giá trị tùy ý
	for (int i = 0; i < res.byteCount - 1; i++)
	{
		res.bytes[i] = rand() % 256;
	}

	byte resLastByte;
	byte nLastByte = getLastByte(n);

	// Random byte cuối cùng sao cho có giá trị nhỏ hơn byte cuối cùng của n
	do {
		res.bytes[res.byteCount - 1] = rand() % 255;
		resLastByte = getLastByte(res);
	} while (resLastByte >= nLastByte);

	return res;
}