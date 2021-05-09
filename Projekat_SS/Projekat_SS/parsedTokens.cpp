#include "parsedTokens.h"

#include <string>
#include <iostream>
#include <list>
using namespace std;

list<ParsedTokens::Tok> ParsedTokens::tokenList ;
string ParsedTokens::typeName[] = { "LABEL","IDENT" };

void ParsedTokens::addNextToken(Type t, string s) {
    Tok Tstruct;
    Tstruct.type = t;
    Tstruct.val = s;
    tokenList.push_back(Tstruct);
}

void ParsedTokens::printParsedTokens() {
    cout << endl << endl << "____PARSED_TOKENS_____" << endl;
    for (list<Tok>::iterator it = tokenList.begin(); it != tokenList.end(); ++it)
        cout   << typeName[(*it).type] <<"->"<< (*it).val <<"   ";
}
