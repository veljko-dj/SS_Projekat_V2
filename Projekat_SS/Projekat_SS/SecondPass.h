#pragma once
#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <string>
using namespace std;

class Section;
class SecondPass {

    static string currSectionName;
    static int currSecNum;
    static Section* currSection;
    static int currOffset;
    static bool end;

    static void error(string msg, int numOfLine);
//    static void instr0( int token);
//    static void instr1( int token);
//    static void instr2( int token);
    static void writeSymbolToMemAndCreatRelEntry(string nameOfSymbol, int numOfLinee);

public:
    static void startSecondPass();
};


#endif // !SECOND_PASS_H
