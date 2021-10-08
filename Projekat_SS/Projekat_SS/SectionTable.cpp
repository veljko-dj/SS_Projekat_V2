//#include "../inc/SectionTable.h"
//#include "../inc/Section.h"
//#include "../inc/RelTable.h"

#include "SectionTable.h"
#include "Section.h"
#include "RelTable.h"


#include <list>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

list<Section*> SectionTable::table;


void SectionTable::addSection(Section * symbolPtr) {
    table.push_back(symbolPtr);
}

Section* SectionTable::getLastSection() {
    if (table.size() > 0) return table.back();
    return nullptr;
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


void SectionTable::printSections(std::ostream& out) {
    for (list<Section*>::iterator it = table.begin(); it != table.end(); ++it) {
        out << "_______________________________________________________" << endl << endl;
        out << (*it)->toString() << endl;
        (*it)->printBytesASCIIHex(out);
        //(*it)->printBytesASCIIHexToFileSquare16();	//Squared PRINT

        RelTable::printRelTableForSection(out,(*it)->getName(), (*it)->getSize());
    }
}