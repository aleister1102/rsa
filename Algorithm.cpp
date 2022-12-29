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
	//io.writeLog("[Algorithm::powMod] Calculate powMod");

	BigInt res = 1;
	n = abs(n);
	e = abs(e);

	while (e.isPositive()) {
		if (e.isOdd())
			res = (res * n) % m;

		n = (n * n) % m;
		e >>= 1;
	}

	//io.writeLog("[Algorithm::powMod] Finish calculating powMod");
	return res;
}

// BUG: thuật toán có vẻ chưa hoạt động đúng
bool Algorithm::millerRabinTest(BigInt n, BigInt d)
{
	// Sinh số a ngẫu nhiên a trong nửa đoạn [2, n - 1)
	BigInt a = random->next(2, n - 1);

	io.writeLog("[Algorithm::isPrime] Random a for checking: " + converter.bigIntToBinaryStr(a));

	// Trường hợp nếu a và n không nguyên tố cùng nhau
	//if ((gcd(a, n) != 1))
	//	return false;

	// Tính x = a^d mod n
	BigInt x = powMod(a, d, n);

	io.writeLog("[Algorithm::isPrime] a^d mod n: " + converter.bigIntToBinaryStr(x));

	// Nếu x = 1 hoặc x == n - 1 thì x không phải là cơ sở miller-rabin (miller-rabin witness) của n => không phải là hợp số
	if (x == 1 || x == n - 1) {
		io.writeLog("[Algorithm::isPrime] a^d mod n is 1 or n - 1, so it is probably prime");
		return true;
	}

	// Kiểm tra các điều kiện của một số miller-rabin witness
	while (d != n - 1)
	{
		x = (x * x) % n;
		io.writeLog("[Algorithm::isPrime] x^2 mod n: " + converter.bigIntToBinaryStr(x));

		d <<= 1;
		io.writeLog("[Algorithm::isPrime] d: " + converter.bigIntToBinaryStr(d));

		if (x == n - 1)
		{
			io.writeLog("[Algorithm::isPrime] x == n - 1, so it propably prime!");
			return true;
		}
		if (x == 1) return false;
	}

	io.writeLog("[Algorithm::isPrime] find a witness of x, so it is composite!");
	return false;
}

bool Algorithm::isPrime(BigInt n, int k)
{
	//! Nếu như kích thước byte là MAXBYTE thì cấp phát thêm một byte để đảm bảo n là số dương
	//! Không lấy abs vì có thể bị sai (tràn số)
	if (n.byteCount == MAXBYTE) addMoreBytes(n, 1);

	io.writeLog("[Algorithm::isPrime] n = " + converter.bigIntToBinaryStr(n));

	// Các trường hợp đơn giản
	if (n <= 1 || n == 4)  return false;
	if (n <= 3) return true;

	// Viết n - 1 dưới dạng 2^s*d bằng cách phân tích thành các nhân tử lũy thừa 2
	BigInt d = n - 1;
	while (d.isEven())
	{
		d >>= 1;
	}

	io.writeLog("[Algorithm::isPrime] d = " + converter.bigIntToBinaryStr(d));

	for (int i = 1; i <= k; i++)
	{
		io.writeLog("[Algorithm::isPrime] Test: " + std::to_string(i));

		if (millerRabinTest(n, d) == false)
			return false;
	}

	return true;
}