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

	cout << "Execution time: " << duration.count() / 10e3
		<< " milliseconds\n\n";
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

		//BigInt n = random->next(MAXBYTE);
		BigInt test = converter.binaryStrToBigInt("00010101101100011000011100000101101101001110111111100010111100011001000010001101110101100010111100010000110001111001010111110010010101011000011010111100100110111001010000010011001110111100001100001100110010100001011111100110100110100111000110110011011100111110011001010110000011100001000001010101100111000111101010101010011001010010011010010000111101010010100010101010010101100110011000010111011101110000011000101110011011010010101010101011000001000110111010001000011010111010111111100110011001011001011110010001");

		startClock(start);

		string isPrime = Algorithm::isPrime(test, 10) ? "yes" : "no";

		stopClock(start);

		io.writeOutput(isPrime);
	}
}

int main()
{
	io.clearFile("output.txt");
	io.clearFile("log.txt");

	//test();
	testWithoutInputs();

	return 0;
}