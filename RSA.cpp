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

string RSA::encrypt(string plainText, BigInt n, BigInt e) {
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

string RSA::decrypt(string cipherText, BigInt n, BigInt d) {
	string plainText = "";
	string binStr = "";

	for (char character : cipherText)
	{
		// Nếu như không phải khoảng trắng thì cho vào chuỗi các số
		if (character != ' ')
			binStr += character;
		// Nếu gặp khoảng trắng thì tiến hành chuyển chuỗi các số về dạng ascii
		else
		{
			io.writeLog("[RSA::decrypt] decrypt " + binStr);

			BigInt c = converter.binaryStrToBigInt(binStr);

			BigInt m = powMod(c, d, n);

			io.writeLog("[RSA::encrypt] ascii value: " + converter.bigIntToBinaryStr(m));

			int ascii = m.getIntValue();

			io.writeLog("[RSA::encrypt] ascii value as int: " + to_string(ascii));

			plainText += char(ascii);

			binStr = ""; // Reset chuỗi nhị phân
		}
	}

	return plainText;
}

void RSA::encryptFile(string plainTextFile, BigInt n, BigInt e, string cipherTextFile)
{
	io.writeOutput("[RSA::encryptFile] encrypting " + plainTextFile);

	fstream pfs, cfs;
	if (io.openFile(pfs, plainTextFile, ios::in) == false) return;
	if (io.openFile(cfs, cipherTextFile, ios::out) == false) return;

	string plainText = io.readContent(pfs);
	string cipherText = encrypt(plainText, n, e);
	io.writeOutput(cfs, cipherText);

	pfs.close();
	cfs.close();

	io.writeOutput("[RSA::encryptFile] finish encrypting!");
}

void RSA::decryptFile(string cipherTextFile, BigInt n, BigInt d, string plainTextFile)
{
	io.writeOutput("[RSA::decryptFile] decrypting " + cipherTextFile);

	fstream cfs, pfs;
	if (io.openFile(cfs, cipherTextFile, ios::in) == false) return;
	if (io.openFile(pfs, plainTextFile, ios::out) == false) return;

	string cipherText = io.readContent(cfs);
	string plainText = decrypt(cipherText, n, d);
	io.writeOutput(pfs, plainText);

	cfs.close();
	pfs.close();

	io.writeOutput("[RSA::decryptFile] finish decrypting!");
}

void RSA::test()
{
	// Sinh khóa từ hai số p và q cho trước (cũng có thể sinh ngẫu nhiên nếu muốn)
	//BigInt p = converter.binaryStrToBigInt("0000000001001000101110000010000011001001010011100000100001110001");
	//BigInt q = converter.binaryStrToBigInt("000000000000001010000010000010101011110010111100101101001000011000110011");
	//RSA rsa(p, q);

	BigInt n = converter.binaryStrToBigInt("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001011011001100000110011110000011011110000001000011011011011000101010011011111001111011110011010101101010010000011");

	// Mã hóa
	BigInt e = converter.binaryStrToBigInt("0000001101011010000010110001110000111111111111011100101010000011");
	RSA::encryptFile("plain.txt", n, e, "cipher.txt");

	// Giải mã
	BigInt d = converter.binaryStrToBigInt("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111100100110011111000100010101111100011101110011011111011011101110010011101111100000111100001111000000000001011");
	RSA::decryptFile("cipher.txt", n, d, "decrypted_plain.txt");
}