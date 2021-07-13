#include "json.h"
#include <iostream>
using namespace std;
using namespace namespace_myJson;

int main() {

	string str;
	char chStr[500] = { "NULL" };
	ifstream ist("test.json");
	for (char chP; ist >> chP;)
	{
		str += chP;
	}

	strcpy_s(chStr, str.c_str());

	char* ts = chStr;
	char* n;

	auto s = ParseObject(ts, n);
	cout << reinterpret_cast<JsonString*>(s->Get("NAME"))->Get() << endl;
	cout << reinterpret_cast<JsonNum*>(s->Get("AGE"))->GetAsInt() << endl;
	cout << reinterpret_cast<JsonString*>(reinterpret_cast<JsonArray*>(s->Get("ARRAY"))->At(1))->Get() << endl;
	cout << reinterpret_cast<JsonString*>(reinterpret_cast<JsonObject*>(s->Get("OBJECT"))->Get("ADDRESS"))->Get() << endl;

	delete s;
	return 0;
}