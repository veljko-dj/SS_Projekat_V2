#pragma once
#include <string>
#include <list>
using namespace std;



class ParsedTokens {
public:
    enum Type { LABEL = 0, IDENT, GLOBAL, EXTERN, SECTION };
    struct Tok {
        Type type;
        list<string> values;
    };

private:
    static string typeName[] ;
    static list<Tok> tokenList;


public:
    static void addNextToken(Type, string);
    static void addValueToLastToken(string);
    static void justCreateTokenWithNoValues(Type);
    static void printParsedTokens();

};

