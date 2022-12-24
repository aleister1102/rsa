#include "BigInt.h"

int main()
{
	int byteCount = 0;
	byte* bytes = IO::inputBin(byteCount);
	IO::outputBin(bytes, byteCount);

	return 0;
}