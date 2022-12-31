#include "RSA.h"
#include "IO.h"
#include "Algorithm.h"
#include "Converter.h"
#include "Random.h"
#include <fstream>

using std::cout;
using std::to_string;

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
	io.writeOutput("[RSA::RSA] n: " + converter.bigIntToDecimalStr(n));
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
	io.writeOutput("[RSA::generateEncryptionKey] e: " + converter.bigIntToDecimalStr(e));
}

void RSA::generateDecryptionKey()
{
	if (phi == 0 || e == 0) return;

	io.writeConsole("Generating decryption key d...");

	d = inverseMod(e, phi);

	io.writeConsole("Finish generating decryption key d!");
	io.writeOutput("[RSA::generateDecryptionKey] d: " + converter.bigIntToDecimalStr(d));
}

string RSA::encrypt(string plainText) {
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

string RSA::decrypt(string cipherText) {
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

			io.writeLog("[RSA::encrypt] ascii value: " + converter.bigIntToDecimalStr(m));

			int ascii = m.getIntValue();

			//io.writeLog("[RSA::encrypt] ascii value as int: " + to_string(ascii));

			plainText += char(ascii);

			binStr = ""; // Reset chuỗi nhị phân
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

	keys += "n: " + converter.bigIntToDecimalStr(n) + '\n';
	keys += "e: " + converter.bigIntToDecimalStr(e) + '\n';
	keys += "d: " + converter.bigIntToDecimalStr(d) + '\n';
	keys += "phi: " + converter.bigIntToDecimalStr(phi) + '\n';

	return keys;
}

string RSA::getBinaryKeys()
{
	string keys;

	keys += "n: " + converter.bigIntToBinaryStr(n) + '\n';
	keys += "e: " + converter.bigIntToBinaryStr(e) + '\n';
	keys += "d: " + converter.bigIntToBinaryStr(d) + '\n';
	keys += "phi: " + converter.bigIntToBinaryStr(phi) + '\n';

	return keys;
}

void RSA::exportKeys(int format, int exportMethod)
{
	io.writeConsole("Exporting keys...");

	fstream fs("keys.txt", ios::out);
	if (fs.is_open() == false) return;

	string keys = "";
	if (format == 10)
		keys = getKeys();
	else if (format == 2)
		keys = getBinaryKeys();

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

void testRSA() {
	// Sinh khóa từ hai số p và q cho trước (cũng có thể sinh ngẫu nhiên nếu muốn)
	//BigInt p = converter.binaryStrToBigInt("0000000001001000101110000010000011001001010011100000100001110001");
	//BigInt q = converter.binaryStrToBigInt("000000000000001010000010000010101011110010111100101101001000011000110011");
	//RSA rsa(p, q);

	BigInt n = converter.decimalStrToBigInt("3332934698137425361660362443025144786509522466962699924195150122084633952149");
	io.writeOutput("[RSA::test] n: " + converter.bigIntToDecimalStr(n));

	BigInt::maxByteCount = n.byteCount;

	BigInt e = converter.decimalStrToBigInt("169093636435538073422978025304282854731");
	io.writeOutput("[RSA::test] e: " + converter.bigIntToDecimalStr(e));

	BigInt d = converter.decimalStrToBigInt("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111100100110011111000100010101111100011101110011011111011011101110010011101111100000111100001111000000000001011");

	RSA rsa(n, e, d);

	// Mã hóa
	rsa.encryptFile("plain.txt", "cipher.txt");

	// Giải mã
	rsa.decryptFile("cipher.txt", "decrypted.txt");
}

BigInt RSA::getD()
{
	return d;
}