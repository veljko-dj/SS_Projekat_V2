#pragma once
#ifndef FIRSTPASS_H

#include <regex>
#include <iostream>
#include <string>
using namespace std;

class firstPass {
    struct regexi {
        // [ \t]* razmak(tab) 0 ili vise puta
        // \\w* slovo ili broj 0 ili vise puta. Sme da pocne velikim slovom
        // [A-Za-z]* slova samo
        regex labelLineOnly{					//samo labela u redu
            "^[ \t]*\\w*[ \t]*\\:[ \t]*$" };
        regex labelLineWithCommand{				// labela pa bilo sta
            "^[ \t]*\\w*[ \t]*\\:[ \t]*.*" };
        regex identfier{						// rec, naziv ili sta god
            "[a-zA-Z]\\w*"};
    };
    static regexi mojRegex;

    static bool label(string line);
    static string deleteLabelWithCommand(string line);

public:
    static void testRegex();
    static void startFirstPass();
};


#endif // !FIRSTPASS_H
