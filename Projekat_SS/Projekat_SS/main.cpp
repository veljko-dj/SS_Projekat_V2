#pragma once

#include <iostream>
#include <string>

#include "MainClass.h"
#include "firstPass.h"
#include "SecondPass.h"
#include "parsedTokens.h"
#include "SymbolTable.h"
#include "RelTable.h"
#include "SectionTable.h"

using namespace std;

int main(int argc, char *argv[]) {
    cout << "Program pokrenut" << endl;
    MainClass mc = MainClass();	// Ovo obrisi, mislim da ne treba


    MainClass::passArgAndOpenFiles(argc, argv);

    FirstPass::startFirstPass();
    MainClass::closeInputFile();

    ParsedTokens::printParsedTokens();
    //Ovo mora ovde jer posle ispraznis listu


    SecondPass::startSecondPass();

    // cout
    // SymbolTable::printSymbolTable(cout);
    // SectionTable::printSections(cout);
    // RelTable::printRelTable(cout);

    // file output
    SymbolTable::printSymbolTable((ostream&)MainClass::outputFile);
    SectionTable::printSections((ostream&)MainClass::outputFile);
    // RelTable::printRelTable((ostream&)MainClass::outputFile);

    MainClass::closeOutputFile();
    // FirstPass::testRegex();

    MainClass::assemblyDone();
}