#include "BigIntConverter.h"

string BigIntConverter::reverseString(string littleEndian)
{
	reverse(littleEndian.begin(), littleEndian.end());
	return littleEndian;
}

string BigIntConverter::removeSpaces(string str)
{
	string result;

	for (auto c : str)
	{
		if (c != ' ')
			result += c;
	}

	return result;
}

byte BigIntConverter::stringToByte(string str)
{
	byte result = 0;

	for (char bit : str)
	{
		// Dịch trái sang một vị trí để có vị trí trống ở cuối
		result = result << 1;

		// Thêm bit vào cuối
		result += bit - '0';
	}

	return result;
}

string BigIntConverter::byteToString(byte number, bool isReversed)
{
	string result;

	for (int i = 0; i < 8; i++)
	{
		// Lấy bit thứ i
		result += to_string(number & 1);

		// Dịch dãy các bits sang một bit để lấy bit tiếp theo
		number >>= 1;
	}

	if (isReversed) result = reverseString(result);
	return result;
}

BigInt BigIntConverter::binaryStrToBigInt(string str)
{
	int length = str.length();
	int offset = length;
	int byteCount = length / 8 + (length % 8 != 0);

	BigInt result;
	result.byteCount = byteCount;
	result.bytes = (byte*)malloc(result.byteCount * sizeof(byte));

	// Tách các octets
	for (int i = 0; i < byteCount; i++)
	{
		string byteStr;

		try
		{
			if (length - 8 >= 0)
				byteStr = str.substr(offset - 8, 8);
			else
				byteStr = str.substr(0, length);

			result.bytes[i] = BigIntConverter::stringToByte(byteStr);
		}
		catch (exception e)
		{
			cout << e.what() << endl;
		}

		offset -= 8;
		length -= 8;
	}

	return result;
}

string BigIntConverter::bigIntToBinaryStr(BigInt* number)
{
	string result;

	for (int i = number->byteCount - 1; i >= 0; i--)
	{
		string str = BigIntConverter::byteToString(number->bytes[i], true);
		result += str + " ";
	}

	return result;
}