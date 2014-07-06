#include "Precompiled.h"
#include <fstream>
#include <iostream>

#include "RhoLang.h"
#include "Parser.h"
#include "Translator.h"

using namespace std;

KAI_TRANS_BEGIN

void RhoLang::Print()
{
	cout << "Input--" << endl;
	cout << lex->input << endl;

	cout << "Lexer--" << endl;
	lex->Print();

	cout << "Parser--" << endl;
	parse->Print();

	cout << "Trans--" << endl;
	cout << trans->Result() << endl;
}

bool RhoLang::TranslateFile(const char *name, Parser::Structure st)
{
	ifstream file(name);
	if (!file)
		return false;
	file.seekg(0, ios::end);
	int len = (int)file.tellg();
	char *text = new char[len + 1];
	file.seekg(0, ios::beg);
	file.read(text, len);
	text[len] = 0;

	return Translate(text, st);
}

bool RhoLang::Translate(const char *text, Parser::Structure st)
{
	lex = new Lexer(text);
	parse = new Parser(lex, st);
	trans = new Translator(parse);
	if (lex->Failed)
		return Fail(lex->Error);
	if (parse->Failed)
		return Fail(parse->Error);
	if (trans->Failed)
		return Fail(trans->Error);
	return true;
}

KAI_TRANS_END