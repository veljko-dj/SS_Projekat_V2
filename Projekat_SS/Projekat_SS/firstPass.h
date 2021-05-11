#pragma once
#ifndef FIRSTPASS_H

#include <regex>
#include <iostream>
#include <string>
using namespace std;

class FirstPass {
    struct regexi {
        // [ \t]* razmak(tab) 0 ili vise puta
        // \\w* slovo ili broj 0 ili vise puta. Sme da pocne velikim slovom
        // [A-Za-z]* slova samo
        regex emptyLine{						//samo labela u redu
            "^[ \t]*$" };
        regex labelLineOnly{					//samo labela u redu
            "^[ \t]*\\w+[ \t]*\\:[ \t]*$" };
        regex labelLineWithCommand{				// labela pa bilo sta
            "^[ \t]*\\w+[ \t]*\\:[ \t]*.*" };
        regex identfier{						// rec, naziv ili sta god
            "[a-zA-Z]\\w*"};
        regex identfierOrNumber{
            "\\w+" };
        regex number{							// broj
            "\\d+" };
        regex global{
            "[ \t]*\\.global[ \t]+\\w+([ \t]*,[ \t]*\\w+)*[ \t]*$"};	// Ovde imas \\. umesto \.
        regex exterN{
            "[ \t]*\\.extern[ \t]+\\w+([ \t]*,[ \t]*\\w+)*[ \t]*$" };	// kao i \\w umesto \w
        regex section{
            "[ \t]*\\.section[ \t]+\\w+[ \t]*$" };
        regex word{
            "[ \t]*\\.word[ \t]+\\w+([ \t]*,[ \t]*\\w+)*[ \t]*$" };	// kao i \\w umesto \w
        regex skip{
            "[ \t]*\\.skip[ \t]+\\d+[ \t]*$" };
        regex equ{
            "[ \t]*\\.equ[ \t]+\\w+[ \t]*,[ \t]*\\d+[ \t]*$" };
        regex end{
            "[ \t]*\\.end[ \t]*$" };
    };
    static regexi mojRegex;
    static string currSection;
    static int currOffset;
    static int numOfLine; // nepotrebno, cisto onako da znas
    static bool end;

    static bool emptyLine(string line);
    // Labela
    static bool label(string line);
    static string deleteLabelFromCommand(string line);
    // Komentar
    static string newLineWithoutComment(string line);
    // global direktiva
    static bool globalDirective(string line);
    // extern direktiva
    static bool externDirective(string line);
    // section direktiva
    static bool sectionDirective(string line);
    // word direktiva
    static bool wordDirective(string line);
    // skip direktiva
    static bool skipDirective(string line);
    // equ direktiva
    static bool equDirective(string line);
    // end direktiva
    static bool endDirective(string line);

public:
    static void testRegex();
    static void startFirstPass();
};


#endif // !FIRSTPASS_H
