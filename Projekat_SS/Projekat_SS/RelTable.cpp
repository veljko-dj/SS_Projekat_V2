#include "RelTable.h"
#include "RelEntry.h"

#include <iostream>
#include <string>
#include <list>


using namespace std;

list<RelEntry*> RelTable::table;

void RelTable::addEntry(RelEntry * symbolPtr) {
    table.push_back(symbolPtr);
}

RelEntry * RelTable::getLastEntry() {
    if (table.size() > 0) return table.back();
    return nullptr;
}

void RelTable::printRelTable() {
    cout << endl << endl << "____ SYMBOL_TABLE____" << endl;
    for (list<RelEntry*>::iterator it = table.begin(); it != table.end(); ++it)
        cout << (*it)->toString() << endl;
}

RelEntry * RelTable::findRelEntryById(int id) {
    RelEntry* sym = nullptr;
    for (list<RelEntry*>::iterator it = table.begin(); it != table.end(); ++it)
        if ((*it)->id == id) {
            sym = (*it);
            break;
        }
    return sym;
}
