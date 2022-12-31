#include "RSA.h"
#include "IO.h"
#include "Algorithm.h"
#include "Random.h"
#include "Converter.h"

using std::to_string;
using std::fstream;

RSA::RSA(uint32_t byteCount, int base)
{
	this->byteCount = byteCount;
	this->base = base;

	//? Random ra hai số nguyên tố p và q
	generatePrimes();

	//? Tính n và phi
	calculateNandPhi();

	//? Sinh khóa e nguyên tố cùng nhau với phi
	generateEncryptionKey();

	//? Sinh khóa d là nghịch đảo modulo của e
	generateDecryptionKey();
}

void RSA::generatePrimes()
{
	if (byteCount == 0) return;

	io.writeConsole("Generating p and q...");

	do {
		p = random.next(byteCount);
		io.writeLog("[RSA::generatePrimes] random p: ", p, base);
	} while (p.isPrime(RSA::checkPrimeLoops) == false);

	io.writeOutput("[RSA::generatePrimes] prime p: ", p, base);

	do {
		q = random.next(byteCount);
		io.writeLog("[RSA::generatePrimes] random q: ", p, base);
	} while (q.isPrime(RSA::checkPrimeLoops) == false);

	io.writeOutput("[RSA::generatePrimes] prime q: ", q, base);

	io.writeConsole("Finish generating p and q!");
}

void RSA::calculateNandPhi()
{
	if (p == 0 || q == 0) return;

	io.writeConsole("Calculating n and phi...");

	n = p * q;
	phi = (p - 1) * (q - 1);

	io.writeOutput("[RSA::RSA] n: ", n, base);
	io.writeOutput("[RSA::RSA] phi: ", phi, base);

	io.writeConsole("Finish calculating n and phi!");
}

void RSA::generateEncryptionKey()
{
	if (phi == 0) return;

	io.writeConsole("Generating encryption key e...");

	do {
		e = random.next(byteCount);
		if (e.isEven()) continue; //? e cần phải lẻ
	} while (gcd(e, phi) != 1);

	io.writeConsole("Finish generating encryption key e!");

	io.writeOutput("[RSA::generateEncryptionKey] e: ", e, base);
}

void RSA::generateDecryptionKey()
{
	if (phi == 0 || e == 0) return;

	io.writeConsole("Generating decryption key d...");

	d = inverseMod(e, phi);

	io.writeConsole("Finish generating decryption key d!");

	io.writeOutput("[RSA::generateDecryptionKey] d: ", d, base);
}

string RSA::encrypt(string plainText) {
	string cipherText = "";

	io.writeLog("[RSA::encrypt] encrypting " + plainText);

	for (char character : plainText)
	{
		io.writeLog("[RSA::encrypt] encrypt: " + to_string(character));

		BigInt m = (int)character;

		//io.writeLog("[RSA::encrypt] ascii value: ", m, base);

		BigInt c = powMod(m, e, n);

		//io.writeLog("[RSA::encrypt] cipher text: ", c, base);

		if (base == BigIntBase::BASE_10)
			cipherText += (converter.bigIntToDecimalStr(c) + " ");
		else if (base == BigIntBase::BASE_2)
			cipherText += (converter.bigIntToBinaryStr(c) + " ");
	}

	io.writeLog("[RSA::encrypt] finish encrypting!");
	return cipherText;
}

string RSA::decrypt(string cipherText) {
	string plainText = "";
	string str = "";

	io.writeLog("[RSA::decrypt] decrypting...");

	for (char character : cipherText)
	{
		if (character != ' ')
			str += character;
		else
		{
			//io.writeLog("[RSA::decrypt] decrypt " + str);

			BigInt c;
			if (base == BigIntBase::BASE_10)
				c = converter.decimalStrToBigInt(str);
			else if (base == BigIntBase::BASE_2)
				c = converter.binaryStrToBigInt(str);

			BigInt m = powMod(c, d, n);

			//io.writeLog("[RSA::encrypt] ascii value: ", m, base);

			int ascii = m.getIntValue();

			io.writeLog("[RSA::encrypt] ascii value as int: " + to_string(ascii));

			plainText += char(ascii);

			str = ""; //? Reset chuỗi nhị phân
		}
	}

	io.writeLog("[RSA::decrypt] finish decrypting: " + plainText);
	return plainText;
}

void RSA::encryptFile(string plainTextFile, string cipherTextFile)
{
	io.writeConsole("Encrypting " + plainTextFile);

	fstream pfs, cfs;
	if (io.openFile(pfs, plainTextFile, ios::in) == false) return;
	if (io.openFile(cfs, cipherTextFile, ios::out) == false) return;

	string plainText = io.readFile(pfs);
	string cipherText = encrypt(plainText);
	cfs << cipherText;

	pfs.close();
	cfs.close();

	io.writeConsole("Finish encrypting!");
}

void RSA::decryptFile(string cipherTextFile, string plainTextFile)
{
	io.writeConsole("Decrypting " + cipherTextFile);

	fstream cfs, pfs;
	if (io.openFile(cfs, cipherTextFile, ios::in) == false) return;
	if (io.openFile(pfs, plainTextFile, ios::out) == false) return;

	string cipherText = io.readFile(cfs);
	string plainText = decrypt(cipherText);
	pfs << plainText;

	cfs.close();
	pfs.close();

	io.writeConsole("Finish decrypting!");
}

string RSA::getKeys()
{
	string keys;

	if (base == BigIntBase::BASE_10)
	{
		keys += "n: " + converter.bigIntToDecimalStr(n) + '\n';
		keys += "e: " + converter.bigIntToDecimalStr(e) + '\n';
		keys += "d: " + converter.bigIntToDecimalStr(d) + '\n';
		keys += "phi: " + converter.bigIntToDecimalStr(phi) + '\n';
	}
	else if (base == BigIntBase::BASE_2)
	{
		keys += "n: " + converter.bigIntToBinaryStr(n) + '\n';
		keys += "e: " + converter.bigIntToBinaryStr(e) + '\n';
		keys += "d: " + converter.bigIntToBinaryStr(d) + '\n';
		keys += "phi: " + converter.bigIntToBinaryStr(phi) + '\n';
	}

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
		io.writeConsole(keys);
		io.writeConsole("Finish exporting keys!");
	}
	else if (exportMethod == 3)
	{
		fs << keys;
		io.writeConsole(keys);
		io.writeConsole("Finish exporting keys! Please check 'keys.txt'");
	}
}