#pragma once
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


#include <list>
#include <string> 

class Symbol;
class SymbolTable {
    static std::list<Symbol*> table;		// Radim sa pokazivacem da ne bi doslo do nekih kopiranja neplaniranih

public:
    static void addSymbol(Symbol* symbolPtr);
    static Symbol* getLastSymbol();
    static int getLastOrdNum();
    static Symbol* getLastSection();
    static Symbol* findSymbolByName(std::string name);
    static void  printSymbolTable(std::ostream& out);
    static void  printSymbolTableBinary(std::ostream& out);
};


#endif // !SYMBOL_TABLE_H