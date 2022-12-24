#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#define NBITSIZE 1234;

using namespace std;
using namespace std::chrono;

// Lớp số nguyên lớn
class BigNum
{
private:
	// Độ dài bits tối đa là 2048, tương ứng với 617 số nhưng khai báo 617*2=1234 số để thuận lợi cho việc tính toán
	// Mảng lưu các số của số nguyên lớn
	int Num[1234] = { 0 };

	// Kiểm tra số âm
	bool isNeg;

	// Vị trí bắt đầu của số n bits trong mảng Num ở trên
	int currentIndex;

public:
	// Hàm khởi tạo số nguyên lớn
	BigNum()
	{
		isNeg = false;
		currentIndex = 0;
	}

	static BigNum abs(BigNum a)
	{
		BigNum result = a;
		result.isNeg = false;
		return result;
	}

	// Radom BigNum in range [2, n - 1]
	static BigNum randomInRange(BigNum n)
	{
		int Size4096 = NBITSIZE;
		int size = rand() % (Size4096 - 1 - n.currentIndex + 1) + n.currentIndex;

		BigNum result;
		result.currentIndex = size;

		if (result.currentIndex == Size4096 - 1)
		{
			result.Num[Size4096 - 1] = rand() % (9 - 2 + 1) + 2;
		}
		else if (result.currentIndex == n.currentIndex)
		{
			do
			{
				for (int i = result.currentIndex; i < Size4096; i++)
				{
					result.Num[i] = rand() % (9 - 1 + 1) + 1;
				}
			} while (result > n - numToBigNum(2) || result == n - numToBigNum(2));
		}
		else
		{
			for (int i = result.currentIndex; i < Size4096; i++)
			{
				result.Num[i] = rand() % (9 - 1 + 1) + 1;
			}
		}

		return result;
	}

	static BigNum gcd(BigNum a, BigNum b)
	{
		int Size4096 = NBITSIZE;
		a = abs(a);
		b = abs(b);

		BigNum Zero;
		BigNum Two = numToBigNum(2);
		BigNum g = numToBigNum(1);

		Zero.setCurrentIndex();
		Two.setCurrentIndex();
		g.setCurrentIndex();

		if (a == Zero && b == Zero)
		{
			cout << "gcd(0, 0) is undefined" << endl;
			return Zero;
		}
		if (a + b == a || a + b == b)
			return a + b;

		while (a % Two == Zero && b % Two == Zero)
		{
			a = a / Two;
			b = b / Two;
			g = g * Two;
		}
		while (a > Zero)
		{
			while (a % Two == Zero)
				a = a / Two;
			while (b % Two == Zero)
				b = b / Two;
			if (a > b || a == b)
				a = (a - b) / Two;
			else
				b = (b - a) / Two;
		}

		return g * b;
	}

	static BigNum mulMod(BigNum a, BigNum b, BigNum m)
	{
		BigNum Zero;
		BigNum One = numToBigNum(1);
		BigNum Two = numToBigNum(2);

		One.setCurrentIndex();
		Two.setCurrentIndex();

		BigNum result = Zero;
		a = a % m;
		b = b % m;

		while (a > Zero)
		{
			if (a % Two == One)
			{
				result = result + b;
				result = result % m;
			}
			a = a / Two;
			b = b * Two;
			b = b % m;
		}

		return result % m;
	}

	static BigNum powerMod(BigNum b, BigNum e, BigNum m)
	{
		BigNum result = numToBigNum(1);

		BigNum Zero;
		BigNum One = numToBigNum(1);
		BigNum Two = numToBigNum(2);

		result.setCurrentIndex();
		One.setCurrentIndex();
		Two.setCurrentIndex();

		b = b % m;
		while (e > Zero)
		{
			if (e % Two == One)
			{
				result = result * b;
				result = result % m;
			}
			e = e / Two;
			b = b * b;
			b = b % m;
		}
		result.setCurrentIndex();
		return result;
	}

	static bool primalityTest(BigNum n, BigNum k)
	{
		BigNum Zero;
		BigNum One = numToBigNum(1);
		BigNum Two = numToBigNum(2);
		BigNum Three = numToBigNum(3);

		Zero.setCurrentIndex();
		One.setCurrentIndex();
		Two.setCurrentIndex();
		Three.setCurrentIndex();

		// Nếu là 2 hoặc 3 thì là số nguyên tố
		if (n == Two || n == Three)
			return true;
		// Nếu nhỏ hơn hoặc bằng 1, hoặc chia hết cho 2 thì là hợp số
		if ((n < One && n == One) || n % Two == Zero)
			return false;

		// Viết n - 1 dưới dạng 2^s*d bằng cách phân tích thành các nhân tử lũy thừa 2
		BigNum d = n - One;
		BigNum s = Zero;
		while (d % Two == Zero)
		{
			d = d / Two;
			s = s + One;
		}

		for (BigNum i = One; i < k || i == k; i = i + One)
		{
			// Sinh số a ngẫu nhiên a in [2, n - 2]
			BigNum a = randomInRange(n);

			// Nếu a và n không nguyên tố cùng nhau
			if (!(gcd(a, n) == One))
				return false;

			// Tính x = a^d mod n
			BigNum x = powerMod(a, d, n);

			// Nếu x = 1 hoặc x == n - 1 thì x không phải là cơ sở của n => không phải là hợp số
			if (x == One || x == n - One)
				continue;

			for (BigNum r = One; (r < s - One || r == s - One); r = r + One)
			{
				x = mulMod(x, x, n);

				if (x == n - One)
					goto LOOP;
			}

			return false;
		LOOP:
			continue;
		}

		return true;
	}

	static BigNum inverseMod(BigNum a, BigNum n)
	{
		BigNum One = numToBigNum(1);
		BigNum Zero;
		BigNum result, p, q, r, n_temp, a_temp, factorOfaAtR, previousFactorOfa, currentFactorOfa;

		a_temp = a;
		n_temp = n;
		previousFactorOfa = Zero;
		currentFactorOfa = One;

		while (!(a_temp == Zero) && !(a_temp == One))
		{
			q = n_temp / a_temp;

			BigNum product = q * currentFactorOfa;
			factorOfaAtR = previousFactorOfa - product;

			r = n_temp % a_temp;

			previousFactorOfa = currentFactorOfa;
			n_temp = a_temp;

			currentFactorOfa = factorOfaAtR;
			a_temp = r;
		}

		if (a_temp == Zero)
		{
			return result;
		}

		if (a_temp == One)
		{
			while (factorOfaAtR.isNeg)
			{
				factorOfaAtR = factorOfaAtR + n;
			}
			result = factorOfaAtR;
		}
		return result;
	}

	void setCurrentIndex()
	{
		int Size4096 = NBITSIZE;

		for (int i = 0; i < Size4096; i++)
		{
			if (Num[i] != 0)
			{
				currentIndex = i;
				break;
			}
		}
	}

	// Hàm chuyển đổi từ số nguyên sang số nguyên tố lơn
	static BigNum numToBigNum(long long int num)
	{
		BigNum result;
		int Size4096 = NBITSIZE;

		while (num)
		{
			result.Num[Size4096 - 1] = num % 10;
			Size4096--;
			num = num / 10;
			result.currentIndex = Size4096;
		}

		return result;
	}

	// Hàm random số nguyên lớn n bits ngẫu nhiên (số lẻ)
	static BigNum nBitsRandomNumber(int n)
	{
		BigNum randomNum;
		int nBitsSize = 0;

		switch (n)
		{
		case 512:
			nBitsSize = 155;
			break;
		case 1024:
			nBitsSize = 309;
			break;
		case 2048:
			nBitsSize = 617;
			break;
		case 4:
			nBitsSize = 60;
			break;
		default:
			break;
		}

		int Size4096 = NBITSIZE;
		int currentIndex = Size4096 - nBitsSize - 1 + 1;
		int i = Size4096 - 1;

		// Random ngược (đi từ dưới mảng số nguyên lớn đi lên)
		for (i; i >= currentIndex; i--)
		{
			int temp = rand() % (9 - 0 + 1);
			if (i == Size4096 - 1)
			{
				while (temp % 2 == 0)
				{
					temp = rand() % (9 - 0 + 1);
				}
			}
			else if (i == currentIndex)
			{
				temp = rand() % (9 - 1 + 1) + 1;
			}

			randomNum.Num[i] = temp;
		}

		randomNum.isNeg = false;
		randomNum.currentIndex = currentIndex;

		return randomNum;
	}

	// In số nguyên lớn ra màn hình
	void Print()
	{
		int Size4096 = NBITSIZE;
		if (this->isNeg)
		{
			cout << "-";
		}

		// In loại bỏ các số 0 không cần thiết

		// Nếu số đầu tiên tại vị trí bắt đầu của số nguyên lớn n bits khác 0 thì in bình thường
		if (this->Num[this->currentIndex] != 0)
		{
			for (int i = this->currentIndex; i < Size4096; i++)
			{
				cout << this->Num[i];
			}
			cout << endl;
		}
		// Ngược lại thì không in các số 0 dư thừa
		else
		{
			// Cho biết đã đến vị trí bắt đầu của số nguyên lớn hay chưa
			bool startBigNum = false;

			for (int i = this->currentIndex; i < Size4096; i++)
			{
				if (this->Num[i] != 0)
				{
					startBigNum = true;
					cout << this->Num[i];
				}
				else if (startBigNum == true)
				{
					cout << this->Num[i];
				}
			}
			if (startBigNum == false)
			{
				cout << 0;
			}
			cout << endl;
		}
	}

	// Thêm một số vào sau số nguyên lớn không thay đổi độ dài số nguyên lớn
	// Vd: 0000....45 => thêm 6 => 000....456
	void AddAfter(int value)
	{
		int Size4096 = NBITSIZE;
		BigNum Result;
		for (int i = Size4096 - 1; i > 0; i--)
		{
			Result.Num[i - 1] = this->Num[i];
		}

		Result.Num[Size4096 - 1] = value;

		*this = Result;
	}

	// Chia hai số nguyên lớn với sự chênh lệch nhỏ
	// Vd: 123124/12334, 123124/112341, 123124/1234
	tuple<BigNum, BigNum> DivSmall(const BigNum& second)
	{
		int Size4096 = NBITSIZE;
		bool isNegResult = false;

		tuple<BigNum, BigNum> result;

		BigNum tempFirst = *this;

		// Biến lưu thương số
		BigNum Quotient;
		// Biến lưu phần dư
		BigNum Remainder;

		// Khai báo số 1 trong số nguyên lớn
		BigNum one;
		one.Num[Size4096 - 1] = 1;

		// Nếu số bị chia bằng số chia
		if (*this == second)
		{
			Quotient = one;
		}
		// Nếu số bị chia lớn hơn số chia
		else if (*this > second)
		{
			// Sử dụng phép trừ cho đến khi kết quả âm
			while (!isNegResult)
			{
				tempFirst = tempFirst - second;

				isNegResult = tempFirst.isNeg;

				if (isNegResult)
				{
					// Do nothing
				}
				else
				{
					// Thương là số lần thực hiện phép trừ
					Quotient = Quotient + one;
					// Dư là kết quả của phép trừ
					// Số dư cuối cùng là kết quả của phép trừ trước khi không trừ được nữa
					Remainder = tempFirst;
				}
			}
		}
		// Nếu số bị chia nhỏ hơn số chia (dư là số bị chia, thương là 0)
		else
		{
			Remainder = tempFirst;
		}

		Quotient.setCurrentIndex();
		Remainder.setCurrentIndex();

		result = make_tuple(Quotient, Remainder);

		return result;
	}

	// Các thao tác với toán tử

	// Phép gán số nguyên lớn
	void operator=(const BigNum& bignum)
	{
		for (int i = 0; i < 1234; i++)
		{
			Num[i] = bignum.Num[i];
		}

		isNeg = bignum.isNeg;

		currentIndex = bignum.currentIndex;
	}

	bool operator==(const BigNum& bignum)
	{
		int Size4096 = NBITSIZE;

		if (this->isNeg != bignum.isNeg)
		{
			return false;
		}
		for (int i = 0; i < Size4096; i++)
		{
			if (this->Num[i] != bignum.Num[i])
			{
				return false;
			}
		}
		return true;
	}

	bool operator<(const BigNum& bignum)
	{
		int Size4096 = NBITSIZE;

		// a < 0, b > 0 => a < b
		if (this->isNeg && !bignum.isNeg)
		{
			return true;
		}

		// a > 0, b < 0 => a > b
		if (!this->isNeg && bignum.isNeg)
		{
			return false;
		}

		// a < 0, b < 0
		if (this->isNeg && bignum.isNeg)
		{
			// Xét từng số trong mảng số nguyên lớn
			for (int i = 0; i < Size4096; i++)
			{
				if (this->Num[i] > bignum.Num[i])
				{
					return true;
				}
				else if (this->Num[i] < bignum.Num[i])
				{
					return false;
				}
			}
		}
		// a > 0, b > 0
		else if (!this->isNeg && !bignum.isNeg)
		{
			for (int i = 0; i < Size4096; i++)
			{
				// Xét từng số trong mảng số nguyên lớn
				if (this->Num[i] > bignum.Num[i])
				{
					return false;
				}
				else if (this->Num[i] < bignum.Num[i])
				{
					return true;
				}
			}
		}
		return true;
	}

	// Phép so sánh lớn hơn
	bool operator>(const BigNum& bignum)
	{
		int Size4096 = NBITSIZE;

		// a < 0, b > 0 => a < b
		if (this->isNeg && !bignum.isNeg)
		{
			return false;
		}

		// a > 0, b < 0 => a > b
		if (!this->isNeg && bignum.isNeg)
		{
			return true;
		}

		// a < 0, b < 0
		if (this->isNeg && bignum.isNeg)
		{
			// Xét từng số trong mảng số nguyên lớn
			for (int i = 0; i < Size4096; i++)
			{
				if (this->Num[i] > bignum.Num[i])
				{
					return false;
				}
				else if (this->Num[i] < bignum.Num[i])
				{
					return true;
				}
			}
		}
		// a > 0, b > 0
		else if (!this->isNeg && !bignum.isNeg)
		{
			for (int i = 0; i < Size4096; i++)
			{
				// Xét từng số trong mảng số nguyên lớn
				if (this->Num[i] > bignum.Num[i])
				{
					return true;
				}
				else if (this->Num[i] < bignum.Num[i])
				{
					return false;
				}
			}
		}
		return false;
	}

	// Cộng hai số nguyên lớn - Làm như cách giải tay bình thường
	BigNum operator+(const BigNum& second)
	{
		int Size4096 = NBITSIZE;

		// Tổng
		int val = 0;
		// Phần nhớ
		int carry = 0;

		BigNum result;

		int i = Size4096 - 1;
		// a<0,b<0 => - (a+b)
		if (this->isNeg && second.isNeg)
		{
			result.isNeg = true;
		}
		// a>0,b<0
		else if (!this->isNeg && second.isNeg)
		{
			BigNum res;
			BigNum temp = second;
			temp.isNeg = false;
			// |a| > |b| => |a| - |b|
			if (*this > temp)
			{
				res = *this - temp;
				res.isNeg = false;
			}
			// |a| < |b| => - (|b| - |a|)
			else
			{
				res = temp - *this;
				res.isNeg = true;
			}
			res.setCurrentIndex();
			return res;
		}
		// a<0,b>0
		else if (this->isNeg && !second.isNeg)
		{
			BigNum res;
			BigNum temp = *this;
			temp.isNeg = false;
			// |a| < |b| => |b| - |a|
			if (*this < second)
			{
				res = (BigNum)second - temp;
				res.isNeg = false;
			}
			// |a| > |b| => - (|a| - |b|)
			else
			{
				res = temp - second;
				res.isNeg = true;
			}
			res.setCurrentIndex();
			return res;
		}
		// Chỉ cần xét xem số nào dài hơn
		if (this->currentIndex < second.currentIndex)
		{
			for (i; i >= this->currentIndex; i--)
			{
				val = this->Num[i] + second.Num[i] + carry;

				// Tính phần nhớ
				carry = val / 10;

				// Lưu phần dư
				result.Num[i] = val % 10;
			}

			if (carry != 0)
			{
				result.Num[i] = carry;
			}
		}
		else
		{
			for (i; i >= second.currentIndex; i--)
			{
				val = second.Num[i] + this->Num[i] + carry;

				// Tính phần nhớ
				carry = val / 10;

				// Lưu phần dư
				result.Num[i] = val % 10;
			}

			if (carry != 0)
			{
				result.Num[i] = carry;
			}
		}

		result.setCurrentIndex();

		return result;
	}

	// Trừ hai số nguyên lớn - Làm như cách giải tay bình thường
	BigNum operator-(const BigNum& second)
	{
		int Size4096 = NBITSIZE;
		// Hiệu
		int val = 0;
		// Phần nhớ
		int carry = 0;

		BigNum result;

		// a<0,b<0 -> -3 - (-7) => -3 + 7
		if (this->isNeg && second.isNeg)
		{
			BigNum res;
			BigNum temp = second;
			temp.isNeg = false;
			res = *this + temp;
			res.setCurrentIndex();
			return res;
		}
		// a>0,b<0 -> 3 - (-7) => 3 + 7
		else if (!this->isNeg && second.isNeg)
		{
			BigNum res;
			BigNum temp = second;
			temp.isNeg = false;
			res = *this + temp;
			res.setCurrentIndex();
			return res;
		}
		// a<0,b>0 -> - 3 - 7 => (-3)+(-7)
		else if (this->isNeg && !second.isNeg)
		{
			BigNum res;
			BigNum temp = second;
			temp.isNeg = true;
			res = *this + temp;
			res.setCurrentIndex();
			return res;
		}

		// Nếu a >= b => kết quả là a - b
		if (*this == second)
		{
			// Do nothing => result = 0;
		}
		// Cần xét số nào lớn hơn
		// Nếu a > b => kết quả là a - b
		else if (*this > second)
		{
			for (int i = Size4096 - 1; i >= this->currentIndex; i--)
			{
				val = this->Num[i] - (second.Num[i] + carry);

				if (val < 0)
				{
					carry = 1;
					result.Num[i] = val + 10;
				}
				else
				{
					carry = 0;
					result.Num[i] = val;
				}
			}
			result.isNeg = false;
		}
		// Nếu a < b => kết quả là -(b - a)
		else
		{
			for (int i = Size4096 - 1; i >= second.currentIndex; i--)
			{
				val = second.Num[i] - (this->Num[i] + carry);

				if (val < 0)
				{
					carry = 1;
					result.Num[i] = val + 10;
				}
				else
				{
					carry = 0;
					result.Num[i] = val;
				}
			}
			result.isNeg = true;
		}

		result.setCurrentIndex();

		return result;
	}

	// Nhân hai số nguyên lớn - Làm như cách giải tay bình thường
	BigNum operator*(const BigNum& second)
	{
		int Size4096 = NBITSIZE;

		BigNum result;
		bool isNegative = false;
		BigNum secondTemp = second;
		BigNum thisTemp = *this;
		int index = 0;

		if (thisTemp.isNeg || secondTemp.isNeg)
		{
			isNegative = true;
			secondTemp.isNeg = false;
			thisTemp.isNeg = false;
		}

		if (thisTemp > secondTemp)
		{
			for (int i = Size4096 - 1; i >= secondTemp.currentIndex; i--)
			{
				int val = 0;
				int carry = 0;
				BigNum product;

				int j = Size4096 - 1;

				for (j; j >= thisTemp.currentIndex; j--)
				{
					val = secondTemp.Num[i] * thisTemp.Num[j] + carry;

					carry = val / 10;

					product.Num[j - index] = val % 10;
				}

				if (carry != 0)
				{
					product.Num[j - index] = carry;
				}
				product.setCurrentIndex();

				result = result + product;

				index++;
			}
		}
		else
		{
			for (int i = Size4096 - 1; i >= thisTemp.currentIndex; i--)
			{
				int val = 0;
				int carry = 0;
				BigNum product;

				int j = Size4096 - 1;

				for (j; j >= secondTemp.currentIndex; j--)
				{
					val = thisTemp.Num[i] * secondTemp.Num[j] + carry;

					carry = val / 10;

					product.Num[j - index] = val % 10;
				}

				if (carry != 0)
				{
					product.Num[j - index] = carry;
				}
				product.setCurrentIndex();

				result = result + product;

				index++;
			}
		}

		result.setCurrentIndex();
		result.isNeg = isNegative;

		return result;
	}

	// Chia lấy nguyên
	BigNum operator/(const BigNum& second)
	{
		int Size4096 = NBITSIZE;
		bool isNegResult = false;

		BigNum tempFirst = *this;
		BigNum temp;

		BigNum Quotient;

		BigNum one;
		one.Num[Size4096 - 1] = 1;

		// a = b => thương bằng 1
		if (*this == second)
		{
			Quotient = one;
		}
		// a > b
		else if (*this > second)
		{
			int index = this->currentIndex;
			while (index != Size4096)
			{
				// Xét từng phần từ ví dụ 12312414 / 1232 như sau:
				// 1/1232 => 12/1232 => 123/1232 => 1231/1232 => 12312/1232 (thỏa điều kiện)
				// temp = 12312
				temp.AddAfter(tempFirst.Num[index]);
				temp.setCurrentIndex();
				while ((temp - second).isNeg && index != Size4096 - 1)
				{
					index++;
					temp.AddAfter(tempFirst.Num[index]);
				}

				tuple<BigNum, BigNum> divRes = temp.DivSmall(second);

				// Vị trí lưu kết quả trong mảng số nguyên lớn
				Quotient.Num[index] = get<0>(divRes).Num[Size4096 - 1];

				// temp = 1224 (12312 % 1232) => Lấy phần dư của phép tính trên
				// Sau đó tiếp tục quay lại bước đầu để cập nhật temp và thực hiện tiếp phép chia
				temp = get<1>(divRes);

				index++;
			}
		}
		// a < b => Phần nguyên bằng 0
		else
		{
			// Do nothing => Phần nguyên bằng 0
		}

		Quotient.setCurrentIndex();

		return Quotient;
	}

	// Chia lấy dư - Tương tự chia lấy nguyên nhưng lưu trữ và trả về phần dư thay vì phần nguyên
	BigNum operator%(const BigNum& second)
	{
		int Size4096 = NBITSIZE;
		bool isNegResult = false;

		BigNum tempFirst = *this;
		BigNum temp;
		BigNum Remainder;

		// a > b
		if (*this == second)
		{
			// Do nothing => Remaider = 0
		}
		else if (*this > second)
		{
			int index = this->currentIndex;
			while (index != Size4096)
			{
				// Xét từng phần từ ví dụ 12312414 / 1232 như sau:
				// 1/1232 => 12/1232 => 123/1232 => 1231/1232 => 12312/1232 (thỏa điều kiện)
				// temp = 12312
				temp.AddAfter(tempFirst.Num[index]);
				temp.setCurrentIndex();
				while ((temp - second).isNeg && index != Size4096 - 1)
				{
					index++;
					temp.AddAfter(tempFirst.Num[index]);
					temp.setCurrentIndex();
				}

				tuple<BigNum, BigNum> divRes = temp.DivSmall(second);

				// temp = 1224 (12312 % 1232) => Lấy phần dư của phép tính trên
				// Sau đó tiếp tục quay lại bước đầu để cập nhật temp và thực hiện tiếp phép chia
				temp = get<1>(divRes);

				index++;
			}

			Remainder = temp;
		}
		// a < b => Phần dư chính bằng a
		else
		{
			Remainder = tempFirst;
		}

		Remainder.setCurrentIndex();

		return Remainder;
	}
};

int main()
{
	srand(time(NULL));

	int choice = 0;
	int nbits = 0;

	cout << "--- rsa key generation program ---\n";
	cout << "1. 512 bits\n";
	cout << "2. 1024 bits\n";
	cout << "3. 2048 bits\n";
	cout << "4. test\n";

	do
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore();
			cout << "enter your choice (number only): ";
			cin >> choice;
		}
		else
		{
			cout << "enter your choice (number only): ";
			cin >> choice;
		}
	} while (choice < 1 || choice > 4);

	switch (choice)
	{
	case 1:
		nbits = 512;
		break;
	case 2:
		nbits = 1024;
		break;
	case 3:
		nbits = 2048;
		break;
	case 4:
		nbits = 4;
		break;
	default:
		break;
	}

	// auto start = high_resolution_clock::now();

	BigNum p1;
	BigNum p2;
	bool isPrimeP1 = false;
	bool isPrimeP2 = false;
	BigNum Ten = BigNum::numToBigNum(10);

	do
	{
		p1 = BigNum::nBitsRandomNumber(nbits);
		p2 = BigNum::nBitsRandomNumber(nbits);

		cout << "p = ";
		p1.Print();
		cout << "q = ";
		p2.Print();

		auto start = high_resolution_clock::now();

		// Test primality
		isPrimeP1 = BigNum::primalityTest(p1, Ten);

		auto stop = high_resolution_clock::now();

		auto duration = duration_cast<microseconds>(stop - start);

		cout << "Time taken by function: "
			<< duration.count() / 10e5 << " seconds" << endl;

		start = high_resolution_clock::now();

		// Test primality
		isPrimeP2 = BigNum::primalityTest(p2, Ten);

		stop = high_resolution_clock::now();

		duration = duration_cast<microseconds>(stop - start);

		cout << "Time taken by function: "
			<< duration.count() / 10e5 << " seconds" << endl;

		if (isPrimeP1 && isPrimeP2)
		{
			break;
		}
		else if (isPrimeP1)
		{
			do
			{
				p2 = BigNum::nBitsRandomNumber(nbits);
				isPrimeP2 = BigNum::primalityTest(p2, Ten);
			} while (!isPrimeP2);
			break;
		}
		else if (isPrimeP2)
		{
			do
			{
				p1 = BigNum::nBitsRandomNumber(nbits);
				isPrimeP1 = BigNum::primalityTest(p1, Ten);
			} while (!isPrimeP1);
			break;
		}
	} while (!isPrimeP1 && !isPrimeP2);

	// auto stop = high_resolution_clock::now();

	// auto duration = duration_cast<microseconds>(stop - start);

	// cout << "Time taken by function: "
	//      << duration.count() << " microseconds" << endl;

	cout << "p = ";
	p1.Print();
	cout << "q = ";
	p2.Print();

	BigNum One = BigNum::numToBigNum(1);
	BigNum n = p1 * p2;
	BigNum phi_n = (p1 - One) * (p2 - One);
	BigNum e;

	cout << "n = ";
	n.Print();
	cout << "phi(n) = ";
	phi_n.Print();

	do
	{
		e = BigNum::randomInRange(phi_n);
	} while (!(BigNum::gcd(e, phi_n) == One));

	BigNum d;
	BigNum de;

	cout << "e = ";
	e.Print();

	d = BigNum::inverseMod(e, phi_n);

	cout << "d = ";
	d.Print();

	return 0;
}