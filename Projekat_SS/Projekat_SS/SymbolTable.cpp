#include "SymbolTable.h"
#include "Symbol.h"

#include <string>
#include <iostream>


using namespace std;

list<Symbol*> SymbolTable::table;


void SymbolTable::addSymbol(Symbol * symbolPtr) {
    table.push_back(symbolPtr);
}

Symbol* SymbolTable::getLastSymbol() {
    if (table.size()>0) return table.back();
    return nullptr;
}

void SymbolTable::printSymbolTable() {
    cout << endl << endl << "____ SYMBOL_TABLE____" << endl;
    for (list<Symbol*>::iterator it = table.begin(); it != table.end(); ++it)
        cout << (*it)->toString() << endl;
}

int SymbolTable::getLastOrdNum() {
    return table.back()->getOrdNum();
}
