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

byte BigIntConverter::stringToByte(string str)
{
	byte res = 0;

	for (char bit : str)
	{
		// Dịch trái sang một vị trí để có vị trí trống ở cuối
		res = res << 1;

		// Thêm bit vào cuối
		res += bit - '0';
	}

	return res;
}

string BigIntConverter::byteToString(byte n, bool isReversed)
{
	string res;

	for (int i = 0; i < 8; i++)
	{
		// Lấy bit thứ i
		res += to_string(n & 1);

		// Dịch dãy các bits sang một bit để lấy bit tiếp theo
		n >>= 1;
	}

	if (isReversed) res = reverseString(res);
	return res;
}

BigInt BigIntConverter::binaryStrToBigInt(string str)
{
	int length = str.length();
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
				byteStr = str.substr(offset - 8, 8);
			else
				byteStr = str.substr(0, length);

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

void insertCharFrontStr(char** str, int* strLen, char chr) {
	(*strLen)++;
	(*str) = (char*)realloc((*str), (*strLen) + 1);
	memcpy_s((*str) + 1, (*strLen), (*str), (*strLen));
	(*str)[0] = chr;
}

char intToDigit(int32_t value)
{
	char digit;

	digit = '0' + value;

	return digit;
}

string BigIntConverter::bigIntToDecimalStr(BigInt n)
{
	bool sign = n.isNegative();

	//? Tại sao lại cần lấy giá trị tuyệt đối của n
	BigInt i = abs(n), base = 10, r;

	string res;

	do {
		//* Lấy i chia cho cơ số là 10
		division(i, base, i, r);

		//* Lấy số dư, số dư lớn nhất là 9
		int32_t value = getValue(r);

		//* Chuyển thành dạng chữ số
		char digit = intToDigit(value);

		//* Cộng vào phía trước chuỗi
		res = digit + res;
	} while (i > 0);

	if (sign == 1) {
		res = "-" + res;
	}

	return res;
}