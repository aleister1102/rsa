#include "Algorithm.h"
#include "BigIntIO.h"
#include "BigIntConverter.h"
#include "BigIntRandom.h"

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

BigInt Algorithm::powMod(BigInt n, BigInt e, BigInt m)
{
	//cout << "[Algorithm::powMod] Calculate powMod\n";

	BigInt res = 1;
	n = abs(n);
	e = abs(e);

	while (e.isPositive()) {
		if (e.isOdd())
			res = (res * n) % m;

		n = (n * n) % m;
		e >>= 1;
	}

	return res;
}

bool Algorithm::isPrime(BigInt n, int k)
{
	n = abs(n);

	// Nếu là 2 hoặc 3 thì là số nguyên tố
	if (n == 2 || n == 3)
		return true;
	// Nếu nhỏ hơn hoặc bằng 1, hoặc chia hết cho 2 thì là hợp số
	if ((n <= 1) || n.isEven())
		return false;

	// Viết n - 1 dưới dạng 2^s*d bằng cách phân tích thành các nhân tử lũy thừa 2
	BigInt d = n - 1;
	BigInt s = 0;
	while (d.isEven())
	{
		d >>= 1;
		s = s + 1;
	}

	//io.writeOutput("[Algorithm::isPrime]: d = " + converter.bigIntToDecimalStr(d));

	for (int i = 1; i <= k; i = i + 1)
	{
		// Sinh số a ngẫu nhiên a trong đoạn [2, n - 2]
		BigInt a = random->next(n - 2);

		//io.writeOutput("[Algorithm::isPrime] Random a for checking: " + converter.bigIntToDecimalStr(a));

		// Nếu a và n không nguyên tố cùng nhau
		if ((gcd(a, n) != 1))
			return false;

		// Tính x = a^d mod n
		BigInt x = powMod(a, d, n);

		//io.writeOutput("[Algorithm::isPrime] a^d mod n: " + converter.bigIntToBinaryStr(x));

		// Nếu x = 1 hoặc x == n - 1 thì x không phải là cơ sở của n => không phải là hợp số
		if (x == 1 || x == n - 1)
			continue;

		for (int r = 1; r <= s - 1; r++)
		{
			x = powMod(x, 2, n);

			if (x == n - 1)
				goto LOOP;
		}

		return false;
	LOOP:
		continue;
	}

	return true;
}