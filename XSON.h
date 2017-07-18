#pragma once
#include <string>
#include <stack>
#include <map>
#include "Serialize.h"

using namespace std;

class TiXmlElement;

enum TokenType { ID, XSON, ATTRIB, STR, SYMBOL };

class Token
{
	string m_value;
	TokenType m_type;

public:
	Token(TokenType type, string value) : m_value(value), m_type(type) {}
	string GetValue(void) { return m_value; }
	TokenType GetType(void) { return m_type; }
};

class Lexer
{
	string m_input;
	stack<Token> m_rollBack;
	string::const_iterator m_curIndex;
public:
	Lexer(string input);
	Token GetNextToken(void);
	void RollBack(Token token);

private:
	bool isAlpha(char c) { return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)); }
	bool isNumber(char c) { return c >= 48 && c <= 57; }
	bool isSymbol(char c) { return c == '[' || c == ']' || c == '{' || c == '}' || c == ';' || c == ':'; }
	bool isSameType(char c1, char c2) { return ((isAlpha(c1) && isAlpha(c2)) || (isNumber(c1) && isNumber(c2))); }
};

class Grammar
{
	Lexer *m_lexer;
public:
	Grammar(Lexer *lexer);
	TiXmlElement* Parse(void);

private:
	TiXmlElement* Xson();
	void Attrib(TiXmlElement *element);
	TiXmlElement* ID();
	void Content(TiXmlElement *element);
	void Match(string token, string str);
};

class Xson
{
	Lexer m_lexer;
	Grammar m_grammar;

public:
	Xson(string input);

	TiXmlElement* Parse(void);
};


class XMLHelper
{
public:
	static void Export(string path, ISerializable *object);
	static const map<string, string>* Import(string path);
};