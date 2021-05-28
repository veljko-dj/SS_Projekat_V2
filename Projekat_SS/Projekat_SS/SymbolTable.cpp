#include "../h/SymbolTable.h"
#include "../h/Symbol.h"

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>


using namespace std;

list<Symbol*> SymbolTable::table;


void SymbolTable::addSymbol(Symbol * symbolPtr) {
    table.push_back(symbolPtr);
}

Symbol* SymbolTable::getLastSymbol() {
    if (table.size() > 0) return table.back();
    return nullptr;
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


void SymbolTable::printSymbolTable(std::ostream& out) {
    out << "Symbol Table :" << endl;
    out << setw(8) << "ord_num:";
    out << setw(14) << "name:";
    out << setw(10) << "val_off:";
    out << setw(10) << "size:";
    out << setw(10) << "local:";
    out << setw(14) << "section:";
    out << endl;
    for (list<Symbol*>::iterator it = table.begin(); it != table.end(); ++it)
        (*it)->toString(out);
    out << endl << endl;
}

void SymbolTable::printSymbolTableBinary(std::ostream & out) {
    int numOfSym = table.size();
    out.write((char*)&numOfSym, sizeof(int));
    for (list<Symbol*>::iterator it = table.begin(); it != table.end(); ++it) {
        Symbol s = *(*it);
        s.toString(cout);
        out.write((char*)&s, sizeof(Symbol));
    }
    out << endl << endl;
}
