#include "SectionTable.h"
#include "Section.h"

#include <list>
#include <string>
#include <iostream>

using namespace std;

list<Section*> SectionTable::table;


void SectionTable::addSection(Section * symbolPtr) {
    table.push_back(symbolPtr);
}

Section* SectionTable::getLastSection() {
    if (table.size() > 0) return table.back();
    return nullptr;
}

void SectionTable::printSectionTable() {
    cout << endl << endl << "____ SECTION_TABLE____" << endl;
    for (list<Section*>::iterator it = table.begin(); it != table.end(); ++it)
        cout << (*it)->toString() << endl;
}

int SectionTable::getLastOrdNum() {
    if (table.size() == 0) return 0;
    else return table.back()->getOrdNum();
}

Section * SectionTable::findSectionByName(string name) {
    Section* sym = nullptr;
    for (list<Section*>::iterator it = table.begin(); it != table.end(); ++it)
        if ((*it)->getName() == name) {
            sym = (*it);
            break;
        }
    return sym;
}
