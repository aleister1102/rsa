#include "Command.h"
#include "RSA.h"
#include "BigIntConverter.h"
#include <fstream>

using std::cout;
using std::cin;
using std::endl;
using std::ios;
using std::fstream;

bool isValidCommand(int command, int commandCount)
{
	return command > 0 && command <= commandCount;
}

void Command::runCommandMenu()
{
	int command = 0;

	do {
		cout << "==== RSA cryptosystem ====\n";
		cout << "1. Generate keys\n";
		cout << "2. Encrypt file\n";
		cout << "3. Decrypt file\n";
		cout << "Select your command: ";
		cin >> command;
		system("cls");
	} while (!isValidCommand(command, 3));

	runCommand(command);
}

int runKeySizeMenu()
{
	int option = 0;
	int keySize = 0;

	do {
		cout << "==== Key size options====\n";
		cout << "1. 128 bit (super recommended)\n";
		cout << "2. 256 bit (recommended)\n";
		cout << "3. 512 bit\n";
		cout << "4. 1024 bit\n";
		cout << "5. 2048 bit\n";
		cout << "Select your option: ";
		cin >> option;
		system("cls");
	} while (!isValidCommand(option, 5));

	if (option == 1)
		keySize = 128;
	else if (option == 2)
		keySize = 256;
	else if (option == 3)
		keySize = 512;
	else if (option == 4)
		keySize = 1024;
	else
		keySize = 2048;

	return keySize;
}

int runExportMethodMenu()
{
	int option = 0;

	do {
		cout << "==== Export method ====\n";
		cout << "1. Export keys to keys.txt\n";
		cout << "2. Display keys on console\n";
		cout << "3. Both\n";
		cout << "Select your option: ";
		cin >> option;
		system("cls");
	} while (!isValidCommand(option, 3));

	return option;
}

int runKeyTypeMenu()
{
	int option = 0;

	do {
		cout << "==== Choose type of key ====\n";
		cout << "1. Type your own key\n";
		cout << "2. Generate new key\n";
		cout << "Select your option: ";
		cin >> option;
		system("cls");
	} while (!isValidCommand(option, 2));

	return option;
}

int runKeyFormatMenu()
{
	int option = 0;

	do {
		cout << "==== Choose key format ====\n";
		cout << "1. Binary (recommended)\n";
		cout << "2. Decimal\n";
		cout << "Select your option: ";
		cin >> option;
		system("cls");
	} while (!isValidCommand(option, 2));

	return option;
}

void Command::runCommand(int command)
{
	switch (command)
	{
	case 1:
	{
		int keySize = runKeySizeMenu();
		int exportMethod = runExportMethodMenu();

		uint32_t maxByteCount = keySize / 8;
		BigInt::maxByteCount = maxByteCount;
		RSA rsa(maxByteCount);
		rsa.exportKeys(exportMethod);

		break;
	}
	case 2:
	{
		int keyFormat = runKeyFormatMenu();

		string nStr; cout << "Enter public key n: "; cin >> nStr;
		string eStr; cout << "Enter public key d: "; cin >> eStr;
		string plainTextFile; cout << "Enter plain text file: "; cin >> plainTextFile;
		string cipherTextFile; cout << "Enter cipher text file: "; cin >> cipherTextFile;

		BigInt n, e;
		if (keyFormat == 1)
		{
			// TODO: cần set maxByteCount khi đọc được chuỗi
			n = converter.binaryStrToBigInt(nStr);
			e = converter.binaryStrToBigInt(eStr);
		}
		else if (keyFormat == 2)
		{
			n = converter.decimalStrToBigInt(nStr);
			e = converter.decimalStrToBigInt(eStr);
		}

		RSA::encryptFile(plainTextFile, n, e, cipherTextFile);

		break;
	}
	case 3:
	{
		int keyType = runKeyTypeMenu();
		int keyFormat = runKeyFormatMenu();

		if (keyType == 1)
		{
			string nStr; cout << "Enter n: "; cin >> nStr;
			string dStr; cout << "Enter d: "; cin >> dStr;
			string cipherTextFile; cout << "Enter cipher text file: "; cin >> cipherTextFile;
			string plainTextFile; cout << "Enter plain text file: "; cin >> plainTextFile;

			BigInt n, d;
			if (keyFormat == 1)
			{
				n = converter.binaryStrToBigInt(nStr);
				d = converter.binaryStrToBigInt(dStr);
			}
			else if (keyFormat == 2)
			{
				n = converter.decimalStrToBigInt(nStr);
				d = converter.decimalStrToBigInt(dStr);
			}

			RSA::decryptFile(cipherTextFile, n, d, plainTextFile);
		}
		else if (keyType == 2)
		{
			int keySize = runKeySizeMenu();
			uint32_t maxByteCount = keySize / 8;
			BigInt::maxByteCount = maxByteCount;
			RSA rsa(maxByteCount);

			string cipherTextFile; cout << "Enter cipher text file: "; cin >> cipherTextFile;
			string plainTextFile; cout << "Enter plain text file: "; cin >> plainTextFile;

			RSA::decryptFile(cipherTextFile, rsa.n, rsa.getD(), plainTextFile);
		}

		break;
	}
	default:
		break;
	}
};