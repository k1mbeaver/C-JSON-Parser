#pragma once
#include <stdlib.h> // ���� ����Ʈ ���ڿ� ��ȯ
#include <vector> // �ڵ����� �޸𸮰� �Ҵ�Ǵ� �迭
#include <tchar.h> // �ƽ�Ű �ڵ�, �����ڵ� ��ȯ�� ����
#include <map> // ������ key ��, �������� value ������ ���, key�� value�� ������ ��带 �����ؼ� ���ĵ� 'Ʈ������'�� �����صξ� Ž���ӵ��� ����
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#define _MBCS 

constexpr double CompareError = 1e-3; // constexpr = ��� ���� ������̶�� ������ִ� Ű����, ������ �ð��� ����� ����� �� �ְ� ���ش�.
inline bool CompareFloats(const double& Afloat, const double& Bfloat);

namespace namespace_myJson
{
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
		union
		{
			double dbValue;
			int nValue;
		};
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
		char* chSizeStr;
		size_t length;
	public:
		JsonString() = delete;
		JsonString(const char* whatString);
		JsonString(const JsonString& whatString);
		JsonString(JsonString&&) = delete;
		~JsonString();

		void Put(const char* whatString);
		const char* Get() const;
		size_t Length() const;

		virtual bool Equal(JsonValue* whatValue) const;
		virtual JsonValue* Clone() const;
	};

	class JsonBool : public JsonValue {
		bool boolValue;
	public:
		JsonBool() = delete;
		JsonBool(const bool& whatBoolean) noexcept;
		JsonBool(const JsonBool& whatBoolean) noexcept;
		JsonBool(JsonBool&&) = delete;

		virtual bool Equal(JsonValue* whatValue) const;
		virtual JsonValue* Clone() const;
	};

	class JsonArray : public JsonValue {
		std::vector<JsonValue*> elements;
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
		std::map<std::string, JsonValue*> properties; // string�� key(�̸�), JsonValue*�� value(��)
	public:
		JsonObject();
		JsonObject(const JsonObject& whatValue);
		JsonObject(JsonObject&&) = delete;
		~JsonObject();

		void Put(const std::string& key, JsonValue* whatValue);
		bool Has(const std::string& key) const;
		JsonValue* Get(const std::string& key) const;

		virtual bool Equal(JsonValue* whatValue) const;
		virtual JsonValue* Clone() const;
	};
	/*
	class ReadJson {
	public:
		char* JsonReader();
	};
	*/

	JsonValue* ParseBoolandNull(char* chBuffer, char*& chNext);
	JsonNum* ParseNumber(char* chBuffer, char*& chNext);
	JsonString* ParseString(char* chBuffer, char*& chNext);
	JsonArray* ParseArray(char* chBuffer, char*& chNext);
	JsonObject* ParseObject(char* chBuffer, char*& chNext);
}