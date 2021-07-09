#pragma once
#include <iostream>
#include <stdlib.h> // ���� ����Ʈ ���ڿ� ��ȯ
#include <string.h>
#include <vector> // �ڵ����� �޸𸮰� �Ҵ�Ǵ� �迭
#include <tchar.h> // �ƽ�Ű �ڵ�, �����ڵ� ��ȯ�� ����
#include <string>
#include <map> // ������ key ��, �������� value ������ ���, key�� value�� ������ ��带 �����ؼ� ���ĵ� 'Ʈ������'�� �����صξ� Ž���ӵ��� ����

enum class JsonType {
	JSONNULL,
	JSONINTEGER,
	JSONFLOATPOINT,
	JSONSTRING,
	JSONBOOLEAN,
	JSONARRAY,
	JSONOBJECT
};

class JsonValue {
public:
	const JsonType type;
	virtual JsonValue* Clone() const = 0;
	virtual bool Equal(JsonValue* WhatValue) const = 0;
	virtual ~JsonValue() {}
protected:
	JsonValue(JsonType type) noexcept;
};

//�� �ؿ� ���� �� �ڷ����� ���� Ŭ������ �ۼ��Ѵ�.