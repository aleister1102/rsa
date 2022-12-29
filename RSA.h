#pragma once
#include "BigInt.h"

class RSA
{
private:
	BigInt q;
	BigInt p;
	BigInt phi;
	BigInt d;
public:
	BigInt n;
	BigInt e;

public:
	RSA(uint32_t byteCount);
};
