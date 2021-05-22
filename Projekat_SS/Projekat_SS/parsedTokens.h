#pragma once
#include <string>
#include <list>
using namespace std;



class ParsedTokens {
public:
    enum Type { LABEL = 0, IDENT, GLOBAL, EXTERN, SECTION, WORD, SKIP, EQU, END,
                INSTR0, INSTR1, INSTR2 //labela i direktive
                //HALT, INT, IRET, CALL, RET, JMP, JEQ, JNE, JGT, PUSH, POP, XCHG, ADD, SUB,		//instrukcije
                //MUL, DIV, CMP, NOT, AND, OR, XOR, TEST, SHL, SHR, LDR, STR
              };
    struct Tok {
        Type type;
        list<string> values;
        int numOfLine;
        string getFrontValue();
    };
    // Kod instrukcija je za sada plan da tip bude INSTR, a da values budu tokeni
    // koliko ima parametara proveravamo kao values.size()==1 2 3 itd.

private:
    static string typeName[] ;
    static list<Tok> tokenList;


public:
    static void addNextToken(Type, string,int);
    static void addValueToLastToken(string);
    static void justCreateTokenWithNoValues(Type, int);
    static void printParsedTokens();
    static Tok getNextToken();
    static int isEmpty();

};

