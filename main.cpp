#include "BigInt.h"
#include "BigIntIO.h"
#include "BigIntRandom.h"
#include "Algorithm.h"
#include <chrono>

using std::cout;
using std::endl;
using std::tie;
using namespace std::chrono;

vector<tuple<BigInt, BigInt>> testCases;

steady_clock::time_point start;

void startClock(steady_clock::time_point& start)
{
	start = high_resolution_clock::now();
}

void stopClock(steady_clock::time_point start) {
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	io.writeLog("Execution time: " + std::to_string(duration.count() / 10e3) + " milliseconds\n\n");
}

void operatorTest(BigInt a, BigInt b) {
#if 0
	a / b;
	a% b;
	a* b;
	a < b;
	a > b;
	a& b;
	a | b;
	a - b;
	a + b;
	a == b;
	a != b;
	a << 5;
	a >> 10;
#endif
}

void ioTest(BigInt a) {
	io.writeOutput("Bin: " + converter.bigIntToBinaryStr(a));
}

void randomTest() {
	BigInt n = 11;
	BigInt randNum = random->next(2, n - 2);

	bool isSmaller = randNum < n - 2;
	io.writeOutput(isSmaller ? "yes" : "no");
}

void algoTest(BigInt a, BigInt b)
{
	BigInt res = 1;

	//res = Algorithm::gcd(a, b);
	//io.writeOutputs(a, b, res, " gcd ");

	BigInt m = converter.decimalStrToBigInt("450044376402256110986693418469");
	res = Algorithm::powMod(a, b, m);
	io.writeOutputs(a, b, res, " powMod ");
}

void test()
{
	io.readBinaryInputs(testCases);

	for (int i = 0; i < testCases.size(); i++)
	{
		BigInt a, b;
		tie(a, b) = testCases[i];

		//operatorTest(a, b);
		//ioTest(a);
		//ioTest(b);
		//algoTest(a, b);
	}
}

void testWithoutInputs()
{
	for (int i = 0; i < 1; i++)
	{
		//randomTest();
	}

	startClock(start);

	BigInt test = converter.binaryStrToBigInt("0001001101101111101111100000111000010110001100010100101100100000010111101111111110111100100101001000001010110100110111011000001000111110111110011111001010000100001111001111001010101011010111100000101110100110011011011001001000011111011000111000110100101001000101010101010000011110110110101110001101101001011100011100101000011111010010010010100101011110101101110010010111111001110001100011010100011110001000010111001101000011110110001110001001100110001111111101011100001101011101011101010011100111110100111101100111011101110011100010100101100001111111011110001000110110001001010110001111010110010100011101010010111101110010111001000010110111010011010101101111110111001100110010011010110101011101110101100010110010011011111101011110010010110001110110011010100001011001000010111011101010101111010110100010110100110110001000000010111000110011111110111000101011000010101110000111101101011101011001000111010110010011001011100010011101001011101101111000001100001010111110110001011000010000010010010110100101"); // 1000 bit

	string isPrime = Algorithm::isPrime(test, 10) ? "yes" : "no";

	stopClock(start);

	io.writeOutput(isPrime);
}

int main()
{
	io.clearFile("output.txt");
	io.clearFile("log.txt");

	//test();
	testWithoutInputs();

	return 0;
}