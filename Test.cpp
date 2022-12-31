#include "Test.h"
#include "IO.h"
#include "RSA.h"
#include "Converter.h"
#include "Random.h"
#include "Algorithm.h"

using std::tie;
using std::to_string;

void Test::startClock()
{
	start = std::chrono::steady_clock::now();
}

void Test::stopClock()
{
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	io.writeOutput("Execution time: " + std::to_string(duration.count() / 10e3) + " milliseconds\n\n");
}

void Test::setBase(int base)
{
	Test::base = base;
}

void Test::testOperators()
{
	BigInt a, b;
	for (tuple<BigInt, BigInt> test : testCases)
	{
		tie(a, b) = test;
#if 1
		io.writeOutputs(a, b, a - b, " - ", base);
		io.writeOutputs(a, b, a + b, " + ", base);
		io.writeOutputs(a, b, a == b, " == ", base);
		io.writeOutputs(a, b, a != b, " != ", base);
		io.writeOutputs(a, b, a < b, " < ", base);
		io.writeOutputs(a, b, a > b, " > ", base);
		io.writeOutputs(a, 10, a >> 10, " >> ", base);
		io.writeOutputs(a, 5, a << 5, " << ", base);
		io.writeOutputs(a, b, a / b, " / ", base);
		io.writeOutputs(a, b, a % b, " % ", base);
		io.writeOutputs(a, b, a * b, " * ", base);
#endif
	}
}

void Test::testIO()
{
	BigInt a, b;
	for (tuple<BigInt, BigInt> test : testCases)
	{
		tie(a, b) = test;

		io.writeOutput("Binary form of a: ", a, BigIntBase::BASE_2);
		io.writeOutput("Decimal form of a: ", a, BigIntBase::BASE_10);
		io.writeOutput("Binary form of b: ", b, BigIntBase::BASE_2);
		io.writeOutput("Decimal form of b: ", b, BigIntBase::BASE_10);
	}
}

void Test::testRandom()
{
	for (int i = 0; i < Test::testCount; i++)
	{
		BigInt firstRandomNumber = random.next(BigInt::maxByteCount);
		BigInt secondRandomNumber = random.next(2, firstRandomNumber - 2);

		io.writeOutput("First random number: ", firstRandomNumber, base);
		io.writeOutput("Second random number: ", secondRandomNumber, base);
	}
}

void Test::testAlgorithm()
{
	BigInt a, b;
	for (tuple<BigInt, BigInt> test : testCases)
	{
		tie(a, b) = test;

		BigInt m = converter.decimalStrToBigInt("93033494171281019060146861548143953189380057555617");
		BigInt g, x, y; tie(g, x, y) = extendedEuclidean(a, b);

		io.writeOutputs(a, b, powMod(a, b, m), " powMod ", base);
		io.writeOutputs(a, b, gcd(a, b), " gcd ", base);
		io.writeOutputs(a, b, g, " extended gcd ", base);
		io.writeOutputs(a, b, inverseMod(a, b), " invMod ", base);
	}
}

void Test::testRSA()
{
	if (BigInt::maxByteCount <= 0) return;

	RSA rsa(BigInt::maxByteCount);

	io.clearFile("cipher.txt");
	io.clearFile("decrypted.txt");

	rsa.encryptFile("plain.txt", "cipher.txt");
	rsa.decryptFile("cipher.txt", "decrypted.txt");
}

void Test::runTest(string type)
{
	io.clearFile("log.txt");
	io.clearFile("output.txt");
	io.readInputs(testCases, base);

	if (type == "operators")
		testOperators();
	else if (type == "random")
		testRandom();
	else if (type == "io")
		testIO();
	else if (type == "algorithm")
		testAlgorithm();
	else if (type == "rsa")
		testRSA();
}