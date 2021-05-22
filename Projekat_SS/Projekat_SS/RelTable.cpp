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

int RelTable::getLastOrdNum() {
    int max = 0;
    for (list<RelEntry*>::iterator it = table.begin(); it != table.end(); ++it)
        if ((*it)->ordNum > max) max = (*it)->ordNum;
    return max;
}

RelEntry * RelTable::getLastEntry() {
    if (table.size() > 0) return table.back();
    return nullptr;
}

void RelTable::printRelTable() {
    cout << endl << endl << "____REL_TABLE____" << endl;
    for (list<RelEntry*>::iterator it = table.begin(); it != table.end(); ++it)
        cout << (*it)->toString() << endl;
}

RelEntry * RelTable::findRelEntryByOrdNum(int id) {
    RelEntry* sym = nullptr;
    for (list<RelEntry*>::iterator it = table.begin(); it != table.end(); ++it)
        if ((*it)->ordNum == id) {
            sym = (*it);
            break;
        }
    return sym;
}
