#include "json.h"
#include <stdexcept> // logic_error, runtime_error 클래스 정의
#include <string.h>
using namespace namespace_myJson;
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

bool JsonNull::Equal(JsonValue* WhatValue) const // 함수 뒤에 const = 이 함수 안에서는 어떤 변수도 바꿀수 없음
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

JsonNum::JsonNum(const double& WhatNum) noexcept : JsonValue(JsonType::JSONNUMBER)
{
	isFloatPoint = true;
	dbValue = WhatNum;
}

JsonNum::JsonNum(const int& WhatNum) noexcept : JsonValue(JsonType::JSONNUMBER)
{
	isFloatPoint = false;
	nValue = WhatNum;
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

	return CompareFloats(GetAsFloat(), reinterpret_cast<JsonNum*>(WhatValue)->GetAsFloat()); // reinterpret_cast = 임의의 포인터 타입끼리 변환을 허용하는 캐스트 연산자
}

JsonValue* JsonNum::Clone() const
{
	return new JsonNum(*this);
}

JsonString::JsonString(const char* whatString) : JsonValue(JsonType::JSONSTRING)
{
	if (whatString == nullptr)
		throw std::runtime_error("문자열이 NULL을 가리킵니다.");
	length = strlen(whatString);
	char* sizeCopied = new char[length + 1];
	if (strcpy_s(sizeCopied, length + 1, whatString) != 0)
	{
		delete[] sizeCopied;
		throw std::runtime_error("문자열 복사 실패");
	}

	sizeCopied[length] = '\0';
	chSizeStr = sizeCopied;
}

JsonString::JsonString(const JsonString& whatString) : JsonValue(JsonType::JSONSTRING)
{
	length = whatString.length;
	char* sizeCopied = new char[length + 1];
	if (strcpy_s(sizeCopied, length + 1, whatString.chSizeStr) != 0)
	{
		delete[] sizeCopied;
		throw std::runtime_error("문자열 복사 실패");
	}

	sizeCopied[length] = '\0';
	chSizeStr = sizeCopied;
}

JsonString::~JsonString()
{
	delete[] chSizeStr;
}

void JsonString::Put(const char* whatString)
{
	if (whatString == nullptr)
		throw std::runtime_error("문자열이 NULL을 가리킵니다.");
	length = strlen(whatString);
	char* sizeCopied = new char[length + 1];
	if (strcpy_s(sizeCopied, length, whatString) != 0)
	{
		delete[] sizeCopied;
		throw std::runtime_error("문자열 복사 실패");
	}

	sizeCopied[length] = '\0';
	delete[] chSizeStr;
	chSizeStr = sizeCopied;
}

const char* JsonString::Get() const
{
	return chSizeStr;
}

size_t JsonString::Length() const
{
	return length;
}

bool JsonString::Equal(JsonValue* whatValue) const
{
	if (this == whatValue)
		return true;
	if (type != whatValue->type)
		return false;

	auto NextValue = reinterpret_cast<JsonString*>(whatValue);
	if (length != NextValue->length)
		return false;
	return strcmp(chSizeStr, NextValue->chSizeStr) == 0;
}

JsonValue* JsonString::Clone() const
{
	return new JsonString(*this);
}

JsonBool::JsonBool(const bool& whatBoolean) noexcept : JsonValue(JsonType::JSONBOOLEAN)
{
	boolValue = whatBoolean;
}

JsonBool::JsonBool(const JsonBool& whatBoolean) noexcept : JsonValue(JsonType::JSONBOOLEAN)
{
	boolValue = whatBoolean.boolValue;
}

bool JsonBool::Equal(JsonValue* whatBoolean) const
{
	if (this == whatBoolean)
		return true;
	if (type != whatBoolean->type)
		return false;

	return boolValue == reinterpret_cast<JsonBool*>(whatBoolean)->boolValue;
}

JsonValue* JsonBool::Clone() const
{
	return new JsonBool(*this);
}

JsonArray::JsonArray() : JsonValue(JsonType::JSONARRAY)
{
}

JsonArray::~JsonArray()
{
	for (auto arrayElements : elements)
	{
		delete arrayElements;
	}
}

JsonValue* JsonArray::At(const size_t& value)
{
	// 범위를 벗어난 값
	return elements.at(value); // at = vector, index번째 요소 접근 (범위 검사함)
}

void JsonArray::Append(JsonValue* item)
{
	elements.push_back(item->Clone()); // 추가, push_back = vector
}

void JsonArray::Remove(JsonValue* item)
{
	for (auto it = elements.begin(); it != elements.end();) // 처음과 끝까지
	{
		if ((*it)->Equal(item))
		{
			delete* it;
			elements.erase(++it);
		}
		else
			++it;
	}
}

void JsonArray::Clear()
{
	elements.clear();
}

size_t JsonArray::Count() const
{
	return elements.size();
}

bool JsonArray::Equal(JsonValue* whatValue) const
{
	if (this == whatValue)
		return true;
	if (type != whatValue->type)
		return false;

	auto WhatValue = reinterpret_cast<JsonArray*>(whatValue);
	const size_t length = Count();
	if (length != WhatValue->Count())
		return false;

	for (size_t index = 0; index < length; ++index)
	{
		if (!elements[index]->Equal(WhatValue->elements[index]))
			return false;
	}

	return true;
}

JsonValue* JsonArray::Clone() const
{
	JsonArray* array = new JsonArray();
	for (auto whatArray : elements)
	{
		array->Append(whatArray);
	}

	return array;
}

JsonObject::JsonObject() : JsonValue(JsonType::JSONOBJECT)
{
}

JsonObject::JsonObject(const JsonObject& whatObject) : JsonValue(JsonType::JSONOBJECT)
{
	for (auto it : whatObject.properties) // :는 auto 키워드와 관련
	{
		it.second = it.second->Clone(); // second는 map과 관련 first = key, second = value
		properties.insert(it);
	}
}

JsonObject::~JsonObject()
{
	for (auto it : properties)
	{
		delete it.second;
	}
}

void namespace_myJson::JsonObject::Put(const std::string& key, JsonValue* whatValue)
{
	auto it = properties.find(key);
	if (it != properties.end())
	{
		delete it->second;
	}
	properties.insert(make_pair(key, whatValue));
}

bool JsonObject::Has(const string& key) const
{
	return properties.find(key) != properties.end();
}

JsonValue* JsonObject::Get(const string& key) const
{
	auto it = properties.find(key);
	if (it == properties.end())
		return nullptr;
	return it->second;
}

bool JsonObject::Equal(JsonValue* whatValue) const
{
	if (this == whatValue)
		return true;
	if (type != whatValue->type)
		return false;

	auto WhatValue = reinterpret_cast<JsonObject*>(whatValue);
	if (properties.size() != WhatValue->properties.size())
		return false;

	return false;
}

JsonValue* JsonObject::Clone() const
{
	return new JsonObject(*this);
}

string ParseKey(char* chBuffer, char*& chNext)
{
	char* chEnd = std::strchr(chBuffer, '"');
	if (chEnd == NULL)
		throw runtime_error("문자열의 끝을 찾을 수 없습니다.");
	auto ofx = chEnd - chBuffer;
	chNext = chEnd + 1;
	return string(chBuffer, ofx);
}

char* SkipSpaces(char* chBuffer)
{
	if (chBuffer == NULL || nullptr)
	{
		return NULL;
	}

	while (*chBuffer != '\0')
	{
		switch (*chBuffer)
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			chBuffer++;
			break;
		default:
			return chBuffer;
		}
	}

	return NULL;
}

JsonValue* namespace_myJson::ParseBoolandNull(char* chBuffer, char*& chNext)
{
	JsonValue* whatValue = nullptr;

	if (std::strncmp(chBuffer, "true", 4) == 0)
	{
		whatValue = new JsonBool(true);
		chNext = chBuffer + 4;
	}

	else if (std::strncmp(chBuffer, "false", 5) == 0)
	{
		whatValue = new JsonBool(false);
		chNext = chBuffer + 5;
	}

	else if (std::strncmp(chBuffer, "null", 4) == 0)
	{
		whatValue = new JsonNull();
		chNext = chBuffer + 4;
	}

	else
	{
		throw runtime_error("존재하지 않습니다.");
	}

	return whatValue;
}

JsonNum* namespace_myJson::ParseNumber(char* chBuffer, char*& chNext)
{
	string number;
	while (*chBuffer != '\0')
	{
		if (!(isdigit(*chBuffer) || *chBuffer == '.'))
			break;
		number.push_back(*(chBuffer++));
	}

	auto findIndex = number.find_first_of('.'); // find_first_of = 전달된 문자들 중 첫 번쨰로 일치하는 것의 위치를 찾는다.

	if (findIndex != string::npos) // string::npos = 찾는 문자열이 없는 경우에는 string npos를 반환한다.
	{
		if (findIndex != number.find_last_of('.')) // 2개의 점을 가졌을 떄
			throw runtime_error("존재하지 않습니다.");
		try
		{
			double dbValue = stof(number);
			chNext = chBuffer;
			return new JsonNum(dbValue);
		}

		catch (std::exception e)
		{
			throw runtime_error("존재하지 않습니다.");
		}
	}

	else
	{
		try
		{
			int nValue = std::stoi(number);
			chNext = chBuffer;
			return new JsonNum(nValue);
		}

		catch (std::exception e)
		{
			throw runtime_error("존재하지 않습니다.");
		}
	}
}

JsonString* namespace_myJson::ParseString(char* chBuffer, char*& chNext)
{
	auto str = ParseKey(chBuffer, chNext);
	return new JsonString(str.c_str());
}

JsonArray* namespace_myJson::ParseArray(char* chBuffer, char*& chNext)
{
	auto obj = new JsonArray();
	chBuffer = SkipSpaces(chBuffer + 1);
	while (*chBuffer != ']' && *chBuffer != '\0')
	{
		try
		{
			if (*chBuffer == '{')
			{
				auto whatValue = ParseObject(chBuffer, chBuffer);
				obj->Append(whatValue);
			}
			else if (*chBuffer == '"')
			{
				auto whatValue = ParseString(chBuffer + 1, chBuffer);
				obj->Append(whatValue);
			}
			else if (isdigit(*chBuffer))
			{
				auto whatValue = ParseNumber(chBuffer, chBuffer);
				obj->Append(whatValue);
			}
			else if (*chBuffer == '[')
			{
				auto whatValue = ParseArray(chBuffer, chBuffer);
				obj->Append(whatValue);
			}
			else if (isalpha(*chBuffer))
			{
				auto whatValue = ParseBoolandNull(chBuffer, chBuffer);
				obj->Append(whatValue);
			} 
			else
			{
				throw runtime_error("완성되지 않았습니다.");
			}
		}

		catch (exception e)
		{
			delete obj;
			throw e;
		}

		chBuffer = SkipSpaces(chBuffer);
		if (*chBuffer == ',')
		{
			chBuffer++;
			chBuffer = SkipSpaces(chBuffer);
		}
	}

	chNext = chBuffer;
	return obj;
}

JsonObject* namespace_myJson::ParseObject(char* chBuffer, char*& chNext)
{
	auto const original = chBuffer;
	auto obj = new JsonObject();
	bool parsekey = true;
	string key;
	chBuffer = SkipSpaces(chBuffer);
	if (*chBuffer != '{')
	{
		delete obj;
		throw runtime_error("No Start");
	}
	chBuffer++;
	while ((*chBuffer != '}' && *chBuffer != '\0'))
	{
		if (parsekey)
		{
			if (*chBuffer == '"')
			{
				key = ParseKey(chBuffer + 1, chBuffer);
				chBuffer = std::strchr(chBuffer, ':'); // strchr : 문자검색
				chBuffer = SkipSpaces(chBuffer);

				if (chBuffer == NULL)
				{
					delete obj;
					throw runtime_error("Can't find end of token");
				}
				if (*chBuffer != ':')
				{
					delete obj;
					throw runtime_error("Can't find end of key");
				}

				chBuffer = SkipSpaces(++chBuffer);
				parsekey = false;
			}
			else
			{
				chBuffer = SkipSpaces(chBuffer);
			}
		}
		else
		{
			try
			{
				if (*chBuffer == '{')
				{
					auto whatValue = ParseObject(chBuffer, chBuffer);
					obj->Put(key, whatValue);
				}
				else if (*chBuffer == '\"')
				{
					auto whatValue = ParseString(chBuffer + 1, chBuffer);
					obj->Put(key, whatValue);
				}
				else if (isdigit(*chBuffer))
				{
					auto whatValue = ParseNumber(chBuffer, chBuffer);
					obj->Put(key, whatValue);
				}
				else if (*chBuffer == '[')
				{
					auto whatValue = ParseArray(chBuffer, chBuffer);
					obj->Put(key, whatValue);
				}
				else if(isalpha(*chBuffer))
				{
					auto whatValue = ParseBoolandNull(chBuffer, chBuffer);
					obj->Put(key, whatValue);
				}
				else
				{
					throw runtime_error("Uncompleted Object");
				}
			}
			catch (exception e)
			{
				delete obj;
				throw e;
			}
			chBuffer = SkipSpaces(chBuffer);
			if (*chBuffer == ',')
			{
				chBuffer++;
				parsekey = true;
				chBuffer = SkipSpaces(chBuffer);
			}
			else
			{
				break;
			}
		}
	}
	chNext = chBuffer + 1;
	return obj;
}
/*
char* namespace_myJson::ReadJson::JsonReader()
{
	string str;
	char chStr[500];
	ifstream ist("test.json");
	for (char chP; ist >> chP;)
	{
		str += chP;
	}

	strcpy_s(chStr, str.c_str());

	return chStr;
}
*/

