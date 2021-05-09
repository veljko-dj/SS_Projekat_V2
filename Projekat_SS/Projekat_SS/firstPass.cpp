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
}

// Vraca liniju koda bez jednolinijskog komentara
// Ukoliko ne pronadje # onda vraca ceo string jer nema komentara
string FirstPass::newLineWithoutComment(string line) {
    string newLine = line.substr(0, line.find("#"));
    return newLine;
}

void FirstPass::testRegex() {
    string line;
    getline(MC::inputFile, line);
    cout << line<<endl;
    if (std::regex_match(line, mojRegex.labelLineWithCommand))
        std::cout << "uspesno";
    else
        cout << "neuspeli pokusaj";

}

void FirstPass::startFirstPass() {

    while (!MC::eofInput()) {	// do kraja fajla

        string line = MC::getInputLine();
        //cout << line << endl;
        line = newLineWithoutComment(line);
        //cout << "BEZ_KOMENTARA_: " << line << endl;
        if (label(line))
            if (deleteLabelFromCommand(line) == "") continue;




    }
    cout << "gotov Prvi prolaz";
}

