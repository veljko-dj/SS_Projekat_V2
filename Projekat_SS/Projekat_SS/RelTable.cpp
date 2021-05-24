#include "RelTable.h"
#include "RelEntry.h"

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <iomanip>


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


RelEntry * RelTable::findRelEntryByOrdNum(int id) {
    RelEntry* sym = nullptr;
    for (list<RelEntry*>::iterator it = table.begin(); it != table.end(); ++it)
        if ((*it)->ordNum == id) {
            sym = (*it);
            break;
        }
    return sym;
}

void RelTable::printRelTable(ostream& out) {
    out << "Rel Table: " << endl << setfill(' ');
    out << "\t" << setw(8) << "ord_num:";
    out << "\t" << setw(8) << "loc_offset:";
    out << "\t" << setw(7) << "type:";
    out << "\t" << setw(8) << "symOrd" << " (symName):";
    out << endl;
    for (list<RelEntry*>::iterator it = table.begin(); it != table.end(); ++it)
        (*it)->toString(out);
}

void RelTable::printRelTableForSection(ostream& out, string nameOfSection) {
    out << "Rel Table: " << nameOfSection << endl << setfill(' ');
    out << "\t" << setw(8) << "ord_num:";
    out << "\t" << setw(8) << "loc_offset:";
    out << "\t" << setw(7) << "type:";
    out << "\t" << setw(8) << "symOrd" << " (symName):";
    out << endl;
    for (list<RelEntry*>::iterator it = table.begin(); it != table.end(); ++it)
        if ((*it)->secName == nameOfSection)
            (*it)->toString(out);
}