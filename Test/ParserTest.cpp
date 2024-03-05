#include "pch.h"

#include "../parser/MyParser.h"

TEST(ParserTest, RFC4120Test) {
	int err = 0;
	MyParser parser;

	wchar_t path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);

	err = parser.Parse(L"../../../Test/testdata/rfc4120.asn1");
}