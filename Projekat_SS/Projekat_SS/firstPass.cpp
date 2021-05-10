#include "MainClass.h"
#include "firstPass.h"
#include "parsedTokens.h"
#include "Symbol.h"
#include "SymbolTable.h"

#include <string>
#include <regex>
#include <iostream>
#include <fstream>

using namespace std;


using MC = MainClass;
using PT = ParsedTokens;
using ST = SymbolTable;


FirstPass::regexi FirstPass::mojRegex;

bool FirstPass::label(string line) {
    smatch match;

    if (regex_match(line,mojRegex.labelLineOnly)
            || regex_match(line, mojRegex.labelLineWithCommand)) {
        regex_search(line, match, mojRegex.identfier);
        //cout << "Labela :|" << match[0] << "|"<<endl;
        PT::addNextToken(PT::LABEL, match[0]);
        {
            //privremena resenja
            int ord_num = 3;
            int currOffset = 10;
            int size = 4;
            string currSection = "trenutnaSekcija";
            {
                // Dodavanje u tabelu simbola
                Symbol* sym = new Symbol(ord_num, match[0], currOffset, size, 'l', true, currSection);
                ST::addSymbol(sym);
            }
        }
        return true;
    } else
        return false;


}

// Brise labelu iz reda koji sadrzi i labelu i komandu neku
// Vraca "" ukoliko je to red samo sa labelom
string FirstPass::deleteLabelFromCommand(string line) {
    if (regex_match(line, mojRegex.labelLineOnly))
        return "";
    if (regex_match(line, mojRegex.labelLineWithCommand)) {
        string newLine = line.substr(1+line.find(":"));	//dodajem 1 da preskocim :
        //cout << "NOVALINIJA" << endl << newLine << endl;
        return newLine;
    }
    return "error"; // ovo postoji samo da bi se sklonio warning
}

// Vraca liniju koda bez jednolinijskog komentara
// Ukoliko ne pronadje # onda vraca ceo string jer nema komentara
string FirstPass::newLineWithoutComment(string line) {
    string newLine = line.substr(0, line.find("#"));
    return newLine;
}

bool FirstPass::globalDirective(string line) {
    smatch match;

    if (regex_match(line, mojRegex.global)) {
        // Uklanjanje global
        line = line.substr(line.find(".global")+7);
        PT::justCreateTokenWithNoValues(PT::GLOBAL);
        // pokupljen kod sa cplusplus.com regex_search
        while (regex_search(line, match, mojRegex.identfier)) {
            for (auto ident : match) {
                PT::addValueToLastToken(ident);
            }
            line = match.suffix().str();	// bez ovoga vecna petlja
        }
        // Nije potrebno raditi u prvom prolazu, samo dodati token
        // predavanja 108/394
        return true;
    } else
        return false;
}

bool FirstPass::externDirective(string line) {
    smatch match;

    if (regex_match(line, mojRegex.exterN)) {
        // Na predavanjima vidimo da treba da dodam u tabelu simbola,
        // Na predavanju je to poslednji ulaz u tabeli simbola
        // ali na netu ne pise da mora da bude poslednji ulaz i zato stavljam redom
        // (https://www.magmath.com/english/programming/c_programming_language/projects/two_pass_assembler.php)

        line = line.substr(line.find(".extern") + 7);
        PT::justCreateTokenWithNoValues(PT::EXTERN);
        // pokupljen kod sa cplusplus.com regex_search
        while (regex_search(line, match, mojRegex.identfier)) {
            for (auto ident : match) {
                PT::addValueToLastToken(ident);
                {
                    //privremeno resenje:
                    // Za extern uvozi simbole, prema vezbama(37/119), sekcija nepoznata
                    // vrednost 0, redni broj okej i GLOBAL je, sekcija UND
                    int ord_num = ST::getLastOrdNum()+1;
                    string name = ident;
                    int currOffset = 0;		// nebitno
                    int size = 0;			// ne znam, logicno je da je nebitno
                    string currSection = "UND"; // s vezbi
                    char type = 's';		// simbol
                    bool isLocal = false;
                    {
                        // Dodavanje u tabelu simbola
                        Symbol* sym = new Symbol(ord_num, name, currOffset, size, type, isLocal, currSection);
                        ST::addSymbol(sym);
                    }
                }
            }
            line = match.suffix().str();	// bez ovoga vecna petlja
        }

        return true;
    } else
        return false;
}

void FirstPass::testRegex() {
    cout << endl << "____TESTREGEX____" << endl;


    while (!MC::eofInput()) {	// do kraja fajla

        string line = MC::getInputLine();
        cout << line << endl;
        line = newLineWithoutComment(line);
        //cout << "BEZ_KOMENTARA_: " << line << endl;

        if (std::regex_match(line, mojRegex.global))
            std::cout << "\t \t \t \t uspesno uparivanje regexa"<< endl;
        else
            cout << "\t \t \t \t \t \t \t neuspesno uparivanje regexa" << endl;

    }
}

void FirstPass::startFirstPass() {

    while (!MC::eofInput()) {	// do kraja fajla

        string line = MC::getInputLine();
        //cout << line << endl;
        line = newLineWithoutComment(line);
        //cout << "BEZ_KOMENTARA_: " << line << endl;
        if (label(line))
            if (deleteLabelFromCommand(line) == "") continue;
        if (globalDirective(line)) continue;
        if (externDirective(line)) continue;


    }
    cout << endl << "gotov Prvi prolaz";
}

