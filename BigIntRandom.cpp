#include "BigIntRandom.h"
#include "BigIntConverter.h"
#include "IO.h"

using std::cout;
using std::endl;

BigInt BigIntRandom::next(uint32_t byteCount)
{
	if (byteCount < 0) return BigInt();

	BigInt res(byteCount);

	// Random mỗi byte giá trị từ 0 đến 255
	for (uint32_t i = 0; i < res.byteCount; i++)
	{
		res.bytes[i] = rand() % 256;
	}

	res = abs(res);

	//io.writeLog("[BigIntRandom::next] random number: " + converter.bigIntToBinaryStr(res));
	return res;
}

BigInt BigIntRandom::next(BigInt n)
{
	uint32_t maxBitCount = getBitLength(n);
	uint32_t resByteCount = maxBitCount / 8 - 1;
	BigInt res = next(resByteCount);

	// Chia lấy dư để res không vượt quá n
	res = res % n;

	//io.writeLog("[BigIntRandom::next] random number smaller than n: " + converter.bigIntToBinaryStr(res));
	return res;
}

BigInt BigIntRandom::next(BigInt a, BigInt b)
{
	BigInt res = next(b - a);

	res = res + a;

	//io.writeLog("[BigIntRandom::next] random number in [a, b): " + converter.bigIntToBinaryStr(res));
	return res;
}