#include "BigInt.h"
#include "IO.h"
#include "Random.h"
#include "Algorithm.h"
#include "Converter.h"
#include "RSA.h"
#include "Test.h"
#include "Command.h"
#include <chrono>

int main()
{
	Test::setBase(BigIntBase::BASE_10);
	Test::runTest("rsa");

	return 0;
}