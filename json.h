#pragma once
#include <iostream>
#include <stdlib.h> // 다중 바이트 문자열 변환
#include <string.h>
#include <vector> // 자동으로 메모리가 할당되는 배열
#include <tchar.h> // 아스키 코드, 유니코드 변환을 위함
#include <string>
#include <map> // 왼쪽은 key 값, 오른쪽은 value 값으로 사용, key와 value를 가지는 노드를 생성해서 정렬된 '트리형태'로 저장해두어 탐색속도를 높임

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

//이 밑에 부터 각 자료형에 대한 클래스를 작성한다.