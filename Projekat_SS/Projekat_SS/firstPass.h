#pragma once
#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include <regex>
#include <iostream>
#include <string>
using namespace std;

class FirstPass {
    struct regexi {
        /*	[ \t]* razmak(tab) 0 ili vise puta
         *	\\w* slovo ili broj ili _ ,0 ili vise puta.
         *	[A-Za-z]* slova samo
         */

        regex emptyLine{						//	Prazna linija
            "^[ \t]*$" };
        regex labelLineOnly{					//	Labela u liniji
            "^[ \t]*\\w+[ \t]*\\:[ \t]*$" };
        regex labelLineWithCommand{				//	Labela i komanda u 1 liniji
            "^[ \t]*\\w+[ \t]*\\:[ \t]*.*" };
        regex identfier{						//	Identifikator koji pocinje slovom
            "[a-zA-Z]\\w*"};
        regex identfierOrNumber{				//	Bilo sta sem razmaka
            "\\w+" };
        regex simbol{							//  Isto kao ident sa ' '$ i^
            "^[ \t]*\\w+[ \t]*$" };
        regex number{							//	Broj dec ili hex
            "(\\d+)|(0x(\\d|[a-f]|[A-F])+)" };
        regex everything{						// Bas bilo sta
            ".*"};
        regex literal{
            // Sta sve spada u literale je nadjeno na keil.com literals
            "^[ \t]*((\\d+)|(0x(\\d|[a-f]|[A-F])+)|(\\d_\\d+)"
            ")[ \t]*$" };			// Ne zaboravi da ovde postoji $
        /*
        	Izbaceni delovi radi uproscavanja literala: RADE
        	|(TRUE|FALSE)	|('[a-zA-Z]')	|(\\d+\\.\\d+)
        */

        //////////////////////////////////////////////////////////////////
        // Regeksi za direktive
        regex global{
            "[ \t]*\\.global[ \t]+\\w+([ \t]*,[ \t]*\\w+)*[ \t]*$"};	// Ovde imas \\. umesto \.
        regex exterN{
            "[ \t]*\\.extern[ \t]+\\w+([ \t]*,[ \t]*\\w+)*[ \t]*$" };	// kao i \\w umesto \w
        regex section{
            "[ \t]*\\.section[ \t]+\\w+[ \t]*$" };
        regex dataSection{
            "[ \t]*\\.data[ \t]*$" };
        regex textSection{
            "[ \t]*\\.text[ \t]*$" };
        regex bssSection{
            "[ \t]*\\.bss[ \t]*$" };
        regex word{
            "[ \t]*\\.word[ \t]+\\w+([ \t]*,[ \t]*\\w+)*[ \t]*$" };	// kao i \\w umesto \w
        regex skip{
            "[ \t]*\\.skip[ \t]+((\\d+)|(0x(\\d|[a-f]|[A-F])+))[ \t]*$" };
        regex equ{
            "[ \t]*\\.equ[ \t]+\\w+[ \t]*,[ \t]*((\\d+)|(0x(\\d|[a-f]|[A-F])+))[ \t]*$" };
        regex end{
            "[ \t]*\\.end[ \t]*$" };
        regex noOper{
            "[ \t]*\\w+[ \t]*$" };
        regex oneOper{
            "[ \t]*\\w+[ \t]+([\\w\\*\\.\\+\\[\\]\\%\\$ ])+[ \t]*$" };  // Prekopiraj ovo dole za twoOper
        regex twoOper{
            "[ \t]*\\w+[ \t]+(r[0-9])[ \t]*,[ \t]*([\\w\\*\\.\\+\\[\\]\\%\\$ ])+[ \t]*$" };
        ////////////////////////////////////////////////////
        // Regeksi vezani za operande i tako to
        // Moras da proveris prvo slovo da li je $%[ ili sta vec
        regex regDir{
            "^[ \t]*(r[0-9]|sp|pc|psw)[ \t]*$"};			//regX, to je valjda i regD i regS

    };
    static regexi mojRegex;
    static string currSection;
    static int currOffset;
    static int numOfLine; // nepotrebno, cisto onako da znas
    static bool end;
    // Bilo kakva greska
    static void error(string msg, string line);

    static bool emptyLine(string line);
    static bool checkOperand(string line);
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
    // section DATA direktiva
    static bool sectionDataDirective(string line);
    // section Text direktiva
    static bool sectionTextDirective(string line);
    // section Bss direktiva
    static bool sectionBssDirective(string line);
    // word direktiva
    static bool wordDirective(string line);
    // skip direktiva
    static bool skipDirective(string line);
    // equ direktiva
    static bool equDirective(string line);
    // end direktiva
    static bool endDirective(string line);

    // instrk bez operanada;
    static bool noOperInstr(string line);
    // instrk sa 1 operandom;
    static bool oneOperInstr(string line);
    // instrk sa 2 operanda;
    static bool twoOperInstr(string line);

public:
    static void testRegex();
    static void startFirstPass();
};


#endif // !FIRSTPASS_H
