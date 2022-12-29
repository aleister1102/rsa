#include "RSA.h"
#include "BigIntRandom.h"
#include "BigIntIO.h"

RSA::RSA(uint32_t byteCount)
{
	// Random ra hai số nguyên tố p và q
	do {
		p = random.next(byteCount);
	} while (p.isPrime(5) == false);

	io.writeLog("[RSA::RSA] random p: " + converter.bigIntToBinaryStr(p));

	do {
		q = random.next(byteCount);
	} while (q.isPrime(5) == false);

	io.writeLog("[RSA::RSA] random q: " + converter.bigIntToBinaryStr(q));

	// Tính n và phi
	n = p * q;
	phi = (p - 1) * (q - 1);

	// Sinh khóa e nguyên tố cùng nhau với phi
	do {
		e = random.next(byteCount);
		if (e.isEven()) continue; // e cần phải lẻ
	} while (gcd(e, phi) != 1);

	// Sinh khóa d là nghịch đảo modulo của e
}