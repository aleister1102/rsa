#include "Command.h"
#include "RSA.h"
#include "Converter.h"
#include "IO.h"
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

int runKeyTypeMenu()
{
	int type = 0;

	do {
		cout << "==== KEY TYPE ====\n";
		cout << "1. Generate keys\n";
		cout << "2. Use your own keys\n";
		cout << "Select key type: ";
		cin >> type;
	} while (!isValidCommand(type, 2));

	return type;
}

int runKeySizeMenu()
{
	int option = 0;
	int keySize = 0;

	do {
		cout << "\n==== KEY SIZE ====\n";
		cout << "1. 128 bit (super recommended)\n";
		cout << "2. 256 bit (recommended)\n";
		cout << "3. 512 bit\n";
		cout << "4. 1024 bit\n";
		cout << "5. 2048 bit\n";
		cout << "Select key size: ";
		cin >> option;
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

	return keySize / 8;
}

int runExportMethodMenu()
{
	int method = 0;

	do {
		cout << "\n==== EXPORT METHOD ====\n";
		cout << "1. Export keys to keys.txt\n";
		cout << "2. Display keys on console\n";
		cout << "3. Both\n";
		cout << "Select export method: ";
		cin >> method;
		system("cls");
	} while (!isValidCommand(method, 3));

	return method;
}

int runKeyFormatMenu()
{
	int option = 0;

	do {
		cout << "\n==== KEY FORMAT ====\n";
		cout << "1. Decimal\n";
		cout << "2. Binary (recommended)\n";
		cout << "Select key format: ";
		cin >> option;
		system("cls");
	} while (!isValidCommand(option, 2));

	return option;
}

int runOperationMenu()
{
	int operation = 0;

	do {
		cout << "\n==== OPERATION ====\n";
		cout << "1. Encrypt\n";
		cout << "2. Decrypt\n";
		cout << "Select operation: ";
		cin >> operation;
	} while (!isValidCommand(operation, 5));

	return operation;
}

RSA createKeys(int keyType)
{
	BigInt n, e, d;

	switch (keyType)
	{
	case 1:
	{
		int keySize = runKeySizeMenu();
		int keyFormat = runKeyFormatMenu();
		int exportMethod = runExportMethodMenu();

		RSA rsa(BigInt::maxByteCount = keySize);

		if (keyFormat == 1)
			rsa.exportKeys(10, exportMethod);
		else
			rsa.exportKeys(2, exportMethod);

		return rsa;
	}
	case 2:
	{
		string nStr, eStr, dStr;

		int keyFormat = runKeyFormatMenu();
		if (keyFormat == 1)
		{
			tie(nStr, eStr, dStr) = io.inputKeys(10);

			n = converter.decimalStrToBigInt(nStr);
			e = converter.decimalStrToBigInt(eStr);
			d = converter.decimalStrToBigInt(dStr);

			BigInt::maxByteCount = getMaxByteCount(n.byteCount, d.byteCount);
		}
		else if (keyFormat == 2)
		{
			tie(nStr, eStr, dStr) = io.inputKeys(2);

			n = converter.binaryStrToBigInt(nStr);
			e = converter.binaryStrToBigInt(eStr);
			d = converter.binaryStrToBigInt(dStr);

			BigInt::maxByteCount = getMaxByteCount(n.byteCount, d.byteCount);
		}

		RSA rsa(n, e, d);

		return rsa;
	}
	default:
		return RSA();
		break;
	}
};

void Command::run()
{
	// Lấy loại khóa (sinh ngẫu nhiên hoặc người dùng nhập)
	int keyType = runKeyTypeMenu();

	// Tạo khóa
	RSA rsa = createKeys(keyType);

	// Lấy thao tác mà người dùng muốn thực hiện
	int operation = runOperationMenu();

	switch (operation)
	{
	case 1:
	{
		string plaintText, cipherText;
		tie(plaintText, cipherText) = io.inputFilesForEncryption();

		rsa.encryptFile(plaintText, cipherText);

		break;
	}
	case 2:
	{
		string cipherText, decryptedText;
		tie(cipherText, decryptedText) = io.inputFilesForDecryption();

		rsa.decryptFile(cipherText, decryptedText);

		break;
	}
	default:
		break;
	}
}