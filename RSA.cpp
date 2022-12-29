#include "RSA.h"
#include "BigIntRandom.h"
#include "IO.h"
#include <fstream>

using std::cout;
using std::to_string;

RSA::RSA(uint32_t byteCount)
{
	this->byteCount = byteCount;

	// Random ra hai số nguyên tố p và q
	generatePrimes();

	//// Tính n và phi
	calculatePhiAndN();

	//// Sinh khóa e nguyên tố cùng nhau với phi
	generateEncryptionKey();

	//// Sinh khóa d là nghịch đảo modulo của e
	generateDecryptionKey();
}

RSA::RSA(BigInt p, BigInt q)
{
	this->byteCount = p.byteCount;
	this->p = p;
	this->q = q;

	calculatePhiAndN();
	generateEncryptionKey();
	generateDecryptionKey();
}

RSA::RSA(BigInt d)
{
	this->d = d;
}

void RSA::generatePrimes()
{
	io.writeOutput("[RSA::generatePrimes] generating p and q...");

	do {
		p = random.next(byteCount);
	} while (p.isPrime(CHECK_PRIME_LOOPS) == false);

	io.writeOutput("[RSA::generatePrimes] random p: " + converter.bigIntToBinaryStr(p));

	do {
		q = random.next(byteCount);
	} while (q.isPrime(CHECK_PRIME_LOOPS) == false);

	io.writeOutput("[RSA::generatePrimes] random q: " + converter.bigIntToBinaryStr(q));
}

void RSA::calculatePhiAndN()
{
	phi = (p - 1) * (q - 1);
	n = p * q;
	io.writeOutput("[RSA::RSA] phi: " + converter.bigIntToBinaryStr(phi));
	io.writeOutput("[RSA::RSA] n: " + converter.bigIntToBinaryStr(n));
}

void RSA::generateEncryptionKey()
{
	if (phi == 0) return;

	io.writeOutput("[RSA::generateEncryptionKey] generating encryption key e...");

	do {
		e = random.next(byteCount);
		if (e.isEven()) continue; // e cần phải lẻ
	} while (gcd(e, phi) != 1);

	io.writeOutput("[RSA::generateEncryptionKey] encryption key: " + converter.bigIntToBinaryStr(e));
}

void RSA::generateDecryptionKey()
{
	if (phi == 0 || e == 0) return;

	io.writeOutput("[RSA::generateDecryptionKey] generating decryption key d...");

	d = inverseMod(e, phi);

	io.writeOutput("[RSA::generateDecryptionKey] decryption key: " + converter.bigIntToBinaryStr(d));
}

string RSA::encrypt(string plainText) {
	string cipherText = "";

	for (char character : plainText)
	{
		io.writeLog("[RSA::encrypt] encrypt: " + to_string(character));

		BigInt m = (int)character;

		io.writeLog("[RSA::encrypt] ascii value: " + converter.bigIntToBinaryStr(m));

		BigInt c = powMod(m, e, n);

		io.writeLog("[RSA::encrypt] cipher text: " + converter.bigIntToBinaryStr(c));

		cipherText += (converter.bigIntToBinaryStr(c) + " ");
	}

	return cipherText;
}

void RSA::encryptFile(string plainTextFile, BigInt encryptionKey, string cipherTextFile)
{
	io.writeOutput("[RSA::encryptFile] encrypting " + plainTextFile);

	fstream pfs, cfs;
	if (io.openFile(pfs, plainTextFile, ios::in) == false) return;
	if (io.openFile(cfs, cipherTextFile, ios::out) == false) return;

	string plainText = io.readContent(pfs);
	string cipherText = encrypt(plainText);
	io.writeOutput(cfs, cipherText);

	io.writeOutput("[RSA::encryptFile] cipher text " + cipherText);

	pfs.close();
	cfs.close();
}