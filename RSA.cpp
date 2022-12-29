#include "RSA.h"
#include "BigIntRandom.h"
#include "BigIntIO.h"

using std::tie;
using std::make_tuple;
using std::cout;

RSA::RSA(uint32_t byteCount)
{
	this->byteCount = byteCount;

	// Random ra hai số nguyên tố p và q
	generatePrimes();

	//// Tính n và phi
	n = p * q;
	phi = (p - 1) * (q - 1);
	io.writeOutput("[RSA::RSA] n: " + converter.bigIntToBinaryStr(n));
	io.writeOutput("[RSA::RSA] phi: " + converter.bigIntToBinaryStr(phi));

	//// Sinh khóa e nguyên tố cùng nhau với phi
	generateEncryptionKey();

	//// Sinh khóa d là nghịch đảo modulo của e
	generateDecryptionKey();
}

void RSA::generatePrimes()
{
	cout << "[RSA::generatePrimes] generating p and q...\n";

	do {
		p = random.next(byteCount);
	} while (p.isPrime(CHECK_PRIME_LOOPS) == false);

	io.writeOutput("[RSA::generatePrimes] random p: " + converter.bigIntToBinaryStr(p));

	do {
		q = random.next(byteCount);
	} while (q.isPrime(CHECK_PRIME_LOOPS) == false);

	io.writeOutput("[RSA::generatePrimes] random q: " + converter.bigIntToBinaryStr(q));

	cout << "[RSA::generatePrimes] finish random p and q!\n";
}

void RSA::generateEncryptionKey()
{
	if (phi == 0) return;

	cout << "[RSA::generateEncryptionKey] generating encryption key e...\n";

	do {
		e = random.next(byteCount);
		if (e.isEven()) continue; // e cần phải lẻ
	} while (gcd(e, phi) != 1);

	io.writeOutput("[RSA::generateEncryptionKey] encryption key: " + converter.bigIntToBinaryStr(e));

	cout << "[RSA::generateEncryptionKey] finish generating encryption key e!\n";
}

void RSA::generateDecryptionKey()
{
	if (phi == 0 || e == 0) return;

	cout << "[RSA::generateDecryptionKey] generating decryption key d...\n";

	d = inverseMod(e, phi);

	io.writeOutput("[RSA::generateDecryptionKey] decryption key: " + converter.bigIntToBinaryStr(d));

	cout << "[RSA::generateDecryptionKey] finish generating decryption key d!\n";
}