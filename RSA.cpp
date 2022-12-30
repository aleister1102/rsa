#include "RSA.h"
#include "IO.h"
#include "Algorithm.h"
#include "BigIntRandom.h"
#include <fstream>

using std::cout;
using std::to_string;

string RSA::N() { return converter.bigIntToDecimalStr(n); }
string RSA::E() { return converter.bigIntToDecimalStr(e); }
string RSA::D() { return converter.bigIntToDecimalStr(d); }
string RSA::PHI() { return converter.bigIntToDecimalStr(phi); }

RSA::RSA(uint32_t byteCount)
{
	this->byteCount = byteCount;

	// Random ra hai số nguyên tố p và q
	generatePrimes();

	//// Tính n và phi
	calculateNandPhi();

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

	calculateNandPhi();
	generateEncryptionKey();
	generateDecryptionKey();
}

void RSA::generatePrimes()
{
	io.writeConsole("Generating p and q...");

	do {
		p = random.next(byteCount);
		io.writeLog("[RSA::generatePrimes] random p: " + converter.bigIntToBinaryStr(p));
	} while (p.isPrime(RSA::checkPrimeLoops) == false);

	io.writeOutput("[RSA::generatePrimes] prime p: " + converter.bigIntToBinaryStr(p));

	do {
		q = random.next(byteCount);
		io.writeLog("[RSA::generatePrimes] random q: " + converter.bigIntToBinaryStr(p));
	} while (q.isPrime(RSA::checkPrimeLoops) == false);

	io.writeOutput("[RSA::generatePrimes] prime q: " + converter.bigIntToBinaryStr(q));

	io.writeConsole("Finish generating p and q!");
}

void RSA::calculateNandPhi()
{
	io.writeConsole("Calculating n and phi...");

	n = p * q;
	phi = (p - 1) * (q - 1);
	io.writeOutput("[RSA::RSA] n: " + N());
	io.writeOutput("[RSA::RSA] phi: " + converter.bigIntToBinaryStr(phi));

	io.writeConsole("Finish calculating n and phi!");
}

void RSA::generateEncryptionKey()
{
	if (phi == 0) return;

	io.writeConsole("Generating encryption key e...");

	do {
		e = random.next(byteCount);
		if (e.isEven()) continue; // e cần phải lẻ
	} while (gcd(e, phi) != 1);

	io.writeConsole("Finish generating encryption key e!");
	io.writeOutput("[RSA::generateEncryptionKey] e: " + E());
}

void RSA::generateDecryptionKey()
{
	if (phi == 0 || e == 0) return;

	io.writeConsole("Generating decryption key d...");

	d = inverseMod(e, phi);

	io.writeConsole("Finish generating decryption key d!");
	io.writeOutput("[RSA::generateDecryptionKey] d: " + D());
}

string RSA::encrypt(string plainText, BigInt n, BigInt e) {
	string cipherText = "";

	io.writeLog("[RSA::encrypt] encrypting " + plainText);

	for (char character : plainText)
	{
		io.writeLog("[RSA::encrypt] encrypt: " + to_string(character));

		BigInt m = (int)character;

		//io.writeLog("[RSA::encrypt] ascii value: " + converter.bigIntToBinaryStr(m));

		BigInt c = powMod(m, e, n);

		io.writeLog("[RSA::encrypt] cipher text: " + converter.bigIntToDecimalStr(c));

		cipherText += (converter.bigIntToDecimalStr(c) + " ");
	}

	io.writeLog("[RSA::encrypt] finish encrypting!");
	return cipherText;
}

string RSA::decrypt(string cipherText, BigInt n, BigInt d) {
	string plainText = "";
	string binStr = "";

	io.writeLog("[RSA::decrypt] decrypting...");

	for (char character : cipherText)
	{
		// Nếu như không phải khoảng trắng thì cho vào chuỗi các số
		if (character != ' ')
			binStr += character;
		// Nếu gặp khoảng trắng thì tiến hành chuyển chuỗi các số về dạng ascii
		else
		{
			//io.writeLog("[RSA::decrypt] decrypt " + binStr);

			BigInt c = converter.decimalStrToBigInt(binStr);

			BigInt m = powMod(c, d, n);

			//io.writeLog("[RSA::encrypt] ascii value: " + converter.bigIntToBinaryStr(m));

			int ascii = m.getIntValue();

			//io.writeLog("[RSA::encrypt] ascii value as int: " + to_string(ascii));

			plainText += char(ascii);

			binStr = ""; // Reset chuỗi nhị phân
		}
	}

	io.writeLog("[RSA::decrypt] finish decrypting: " + plainText);
	return plainText;
}

void RSA::encryptFile(string plainTextFile, BigInt n, BigInt e, string cipherTextFile)
{
	io.writeConsole("[RSA::encryptFile] encrypting " + plainTextFile);

	fstream pfs, cfs;
	if (io.openFile(pfs, plainTextFile, ios::in) == false) return;
	if (io.openFile(cfs, cipherTextFile, ios::out) == false) return;

	string plainText = io.readContent(pfs);
	string cipherText = encrypt(plainText, n, e);
	io.writeOutput(cfs, cipherText);

	pfs.close();
	cfs.close();

	io.writeConsole("[RSA::encryptFile] finish encrypting!");
}

void RSA::decryptFile(string cipherTextFile, BigInt n, BigInt d, string plainTextFile)
{
	io.writeConsole("[RSA::decryptFile] decrypting " + cipherTextFile);

	fstream cfs, pfs;
	if (io.openFile(cfs, cipherTextFile, ios::in) == false) return;
	if (io.openFile(pfs, plainTextFile, ios::out) == false) return;

	string cipherText = io.readContent(cfs);
	string plainText = decrypt(cipherText, n, d);
	io.writeOutput(pfs, plainText);

	cfs.close();
	pfs.close();

	io.writeConsole("[RSA::decryptFile] finish decrypting!");
}

string RSA::getKeys()
{
	string keys;

	keys += "n: " + N() + '\n';
	keys += "e: " + E() + '\n';
	keys += "d: " + D() + '\n';
	keys += "phi: " + PHI() + '\n';

	return keys;
}

void RSA::exportKeys(int exportMethod)
{
	io.writeConsole("Exporting keys...");

	fstream fs("keys.txt", ios::out);
	if (fs.is_open() == false) return;

	string keys = getKeys();

	if (exportMethod == 1)
	{
		fs << keys;
		io.writeConsole("Finish exporting keys! Please check 'keys.txt'");
	}
	else if (exportMethod == 2)
	{
		cout << keys;
		io.writeConsole("Finish exporting keys!");
	}
	else if (exportMethod == 3)
	{
		fs << keys;
		cout << keys;
		io.writeConsole("Finish exporting keys! Please check 'keys.txt'");
	}
}

void RSA::test()
{
	// Sinh khóa từ hai số p và q cho trước (cũng có thể sinh ngẫu nhiên nếu muốn)
	//BigInt p = converter.binaryStrToBigInt("0000000001001000101110000010000011001001010011100000100001110001");
	//BigInt q = converter.binaryStrToBigInt("000000000000001010000010000010101011110010111100101101001000011000110011");
	//RSA rsa(p, q);

	BigInt n = converter.decimalStrToBigInt("3332934698137425361660362443025144786509522466962699924195150122084633952149");
	io.writeOutput("[RSA::test] n: " + converter.bigIntToDecimalStr(n));

	BigInt::maxByteCount = n.byteCount;

	// Mã hóa
	BigInt e = converter.decimalStrToBigInt("169093636435538073422978025304282854731");
	io.writeOutput("[RSA::test] e: " + converter.bigIntToDecimalStr(e));

	RSA::encryptFile("plain.txt", n, e, "cipher.txt");

	// Giải mã
	//BigInt d = converter.decimalStrToBigInt("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111100100110011111000100010101111100011101110011011111011011101110010011101111100000111100001111000000000001011");
	//RSA::decryptFile("cipher.txt", n, d, "decrypted.txt");
}

BigInt RSA::getD()
{
	return d;
}