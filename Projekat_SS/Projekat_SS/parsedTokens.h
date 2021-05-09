#pragma once
#include <string>
#include <list>
using namespace std;



class ParsedTokens {
public:
    enum Type { LABEL = 0, IDENT };
    struct Tok {
        Type type;
        string val;
    };

private:
    static string typeName[] ;
    static list<Tok> tokenList;


public:
    static void addNextToken(Type, string);
    static void printParsedTokens();

};

