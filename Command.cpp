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

	return keySize;
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

int runKeyBaseMenu()
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
	switch (keyType)
	{
	case 1:
	{
		int keySize = runKeySizeMenu();
		int keyBase = runKeyBaseMenu();
		int exportMethod = runExportMethodMenu();

		uint32_t rsaByteCount = BigInt::maxByteCount = keySize / 8;
		int rsaBase = keyBase == 1 ? BigIntBase::BASE_10 : BigIntBase::BASE_2;

		RSA rsa(rsaByteCount, rsaBase);

		rsa.exportKeys(exportMethod);

		return rsa;
	}
	case 2:
	{
		int keyBase = runKeyBaseMenu();
		int rsaBase = keyBase == 1 ? BigIntBase::BASE_10 : BigIntBase::BASE_2;

		auto keys = io.inputKeys(rsaBase);

		// Todo: thay thế các cú pháp dùng tie bằng cú pháp structured bindings (C++17)
		auto [n, e, d] = converter.toRSAKeys(keys, rsaBase);

		RSA rsa(n, e, d, rsaBase);

		return rsa;
	}
	default:
		return RSA();
		break;
	}
};

void Command::run()
{
	io.clearFile("log.txt");
	io.clearFile("output.txt");

	//? Lấy loại khóa (sinh ngẫu nhiên hoặc người dùng nhập)
	int keyType = runKeyTypeMenu();

	//? Tạo khóa
	RSA rsa = createKeys(keyType);

	//? Lấy thao tác mà người dùng muốn thực hiện
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