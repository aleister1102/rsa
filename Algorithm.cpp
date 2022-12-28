#include "Algorithm.h"
#include "BigIntIO.h"
#include "BigIntConverter.h"

using std::cout;
using std::endl;

BigInt Algorithm::gcd(BigInt a, BigInt b)
{
	BigInt r = 0;
	a = abs(a);
	b = abs(b);

	if (a == 0 && b == 0 || b == 0)
	{
		cout << "gcd(0, 0) of gcd(a, 0) is undefined" << endl;
		return r;
	}

	while ((a % b) > 0) {
		r = a % b;
		a = b;
		b = r;
	}

	return b;
}