#include "BigInt.h"

int main(int argc, const char** argv)
{
	BigInt number = IO::inputBin(argv[1]);
	string binaryStr = IO::outputBin(number);
	cout << binaryStr << endl;

	//cout << Converter::byteToString((byte)3);

	return 0;
}