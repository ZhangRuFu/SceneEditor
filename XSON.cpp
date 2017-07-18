#include "XSON.h"
#include <TinyXML\tinyxml.h>
#include <iostream>
#include <queue>

using namespace std;

Xson::Xson(string input) : m_lexer(input), m_grammar(&m_lexer)
{

}

TiXmlElement* Xson::Parse(void)
{
	return m_grammar.Parse();
}

Grammar::Grammar(Lexer * lexer)
{
	m_lexer = lexer;
}

TiXmlElement* Grammar::Parse(void)
{
	return Xson();
}

TiXmlElement* Grammar::Xson()
{

	Match(m_lexer->GetNextToken().GetValue(), "[");
	TiXmlElement *element = ID();
	Match(m_lexer->GetNextToken().GetValue(), "]");
	Match(m_lexer->GetNextToken().GetValue(), "{");
	Content(element);
	Match(m_lexer->GetNextToken().GetValue(), "}");
	return element;
}

void Grammar::Attrib(TiXmlElement *element)
{
	Token token = m_lexer->GetNextToken();
	while (token.GetValue() != "}")
	{
		TiXmlElement *key = new TiXmlElement(token.GetValue().data());
		token = m_lexer->GetNextToken();
		Match(token.GetValue(), ":");
		token = m_lexer->GetNextToken();
		key->LinkEndChild(new TiXmlText(token.GetValue().data()));
		element->LinkEndChild(key);
		token = m_lexer->GetNextToken();
		if (token.GetValue() != ";")
			break;
		token = m_lexer->GetNextToken();
	}
	m_lexer->RollBack(token);
}


TiXmlElement* Grammar::ID()
{
	Token token = m_lexer->GetNextToken();
	TiXmlElement *element = new TiXmlElement(token.GetValue().data());
	return element;
}

void Grammar::Content(TiXmlElement *element)
{
	Token token = m_lexer->GetNextToken();
	while (token.GetValue() != "}")
	{
		m_lexer->RollBack(token);
		if (token.GetValue() == "[")
			element->LinkEndChild(Xson());
		else if (token.GetType() == TokenType::STR)
			Attrib(element);
		token = m_lexer->GetNextToken();
	}
	m_lexer->RollBack(token);
}

void Grammar::Match(string token, string str)
{
	if (token != str)
		cout << "²»Æ¥Åä" << endl;
}

Lexer::Lexer(string input) : m_input(input)
{
	m_curIndex = m_input.cbegin();
}

Token Lexer::GetNextToken(void)
{
	if (!m_rollBack.empty())
	{
		Token t = m_rollBack.top();
		m_rollBack.pop();
		return t;
	}
	if (m_curIndex == m_input.cend())
		throw exception("No Token\n");
	if (isSymbol(*m_curIndex))
	{
		string s;
		s += *m_curIndex;
		Token t(TokenType::SYMBOL, s);
		m_curIndex++;
		return t;
	}

	string::const_iterator oldIndex = m_curIndex;
	string::const_iterator i = m_curIndex;
	while (!isSymbol(*m_curIndex))
		i = m_curIndex++;
	string value(oldIndex, m_curIndex);
	return Token(TokenType::STR, value);
}

void Lexer::RollBack(Token token)
{
	m_rollBack.push(token);
}


void XMLHelper::Export(string path, ISerializable *object)
{
	TiXmlDocument *doc = new TiXmlDocument();
	Xson xson(object->Serialize());
	TiXmlElement *root = xson.Parse();
	doc->LinkEndChild(root);
	doc->SaveFile(path.c_str());
	doc->Clear();
	delete doc;
}

const map<string, string>* XMLHelper::Import(string path)
{
	map<string, string> *keyValue = new map<string, string>();
	TiXmlDocument *doc = new TiXmlDocument();
	doc->LoadFile(path.data());
	TiXmlElement *root = doc->RootElement();
	queue<TiXmlElement*> xmlQueue;
	xmlQueue.push(root);
	while (!xmlQueue.empty())
	{
		TiXmlElement *curElement = xmlQueue.front();
		const char *text = curElement->GetText();
		if (text != nullptr)
			(*keyValue)[curElement->Value()] = text;
		TiXmlElement * child = curElement->FirstChildElement();
		while (child != nullptr)
		{
			xmlQueue.push(child);
			child = child->NextSiblingElement();
		}
		xmlQueue.pop();
	}
	doc->Clear();
	delete doc;
	return keyValue;
}
