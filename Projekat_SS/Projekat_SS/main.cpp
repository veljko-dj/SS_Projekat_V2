#pragma once

#include <iostream>
#include <string>

#include "MainClass.h"
#include "firstPass.h"
#include "parsedTokens.h"
#include "SymbolTable.h"

using namespace std;

int main(int argc, char *argv[]) {
    cout << "Program pokrenut" << endl;
    MainClass mc = MainClass();	// Ovo obrisi, mislim da ne treba


    MainClass::passArgAndOpenFiles(argc, argv);
    FirstPass::startFirstPass();

    ParsedTokens::printParsedTokens();
    SymbolTable::printSymbolTable();

    OVDE UBACI POZIV TESTREGEX. DODAJ regex za global i nastavi da radis dalje

    MainClass::assemblyDone();
    cout << endl << endl;
}