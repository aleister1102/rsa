#include "RSA.h"
#include "IO.h"
#include "Command.h"
#include "Converter.h"

using std::cout;
using std::cin;
using std::endl;
using std::ios;
using std::fstream;

bool isValidCommand(int command, int commandCount)
{
	return command > 0 && command <= commandCount;
}

int printMainMenu()
{
	int command = 0;

	do {
		system("cls");
		cout << "==== Welcome to RSA Cryptosystem ====\n";
		cout << "1. Generate or insert keys\n";
		cout << "2. Encrypt or decrypt\n";
		cout << "3. Exit\n";
		cout << "Select key type: ";
		cin >> command;
		system("cls");
	} while (!isValidCommand(command, 3));

	return command;
}

int printKeyTypeMenu()
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

RSA handleKeyTypeMenu(int type)
{
	switch (type)
	{
	case 1:
	{
		int keySize = handleKeySizeMenu(printKeySizeMenu());
		int keyBase = handleKeyBaseMenu(printKeyBaseMenu());
		int exportMethod = handleExportMethodMenu(printExportMethodMenu());

		uint32_t rsaByteCount = BigInt::maxByteCount = keySize / 8;

		RSA rsa(rsaByteCount, keyBase);

		io.exportKeys(rsa, exportMethod);

		return rsa;
	}
	case 2:
	{
		int keyBase = handleKeyBaseMenu(printKeyBaseMenu());

		auto keys = io.insertKeys(keyBase);

		// Todo: thay thế các cú pháp dùng tie bằng cú pháp structured bindings (C++17)
		auto [n, e, d] = converter.toRSAKeys(keys, keyBase);

		//! Bước xác định số byte tối đa này rất quan trọng, sẽ đảm bảo chương trình chạy đúng
		BigInt::maxByteCount = getMaxByteCount(n.byteCount, d.byteCount);

		RSA rsa(n, e, d, keyBase);

		return rsa;
	}
	default:
		return RSA();
		break;
	}
};

int printKeySizeMenu()
{
	int option = 0;

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

	return option;
}

int handleKeySizeMenu(int option)
{
	int keySize = 0;

	if (option == 1)
		keySize = 128;
	else if (option == 2)
		keySize = 256;
	else if (option == 3)
		keySize = 512;
	else if (option == 4)
		keySize = 1024;
	else if (option == 5)
		keySize = 2048;
	else
		keySize = 0;

	return keySize;
}

int printKeyBaseMenu()
{
	int option = 0;

	do {
		cout << "\n==== KEY BASE ====\n";
		cout << "1. Decimal\n";
		cout << "2. Binary (recommended)\n";
		cout << "Select key base: ";
		cin >> option;
		system("cls");
	} while (!isValidCommand(option, 2));

	return option;
}

int handleKeyBaseMenu(int option)
{
	int keyBase = option == 1 ? BigIntBase::BASE_10 : BigIntBase::BASE_2;
	return keyBase;
}

int printExportMethodMenu()
{
	int option = 0;

	do {
		cout << "\n==== EXPORT METHOD ====\n";
		cout << "1. Export keys to keys.txt\n";
		cout << "2. Display keys on console\n";
		cout << "3. Both\n";
		cout << "Select export method: ";
		cin >> option;
		system("cls");
	} while (!isValidCommand(option, 3));

	return option;
}

int handleExportMethodMenu(int option)
{
	int method = 0;

	switch (option)
	{
	case 1:
		method = ExportMethod::FILE;
		break;
	case 2:
		method = ExportMethod::CONSOLE;
		break;
	case 3:
		method = ExportMethod::BOTH;
		break;
	default:
		break;
	}

	return method;
}

int printOperationMenu()
{
	int operation = 0;

	do {
		cout << "\n==== OPERATION ====\n";
		cout << "1. Encrypt\n";
		cout << "2. Decrypt\n";
		cout << "Select operation: ";
		cin >> operation;
	} while (!isValidCommand(operation, 2));

	return operation;
}

void handleOperationMenu(RSA rsa, int option)
{
	switch (option)
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

void Command::run()
{
	io.clearFile("log.txt");
	io.clearFile("output.txt");

	bool exit = false;
	RSA rsa;

	do {
		int command = printMainMenu();

		switch (command)
		{
		case 1:
		{
			rsa = handleKeyTypeMenu(printKeyTypeMenu());
		}
		break;
		case 2:
		{
			handleOperationMenu(rsa, printOperationMenu());
		}
		break;
		case 3:
		{
			exit = true;
			break;
		}
		default:
			break;
		}
	} while (exit == false);
}