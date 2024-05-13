#include "gtest/gtest.h"
#include "../parser/MyParser.h"

#define TEST_ASN_MODULE_BEGIN "Spec { iso(1) } DEFINITIONS EXPLICIT TAGS ::= BEGIN\r\n"
#define TEST_ASN_MODULE_END   "END\r\n"

TEST(ParserTest, RFC4120Test) {
	int err = 0;
	MyParser parser;
	err = parser.Parse(L"../../../Test/testdata/rfc4120.asn1");
	ASSERT_EQ(err, 0);
}

TEST(ParserTest, TestCase1) {

	const char* data = 
		TEST_ASN_MODULE_BEGIN
		"KDC-REQ         ::= SEQUENCE {\r\n"
    "    msg-type        [2] INTEGER (10 -- AS -- | 12 -- TGS --)"
		"}\r\n"
		TEST_ASN_MODULE_END;

	int err = 0;
	MyParser parser;
	err = parser.Parse(data, strlen(data));
	ASSERT_EQ(err, 0);
}

TEST(ParserTest, TestCase2) {

	const char* data = 
		TEST_ASN_MODULE_BEGIN
		"KDC-REQ         ::= SEQUENCE {\r\n"
    "     pvno            [1] INTEGER (5)"
		"}\r\n"
		TEST_ASN_MODULE_END;

	int err = 0;
	MyParser parser;
	err = parser.Parse(data, strlen(data));
	ASSERT_EQ(err, 0);
}