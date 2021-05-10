#include "parsedTokens.h"

#include <string>
#include <iostream>
#include <list>
using namespace std;

list<ParsedTokens::Tok> ParsedTokens::tokenList ;
string ParsedTokens::typeName[] = { "LABEL","IDENT", "GLOBAL", "EXTERN"
                                    , "SECTION", "WORD", "SKIP"
                                  };

void ParsedTokens::addNextToken(Type t, string s) {
    Tok Tstruct;
    Tstruct.type = t;
    Tstruct.values.push_back(s);
    tokenList.push_back(Tstruct);
}

void ParsedTokens::addValueToLastToken(string s) {
    tokenList.back().values.push_back(s);
}

void ParsedTokens::justCreateTokenWithNoValues(Type t) {
    Tok Tstruct;
    Tstruct.type = t;
    tokenList.push_back(Tstruct);
}


void ParsedTokens::printParsedTokens() {
    cout << endl << endl << "____PARSED_TOKENS_____" << endl;
    for (list<Tok>::iterator it = tokenList.begin(); it != tokenList.end(); ++it) {
        cout   << typeName[(*it).type] <<"->" <<"   ";
        for (auto const &i : (*it).values) {
            std::cout << i <<" + ";
        }
        cout << endl;
    }
}
