#include "include/IO.h"
#include "include/Algorithm.h"
#include "include/Converter.h"
#include "include/Random.h"

BigInt gcd(BigInt a, BigInt b)
{
	BigInt r = 0;
	a = abs(a);
	b = abs(b);

	if (a == 0 && b == 0 || b == 0)
	{
		io.writeLog("[gcd] gcd(0, 0) or gcd(a, 0) is undefined");
		return r;
	}

	while ((a % b).isPositive()) {
		r = a % b;
		a = b;
		b = r;
	}

	return b;
}

BigInt powMod(BigInt n, BigInt e, BigInt m)
{
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

tuple<BigInt, BigInt, BigInt> extendedEuclidean(BigInt a, BigInt b)
{
	if (a == 0) {
		return std::make_tuple(b, 0, 1);
	}

	BigInt gcd, x, y;

	std::tie(gcd, x, y) = extendedEuclidean(b % a, a);

	return std::make_tuple(gcd, (y - (b / a) * x), x);
}

// Todo: hiểu cách hoạt động của thuật toán
BigInt inverseMod(BigInt a, BigInt m)
{
	BigInt m0 = m, t, q;
	BigInt x0 = 0, x1 = 1;

	if (m == 1) return 1;

	while (a > 1) {
		q = a / m;
		t = m, m = a % m, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}

	if (x1 < 0) x1 += m0;
	return x1;
}

bool millerRabinTest(BigInt n, BigInt d)
{
	//? Sinh số a ngẫu nhiên a trong nửa đoạn [2, n - 1)
	BigInt a = random.next(2, n - 1);
	//io.writeLog("[isPrime] random a for checking: " + converter.bigIntToBinaryStr(a));

	//? Nếu a và n không nguyên tố cùng nhau thì là hợp số
	if (gcd(a, n) != 1) {
		io.writeLog("[isPrime] a and n is not relative prime, so n is composite!");
		return false;
	}

	//? Tính x = a^d mod n
	BigInt x = powMod(a, d, n);
	//io.writeLog("[isPrime] a^d mod n: " + converter.bigIntToBinaryStr(x));

	//? Nếu x = 1 hoặc x == n - 1 thì x không phải là cơ sở miller-rabin (miller-rabin witness) của n => không phải là hợp số
	if (x == 1 || x == n - 1) {
		//io.writeLog("[isPrime] a^d mod n is 1 or n - 1, so n is probably prime");
		return true;
	}

	//? Kiểm tra các điều kiện của một số miller-rabin witness
	while (d != n - 1)
	{
		x = (x * x) % n;
		//io.writeLog("[isPrime] x^2 mod n: " + converter.bigIntToBinaryStr(x));

		d <<= 1;
		//io.writeLog("[isPrime] d: " + converter.bigIntToBinaryStr(d));

		if (x == n - 1)
		{
			//io.writeLog("[isPrime] x == n - 1, so n is propably prime!");
			return true;
		}
	}

	//io.writeLog("[isPrime] a is a witness of n, so n is composite!");
	return false;
}