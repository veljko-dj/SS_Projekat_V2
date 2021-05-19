#pragma once
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


#include <list>
using namespace std;

class Symbol;
class SymbolTable {
    static list<Symbol*> table;		// Radim sa pokazivacem da ne bi doslo do nekih kopiranja neplaniranih

public:
    static void addSymbol(Symbol* symbolPtr);
    static Symbol* getLastSymbol();
    static void  printSymbolTable();
    static int getLastOrdNum();
    static Symbol* getLastSection();
    static Symbol* findSymbolByName(string name);
};


#endif // !SYMBOL_TABLE_H