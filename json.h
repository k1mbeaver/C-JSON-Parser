#pragma once
#include <iostream>
#include <stdlib.h> // ���� ����Ʈ ���ڿ� ��ȯ
#include <string.h>
#include <vector> // �ڵ����� �޸𸮰� �Ҵ�Ǵ� �迭
#include <tchar.h> // �ƽ�Ű �ڵ�, �����ڵ� ��ȯ�� ����
#include <string>
#include <map> // ������ key ��, �������� value ������ ���, key�� value�� ������ ��带 �����ؼ� ���ĵ� 'Ʈ������'�� �����صξ� Ž���ӵ��� ����
using namespace std;

constexpr double CompareError = 1e-3; // constexpr = ��� ���� ������̶�� ������ִ� Ű����, ������ �ð��� ����� ����� �� �ְ� ���ش�.
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
	virtual JsonValue* Clone() const = 0; //JsonValue���� ����ǰ�, ���� �� �ڷ����� Ŭ�������� ������ �ȴ�.
	virtual bool Equal(JsonValue* WhatValue) const = 0;//JsonValue���� ����ǰ�, ���� �� �ڷ����� Ŭ�������� ������ �ȴ�.
	virtual ~JsonValue() {}//JsonValue���� ����ǰ�, ���� �� �ڷ����� Ŭ�������� ������ �ȴ�.
protected:
	JsonValue(JsonType type) noexcept; // ���ܸ� �������� ���� ��쿡 noexcept ���
};

//�� �ؿ� ���� �� �ڷ����� ���� Ŭ������ �ۼ��Ѵ�.
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