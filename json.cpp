#include "json.h"
#include <stdexcept> // logic_error, runtime_error 클래스 정의
#include <string.h>
using namespace std;

inline bool CompareFloats(const double& Afloat, const double& Bfloat)
{
	return abs(Afloat - Bfloat) <= CompareError;
}

JsonValue::JsonValue(JsonType type) noexcept : type(type)
{
}

JsonNull::JsonNull() noexcept : JsonValue(JsonType::JSONNULL)
{
}

bool JsonNull::Equal(JsonValue* WhatValue) const
{
	if (this == WhatValue)
		return true;
	if (type == WhatValue->type)
		return true;

	return false;
}

JsonValue* JsonNull::Clone() const
{
	return new JsonNull();
}

JsonNum::JsonNum(const int& WhatNum) noexcept : JsonValue(JsonType::JSONNUMBER)
{
	isFloatPoint = true;
	dbValue = WhatNum;
}

JsonNum::JsonNum(const JsonNum& WhatNum) noexcept : JsonValue(JsonType::JSONNUMBER)
{
	isFloatPoint = WhatNum.isFloatPoint;
	if (isFloatPoint)
		dbValue = WhatNum.dbValue;
	else
		nValue = WhatNum.nValue;
}

void JsonNum::Put(const double& WhatNum) noexcept
{
	isFloatPoint = true;
	dbValue = WhatNum;
}

void JsonNum::Put(const int& WhatNum) noexcept
{
	isFloatPoint = false;
	nValue = WhatNum;
}

bool JsonNum::IsFloatPoint() const noexcept
{
	return isFloatPoint;
}

double JsonNum::GetAsFloat() const
{
	if (isFloatPoint)
		return dbValue;
	return static_cast<double>(nValue);
}

int JsonNum::GetAsInt() const
{
	if (isFloatPoint)
		return static_cast<int>(dbValue);
	return nValue;
}

bool JsonNum::Equal(JsonValue* WhatValue) const
{
	if (this == WhatValue)
		return true;
	if (type != WhatValue->type)
		return false;

	return CompareFloats(GetAsFloat(), reinterpret_cast<JsonNum*>(WhatValue)->GetAsFloat());
}

JsonValue* JsonNum::Clone() const
{
	return new JsonNum(*this);
}

//String 진행