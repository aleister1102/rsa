#include "include/BigInt.h"
#include "include/IO.h"
#include "include/Random.h"
#include "include/Algorithm.h"
#include "include/Converter.h"
#include "include/RSA.h"
#include "include/Test.h"
#include "include/Command.h"

int main()
{
	//Test::setBase(BigIntBase::BASE_10);
	//Test::runTest("rsa");

	Command::run();

	return 0;
}