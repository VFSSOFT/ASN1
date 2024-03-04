#include "pch.h"

#include "../parser/MyTokenizer.h"

TEST(TokenizerTest, TypeRefTest) {
	const char* data = "TypeA";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_TYPE_REF);
	ASSERT_STREQ(tokenizer.Token()->Deref(), data);

	err = tokenizer.Next();
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_EOF);
	ASSERT_EQ(tokenizer.Token()->Length(), 0);
}

TEST(TokenizerTest, IdentifiersTest) {
	const char* data = "identifier";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_IDENTIFIER);
	ASSERT_STREQ(tokenizer.Token()->Deref(), data);

	err = tokenizer.Next();
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_EOF);
	ASSERT_EQ(tokenizer.Token()->Length(), 0);
}

TEST(TokenizerTest, CommentSingleLineTest) {
	const char* data = "--abcdefg\r\n";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_ONE_LINE_COMMENT);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "abcdefg");
}

TEST(TokenizerTest, CommentSingleLineTest2) {
	const char* data = "--abcdefg--";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_ONE_LINE_COMMENT);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "abcdefg");
}

TEST(TokenizerTest, CommentSingleLineTest3) {
	const char* data = "--/*aaa*/--";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_ONE_LINE_COMMENT);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "/*aaa*/");
}

TEST(TokenizerTest, CommentMultipleLineTest) {
	const char* data = "/*comment*/";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_MUL_LINE_COMMENT);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "comment");
}

TEST(TokenizerTest, CommentMultipleLineTest2) {
	const char* data = "/*com\r\n\r\nment*/";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_MUL_LINE_COMMENT);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "com\r\n\r\nment");
}

TEST(TokenizerTest, CommentMultipleLineTest3) {
	const char* data = "/*com--\r\n\r\n--ment*/";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_MUL_LINE_COMMENT);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "com--\r\n\r\n--ment");
}

TEST(TokenizerTest, CommentMultipleLineTest4) {
	const char* data = "/*com/*m*/ent*/";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_MUL_LINE_COMMENT);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "com/*m*/ent");
}


TEST(TokenizerTest, NumberTest) {
	const char* data = "123456";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_NUMBER);
	ASSERT_STREQ(tokenizer.Token()->Deref(), data);
}

TEST(TokenizerTest, RealNumberTest) {
	const char* data = "12.3456";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_REAL_NUMBER);
	ASSERT_STREQ(tokenizer.Token()->Deref(), data);
}

TEST(TokenizerTest, BStringTest) {
	const char* data = "\'01101100\'B";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_BSTRING);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "01101100");
}

TEST(TokenizerTest, BStringTest2) {
	const char* data = "\'011  011\t00\'B";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_BSTRING);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "01101100");
}

TEST(TokenizerTest, HexStringTest) {
	const char* data = "\'AB0196\'H";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_HEXSTRING);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "AB0196");
}

TEST(TokenizerTest, HexStringTest2) {
	const char* data = "\'AB  0 196\'H";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_HEXSTRING);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "AB0196");
}

TEST(TokenizerTest, CStringTest) {
	const char* data = "\"A B C D\"";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_CSTRING);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "A B C D");
}

TEST(TokenizerTest, CStringTest2) {
	const char* data = "\"ABCDE FGH\r\nIJK\"\"XYZ\"";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_CSTRING);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "ABCDE FGHIJK\"XYZ");
}


TEST(TokenizerTest, AssignmentLexicalItemTest) {
	const char* data = "::=";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_ASSIGNMENT);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "::=");
}

TEST(TokenizerTest, RangeSeparatorTest) {
	const char* data = "..";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_RANGE_SEPARATOR);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "..");
}


TEST(TokenizerTest, EllipsisTest) {
	const char* data = "...";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_ELLIPSIS);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "...");
}


TEST(TokenizerTest, LeftVersionBraketsTest) {
	const char* data = "[[";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_LEFT_VERSION_BRACKETS);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "[[");
}


TEST(TokenizerTest, RightVersionBraketsTest) {
	const char* data = "]]";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_RIGHT_VERSION_BRACKETS);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "]]");
}


TEST(TokenizerTest, XMLEndTagStartItemTest) {
	const char* data = "</";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_XML_END_TAG_START);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "</");
}

TEST(TokenizerTest, XMLSingleTagEndItemTest) {
	const char* data = "/>";
	int err = 0;
	MyTokenizer tokenizer(data, strlen(data));
	err = tokenizer.Next();
	ASSERT_EQ(err, 0);
	ASSERT_EQ(tokenizer.TokenType(), TOKEN_XML_SIGNAL_TAG_END);
	ASSERT_STREQ(tokenizer.Token()->Deref(), "/>");
}

TEST(TokenizerTest, SingleCharacterLexicalItemsTest) {
	int err = 0;
	char buf[2];
	buf[1] = ' ';
	const char* data = "{}<>,.()[]-:=\"\' ;@|!^";
	
	for (int i = 0; i < strlen(data); i++) {	
		buf[0] = data[i];
		
		MyTokenizer tokenizer(buf, 2);
		err = tokenizer.Next();
		ASSERT_EQ(err, 0);
		ASSERT_EQ(tokenizer.TokenType(), TOKEN_SINGLE_CHAR_ITEM);
		ASSERT_EQ(tokenizer.Token()->Length(), 1);
		ASSERT_EQ(tokenizer.Token()->CharAt(0), data[i]);
	}
}

TEST(TokenizerTest, SingleCharacterLexicalItemsTest2) {
	int err = 0;
	char buf[1];
	const char* data = "{}<>,.()[]-:=\"\' ;@|!^";
	
	for (int i = 0; i < strlen(data); i++) {	
		buf[0] = data[i];
		
		MyTokenizer tokenizer(buf, 1);
		err = tokenizer.Next();
		ASSERT_EQ(err, 0);
		ASSERT_EQ(tokenizer.TokenType(), TOKEN_SINGLE_CHAR_ITEM);
		ASSERT_EQ(tokenizer.Token()->Length(), 1);
		ASSERT_EQ(tokenizer.Token()->CharAt(0), data[i]);
	}
}

