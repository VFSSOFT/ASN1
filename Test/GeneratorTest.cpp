#include "gtest/gtest.h"

#include "../parser/MyParser.h"
#include "../generator/MyGenerator.h"

TEST(GeneratorTest, RFC4120Test) {
	int err = 0;
	MyParser parser;
	err = parser.Parse(L"../../Test/testdata/rfc4120.asn1");
	ASSERT_EQ(err, 0);

	MyGenerator gen;
	err = gen.Process(parser.Result());
	ASSERT_EQ(err, 0);

	MyStringA code;
	err = gen.GenCode(&code);
	ASSERT_EQ(err, 0);
}