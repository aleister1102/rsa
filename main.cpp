#include "BigInt.h"
#include "IO.h"
#include "BigIntRandom.h"
#include "Algorithm.h"
#include "RSA.h"
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
}

void randomTest() {
	BigInt n = 11;
	BigInt randNum = random.next(2, n - 2);

	bool isSmaller = randNum < n - 2;
	io.writeOutput(isSmaller ? "yes" : "no");
}

void algoTest(BigInt a, BigInt b)
{
	//BigInt g = gcd(a, b);
	//io.writeOutputs(a, b, g, " gcd ");

	BigInt m = converter.binaryStrToBigInt("00101100011000111111001100010000101011010010110101100010010110001101010111011111111101111101010001111101010100110100010010011101010000000111101000101000110101000000110000100011110111001100100010100110000100011011111111010001011101101010011101110000011000011101100110000010100111110100011011101110010001001100010100111000101010110010011010011101100010000110100011101100010110000011001011010010010001100110011010000001101001000100100001000110100100110011100100111101010111110010001001101001010111111100110010000101");
	BigInt res = powMod(a, b, m);
	io.writeOutputs(a, b, res, " powMod ");

	//BigInt g, x, y;
	//std::tie(g, x, y) = extendedEuclidean(a, b);
	//io.writeOutputs(a, b, g, " extended gcd ");

	//BigInt res = inverseMod(a, b);
	//io.writeOutputs(a, b, res, " invMod ");
}

void test()
{
	io.readBinaryInputs(testCases);

	for (int i = 0; i < testCases.size(); i++) {
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
	//BigInt test = converter.binaryStrToBigInt("10011011111000110011000001100000111110110011011100100011110100001011000101011100110011100001101001011100001101010000101110111011001101100100000110010001101100110110101110010110100001001001101110011111100000011111011101011010110111111100110101101001000011101100111110101010100110111000000100001010001100011011101111000111001100011111001010010111001101100000000001110001110100110011011000011000001110100011100010110101100010110000011001110110011000000001010101110111110101110110011010011010111010011111001011100001"); // 512 bit
	//string isPrime = test.isPrime(10) ? "yes" : "no";
	//io.writeOutput(isPrime);

	BigInt p = converter.binaryStrToBigInt("0000000001001000101110000010000011001001010011100000100001110001");
	BigInt q = converter.binaryStrToBigInt("000000000000001010000010000010101011110010111100101101001000011000110011");
	RSA rsa(p, q);
	rsa.encryptFile("plain.txt", "cipher.txt");
	rsa.decryptFile("cipher.txt", "decrypted_plain.txt");
}

int main()
{
	io.clearFile("output.txt");
	io.clearFile("log.txt");

	test();
	testWithoutInputs();

	return 0;
}