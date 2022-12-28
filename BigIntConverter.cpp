#include "BigIntConverter.h"
#include "BigIntIO.h"

string reverseString(string str)
{
	reverse(str.begin(), str.end());
	return str;
}

string removeSpaces(string str)
{
	string res;

	for (auto c : str)
	{
		if (c != ' ')
			res += c;
	}

	return res;
}

char intToDigit(int value)
{
	char digit;

	digit = '0' + value;

	return digit;
}

int digitToInt(char digit)
{
	int value;

	value = digit - '0';

	return value;
}

byte BigIntConverter::stringToByte(string str)
{
	byte res = 0;

	for (char bit : str)
	{
		// Dịch trái sang một vị trí để có vị trí trống ở cuối
		res = res << 1;

		// Thêm bit vào cuối
		res += digitToInt(bit);
	}

	return res;
}

string BigIntConverter::byteToString(byte n, bool isReversed)
{
	string res;

	for (int i = 0; i < 8; i++)
	{
		// Lấy bit thứ i
		res += intToDigit(n & 1);

		// Dịch dãy các bits sang một bit để lấy bit tiếp theo
		n >>= 1;
	}

	if (isReversed) res = reverseString(res);
	return res;
}

BigInt BigIntConverter::binaryStrToBigInt(string binStr)
{
	int length = binStr.length();
	int offset = length;
	int byteCount = length / 8 + (length % 8 != 0);

	BigInt res;
	res.byteCount = byteCount;
	res.bytes = (byte*)malloc(res.byteCount * sizeof(byte));

	// Tách các octets
	for (int i = 0; i < byteCount; i++)
	{
		string byteStr;

		try
		{
			// Tách các byte từ phải qua
			if (length - 8 >= 0)
				byteStr = binStr.substr(offset - 8, 8);
			else
				byteStr = binStr.substr(0, length);

			res.bytes[i] = BigIntConverter::stringToByte(byteStr);
		}
		catch (exception e)
		{
			cout << e.what() << endl;
		}

		offset -= 8;
		length -= 8;
	}

	return res;
}

string BigIntConverter::bigIntToBinaryStr(BigInt n)
{
	//removeLastBytesIfNull(n, 2);

	string res;

	for (int i = n.byteCount - 1; i >= 0; i--)
	{
		string str = BigIntConverter::byteToString(n.bytes[i], true);
		res += str + " ";
	}

	return res;
}

string BigIntConverter::bigIntToDecimalStr(BigInt n)
{
	bool sign = n.isNegative();

	BigInt i = abs(n), base = 10, r;

	string res;

	// Tương tự như phép chuyển số thập phân sang chuỗi của kiểu int:
	do {
		// Lấy i chia cho cơ số là 10
		division(i, base, i, r);

		// Đảm bảo kết quả của phép chia luôn dương
		i = abs(i);

		// Lấy giá trị của số dư (max = 9)
		int value = r.getIntValue();

		// Chuyển thành dạng chữ số
		char digit = intToDigit(value);

		// Cộng vào phía trước chuỗi
		res = digit + res;
	} while (i > 0);

	if (sign == 1) {
		res = "-" + res;
	}

	return res;
}

BigInt BigIntConverter::decimalStrToBigInt(string decStr)
{
	BigInt res = 0;

	int length = decStr.length();
	if (length == 0) return res;

	BigInt d = 0;
	BigInt i = 1;
	BigInt base = 10;

	// Tương tự như phép chuyển chuỗi sang số thập phân của kiểu int:
	// lặp qua từng chữ số d ở vị trí j (j giảm dần)
	// và cộng res cho d * i (i = 10^(length - 1 - j)).
	for (int j = length - 1; j > 0; j--)
	{
		d = digitToInt(decStr[j]);
		res = res + d * i;
		i = i * base;
	}

	// Nếu ký tự đầu là dấu trừ thì lấy bù 2
	if (decStr[0] == '-')
	{
		res = twoComplement(res);
	}
	// ngược lại thì res = res + d * i
	else
	{
		d = digitToInt(decStr[0]);
		res = res + d * i;
	}

	return res;
}