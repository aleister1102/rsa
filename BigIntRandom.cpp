#include "BigIntRandom.h"
#include "BigIntConverter.h"
#include "BigIntIO.h"

using std::cout;
using std::endl;

BigInt BigIntRandom::next(uint32_t bitCount)
{
	if (bitCount < 8) return BigInt(); // tối thiểu phải là 8 bit

	uint32_t byteCount = bitCount / 8 + (bitCount % 8 ? 1 : 0);
	BigInt res(byteCount);

	// Random mỗi byte giá trị từ 0 đến 255
	for (int i = 0; i < res.byteCount; i++)
	{
		res.bytes[i] = rand() % 256;
	}

	res = abs(res);
	return res;
}

// BUG: lặp vô hạn khi byte cuối của n = 0
BigInt BigIntRandom::next(BigInt n)
{
	uint32_t maxBitCount = getBitLength(n);
	BigInt res = next(maxBitCount);

	// Chia lấy dư để res không vượt quá n
	res = res % n;
	return res;
}