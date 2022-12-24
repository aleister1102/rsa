#include <ctime>
#include <iostream>
#include <tuple>
using namespace std;

typedef int BigInt;

unsigned long long llrand()
{
	unsigned long long r = 0;

	for (int i = 0; i < 5; ++i)
	{
		r = (r << 15) | (rand() & 0x7FFF);
	}

	return r & 0xFFFFFFFFFFFFFFFFULL;
}

BigInt gcd(int a, int b)
{
	a = abs(a);
	b = abs(b);
	int g = 1;

	if (a == 0 && b == 0)
	{
		cout << "gcd(0, 0) is undefined" << endl;
		return 0;
	}
	if (a + b == a || a + b == b)
		return a + b;

	while (a % 2 == 0 && b % 2 == 0)
	{
		a /= 2;
		b /= 2;
		g *= 2;
	}
	while (a > 0)
	{
		while (a % 2 == 0)
			a /= 2;
		while (b % 2 == 0)
			b /= 2;
		if (a >= b)
			a = (a - b) / 2;
		else
			b = (b - a) / 2;
	}

	return g * b;
}

BigInt mulMod(BigInt a, BigInt b, BigInt m)
{
	BigInt x = 0, y = a % m;

	while (b > 0)
	{
		if (b % 2 == 1)
			x = (x + y) % m;
		y = (y * 2) % m;
		b /= 2;
	}

	return x % m;
}

BigInt powerMod(BigInt a, BigInt x, BigInt m)
{
	BigInt r = 1;

	while (x)
	{
		if ((x & 1) == 1)
		{
			r = a * r;
			r = r % m;
		}

		a = a * a;
		a = a % m;
		x = x >> 1;
	}

	return r;
}

tuple<BigInt, BigInt> factorPowersOfTwo(BigInt n)
{
	BigInt s = 0, d = 0;

	while (n % 2 == 0)
	{
		s += 1;
		n /= 2;
	}
	d = n;

	return make_tuple(s, d);
}

bool primalityTest(BigInt n, BigInt k)
{
	if (n < 2)
		return false;
	if (n != 2 && n % 2 == 0)
		return false;

	BigInt d = n - 1;
	while (d % 2 == 0)
	{
		d /= 2;
	}

	for (BigInt i = 1; i <= k; i++)
	{
		// Random a in [2, n - 2]
		BigInt a = rand() % (n - 1) + 1;

		// x = a^d mod n
		BigInt x = powerMod(a, d, n);

		BigInt temp = d;
		while (temp != n - 1 && x != 1 && x != n - 1)
		{
			x = mulMod(x, x, n);
			temp *= 2;
		}
		if (x != n - 1 && temp % 2 == 0)
		{
			return false;
		}
	}

	return true;
}

//BigInt main()
//{
//	srand(time(NULL));
//
//	// // Generate random test number
//	// int n;
//	// do
//	// {
//	//     n = llrand();
//	// } while (n < 0);
//
//	// cout << "Number: " << n << endl;
//	// cout << "Result: " << primalityTest(n, 10) << endl;
//
//	cout << powerMod(2, 7, 5) << endl;
//
//	return 0;
//}