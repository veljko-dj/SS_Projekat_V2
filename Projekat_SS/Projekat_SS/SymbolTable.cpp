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
    if (table.size() == 0) return 0;
    else return table.back()->getOrdNum();
}

Symbol * SymbolTable::getLastSection() {
    Symbol* last = nullptr;
    if (table.size() > 0) {
        list<Symbol*>::iterator it;
        for (list<Symbol*>::iterator it = table.begin(); it != table.end(); ++it)
            if ((*it)->getType() == 'S') last = (*it);
    }
    return last;
}

Symbol * SymbolTable::findSymbolByName(string name) {
    Symbol* sym = nullptr;
    for (list<Symbol*>::iterator it = table.begin(); it != table.end(); ++it)
        if ((*it)->getName() == name) {
            sym = (*it);
            break;
        }
    return sym;
}
