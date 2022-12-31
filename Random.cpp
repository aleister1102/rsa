#include "IO.h"
#include "Random.h"
#include "Converter.h"

BigInt Random::next(uint32_t byteCount)
{
	if (byteCount < 0) return BigInt();
	BigInt res(byteCount);

	//? Random mỗi byte giá trị từ 0 đến 255
	for (uint32_t i = 0; i < res.byteCount; i++)
	{
		res.bytes[i] = rand() % 256;
	}

	res = abs(res);

	//io.writeLog("[Random::next] random number: ", res, BigIntBase::BASE_10);
	return res;
}

BigInt Random::next(BigInt n)
{
	uint32_t maxBitCount = n.getBitLength();
	uint32_t resByteCount = maxBitCount / 8;
	BigInt res = next(resByteCount ? resByteCount : 1);

	//? Chia lấy dư để res không vượt quá n
	res = res % n;

	//io.writeLog("[Random::next] random number smaller than n: ", res, BigIntBase::BASE_10);
	return res;
}

BigInt Random::next(BigInt a, BigInt b)
{
	BigInt res = next(b - a);

	res = res + a;

	//io.writeLog("[Random::next] random number in [a, b): ", res, BigIntBase::BASE_10);
	return res;
}