#pragma once
#include <iostream>
#include <stdlib.h> // 다중 바이트 문자열 변환
#include <string.h>
#include <vector> // 자동으로 메모리가 할당되는 배열
#include <tchar.h> // 아스키 코드, 유니코드 변환을 위함
#include <string>
#include <map> // 왼쪽은 key 값, 오른쪽은 value 값으로 사용, key와 value를 가지는 노드를 생성해서 정렬된 '트리형태'로 저장해두어 탐색속도를 높임
using namespace std;

constexpr double CompareError = 1e-3; // constexpr = 어떠한 식이 상수식이라고 명시해주는 키워드, 컴파일 시간에 상수를 사용할 수 있게 해준다.
inline bool CompareFloats(const double& x, const double& y);

enum class JsonType {
	JSONNULL,
	JSONNUMBER,
	JSONSTRING,
	JSONBOOLEAN,
	JSONARRAY,
	JSONOBJECT
};

class JsonValue {
public:
	const JsonType type;
	virtual JsonValue* Clone() const = 0; //JsonValue에서 선언되고, 이제 각 자료형의 클래스에서 재정의 된다.
	virtual bool Equal(JsonValue* WhatValue) const = 0;//JsonValue에서 선언되고, 이제 각 자료형의 클래스에서 재정의 된다.
	virtual ~JsonValue() {}//JsonValue에서 선언되고, 이제 각 자료형의 클래스에서 재정의 된다.
protected:
	JsonValue(JsonType type) noexcept; // 예외를 방출하지 않을 경우에 noexcept 사용
};

//이 밑에 부터 각 자료형에 대한 클래스를 작성한다.
class JsonNull : public JsonValue {
public:
	JsonNull() noexcept;
	JsonNull(const JsonNull&) = delete;
	JsonNull(JsonNull&&) = delete;

	virtual bool Equal(JsonValue* WhatValue) const;
	virtual JsonValue* Clone() const;
};

class JsonNum : public JsonValue {
	bool isFloatPoint;
	double dbValue;
	int nValue;
public:
	JsonNum() = delete;
	JsonNum(const double& whatNum) noexcept;
	JsonNum(const int& whatNum) noexcept;
	JsonNum(const JsonNum& whatNumber) noexcept;
	JsonNum(JsonNum&&) = delete;

	void Put(const double& whatNum) noexcept;
	void Put(const int& whatNum) noexcept;
	bool IsFloatPoint() const noexcept;
	double GetAsFloat() const;
	int GetAsInt() const;

	virtual bool Equal(JsonValue* whatValue) const;
	virtual JsonValue* Clone() const;
};

class JsonString : public JsonValue {
public:
	JsonString() = delete;
	JsonString(const char* whatStr);
	JsonString(const JsonString& whatString);
	JsonString(JsonString&&) = delete;
	~JsonString();

	void Put(const char* whatStr);
	const char* Get() const;
	size_t Length() const;

	virtual bool Equal(JsonValue* whatValue) const;
	virtual JsonValue* Clone() const;
};

class JsonBool : public JsonValue {
public:
	JsonBool() = delete;
	JsonBool(const bool& whatBool) noexcept;
	JsonBool(const JsonBool& whatBoolean) noexcept;
	JsonBool(JsonBool&&) = delete;

	virtual bool Equal(JsonValue* whatValue) const;
	virtual JsonValue* Clone() const;
};

class JsonArray : public JsonValue {
	vector<JsonValue*> elements;
public:
	JsonArray();
	~JsonArray();

	JsonValue* At(const size_t& value);
	void Append(JsonValue* item);
	void Remove(JsonValue* item);
	void Clear();
	size_t Count() const;

	virtual bool Equal(JsonValue* whatValue) const;
	virtual JsonValue* Clone() const;
};

class JsonObject : public JsonValue {
	map<string, JsonValue*> properties;
public:
	JsonObject();
	JsonObject(const JsonObject& whatValue);
	JsonObject(JsonObject&&) = delete;
	~JsonObject();

	void Put(const string& key, JsonValue* whatValue);
	bool Has(const string& key) const;
	JsonValue* Get(const string& key) const;

	virtual bool Equal(JsonValue* whatValue) const;
	virtual JsonValue* Clone() const;
};

JsonValue* ParseBoolandNull(char* buffer, char*& next);
JsonNum* ParseNumber(char* buffer, char*& next);
JsonString* ParseString(char* buffer, char*& next);
JsonArray* ParseArray(char* buffer, char*& next);
JsonObject* ParseObject(char* buffer, char*& next);