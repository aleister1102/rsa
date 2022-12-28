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

	string decStr = converter.bigIntToDecimalStr(a);

	io.writeOutput("Dec: " + decStr);
}

void randomTest(int bitCount) {
	BigInt numA = random->next(bitCount);
	io.writeOutput(converter.bigIntToDecimalStr(numA));

	BigInt numB = random->next(numA);
	io.writeOutput(converter.bigIntToDecimalStr(numB));

	cout << (numA < numB) << endl;
}

void algoTest(BigInt a, BigInt b)
{
	BigInt res = 1;

	//res = Algorithm::gcd(a, b);
	//io.writeOutputs(a, b, res, " gcd ");

	BigInt m = converter.binaryStrToBigInt("450044376402256110986693418469");
	res = Algorithm::powMod(a, b, m);
	io.writeOutputs(a, b, res, " powMod ");
}

void testWithInputs()
{
	io.readInputs(testCases);

	for (int i = 0; i < testCases.size(); i++)
	{
		BigInt a, b;
		tie(a, b) = testCases[i];

		//operatorTest(a, b);
		//ioTest(a);
		//ioTest(b);
		//randomTest(512);
		algoTest(a, b);
	}
}

void testWithoutInputs()
{
	for (int i = 0; i < 10; i++)
	{
		BigInt res = 1;

		BigInt n = random->next(128);
		io.writeOutput(converter.bigIntToBinaryStr(n));
	}
}

int main()
{
	io.clearFile("output.txt");
	io.clearFile("log.txt");
	testWithInputs();
	//testWithoutInputs();
	return 0;
}