#pragma once
#include <list>
using namespace std;

class Symbol;
class SymbolTable {
    static list<Symbol*> table;		// Radim sa pokazivacem da ne bi doslo do nekih kopiranja neplaniranih

public:
    static void addSymbol(Symbol* symbolPtr);
    static Symbol* getLastSymbol();
    static void  printSymbolTable();
};

