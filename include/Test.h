#pragma once
#include "BigInt.h"
#include <vector>
#include <tuple>
#include <string>
#include <chrono>

using std::vector;
using std::tuple;
using std::string;

class Test
{
public:
	static void runTest(string type);

public:
	static void testOperators();
	static void testIO();
	static void testRandom();
	static void testAlgorithm();
	static void testRSA();

public:
	static void startClock();
	static void stopClock();
	static void setBase(int base);

public:
	static inline int testCount = 10;
	static inline int base = BigIntBase::BASE_2;
	static inline std::chrono::steady_clock::time_point start;
	static inline vector<tuple<BigInt, BigInt>> testCases;
};