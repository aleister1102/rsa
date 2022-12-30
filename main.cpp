#include "BigInt.h"
#include "IO.h"
#include "BigIntRandom.h"
#include "Algorithm.h"
#include "RSA.h"
#include "Command.h"
#include <chrono>

using std::cout;
using std::endl;
using std::tie;
using namespace std::chrono;

vector<tuple<BigInt, BigInt>> testCases;

steady_clock::time_point start;

void startClock(steady_clock::time_point& start) {
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
	io.writeOutput("Dec: " + converter.bigIntToDecimalStr(a));
}

void randomTest() {
	BigInt n = 11;
	BigInt randNum = random.next(2, n - 2);

	bool isSmaller = randNum < n - 2;
	io.writeOutput(isSmaller ? "yes" : "no");
}

void algoTest(BigInt a, BigInt b) {
#if 0
	BigInt m = converter.decimalStrToBigInt("3332934698137425361660362443025144786509522466962699924195150122084633952149");
	BigInt res = powMod(a, b, m);
	io.writeOutputs(a, b, res, " powMod ");

	BigInt g = gcd(a, b);
	io.writeOutputs(a, b, g, " gcd ");

	BigInt g, x, y;
	std::tie(g, x, y) = extendedEuclidean(a, b);
	io.writeOutputs(a, b, g, " extended gcd ");

	BigInt res = inverseMod(a, b);
	io.writeOutputs(a, b, res, " invMod ");
#endif
}

void test()
{
	io.readInputs(testCases);

	for (int i = 0; i < testCases.size(); i++) {
		BigInt a, b;
		tie(a, b) = testCases[i];

		//operatorTest(a, b);
		//ioTest(a);
		//ioTest(b);
		algoTest(a, b);
	}
}

int main()
{
	io.clearFile("log.txt");
	io.clearFile("output.txt");

	Command::runCommandMenu();
	//RSA::test();
	//test();

	return 0;
}