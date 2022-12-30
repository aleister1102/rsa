#pragma once
#include "BigInt.h"

using std::tuple;
using std::string;

class RSA
{
public:
	static void test();
	BigInt getD();

public:
	static string encrypt(string plainText, BigInt n, BigInt e);
	static string decrypt(string cipherText, BigInt n, BigInt d);
	static void encryptFile(string p, BigInt n, BigInt e, string c);
	static void decryptFile(string c, BigInt n, BigInt d, string p);
	void exportKeys(int exportMethod);

public:
	RSA(uint32_t byteCount);
	RSA(BigInt p, BigInt q);

public:
	BigInt n = 0;
	BigInt e = 0;
	uint32_t byteCount;
	static inline int checkPrimeLoops = 5;

private:
	BigInt q = 0;
	BigInt p = 0;
	BigInt phi = 0;
	BigInt d = 0;

private:
	void generatePrimes();
	void calculateNandPhi();
	void generateEncryptionKey();
	void generateDecryptionKey();
	string getKeys();
	string N();
	string E();
	string D();
	string PHI();
};