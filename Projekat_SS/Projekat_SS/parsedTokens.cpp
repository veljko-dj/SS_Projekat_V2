#include "../h/parsedTokens.h"

#include <string>
#include <iostream>
#include <list>
#include <iomanip>      // std::setw
#include <algorithm>
using namespace std;

list<ParsedTokens::Tok> ParsedTokens::tokenList ;
string ParsedTokens::typeName[] = { "LABEL","IDENT", "GLOBAL", "EXTERN", "SECTION",
                                    "WORD", "SKIP", "EQU","END", "INSTR0", "INSTR1", "INSTR2"
                                    //instrukcije
                                    //"HALT", "INT", "IRET", "CALL", "RET", "JMP",
                                    //"JEQ", "JNE", "JGT","PUSH", "POP", "XCHG",
                                    //"ADD", "SUB", "MUL", "DIV", "CMP", "NOT",
                                    //"AND", "OR", "XOR", "TEST", "SHL", "SHR", "LDR", "STR"
                                  };

void ParsedTokens::addNextToken(Type t, string s, int numOfLinee) {
    Tok Tstruct;
    Tstruct.type = t;
    Tstruct.numOfLine = numOfLinee;
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    s.erase(remove(s.begin(), s.end(), '\t'), s.end());
    Tstruct.values.push_back(s);
    tokenList.push_back(Tstruct);
}

void ParsedTokens::addValueToLastToken(string s) {
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    s.erase(remove(s.begin(), s.end(), '\t'), s.end());
    tokenList.back().values.push_back(s);
}

void ParsedTokens::justCreateTokenWithNoValues(Type t, int num) {
    Tok Tstruct;
    Tstruct.type = t;
    Tstruct.numOfLine = num;
    tokenList.push_back(Tstruct);
}

void ParsedTokens::printParsedTokens() {
    cout << endl << endl << "____PARSED_TOKENS_____" << endl;
    for (list<Tok>::iterator it = tokenList.begin(); it != tokenList.end(); ++it) {
        cout   <<setw(13)<< typeName[(*it).type] <<":"<< setw(2)<<(*it).numOfLine <<" -> ";
        for (auto const &i : (*it).values) {
            std::cout << i <<" - ";
        }
        cout << endl;
    }
}

ParsedTokens::Tok ParsedTokens::getNextToken() {
    // Brise, nema stampanja tokena posle drugog prolasa
    Tok tmpTok = tokenList.front();
    tokenList.pop_front();
    return tmpTok;
}

int ParsedTokens::isEmpty() {
    return tokenList.empty();
}

string ParsedTokens::Tok::getFrontValue() {
    if (this->values.empty()) return nullptr;
    string tmp = this->values.front();
    this->values.pop_front();
    return tmp;
}
