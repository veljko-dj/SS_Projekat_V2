#pragma once
#ifndef REL_TABLE_H
#define REL_TABLE_H

#include <list>
#include <fstream>
#include <iostream>
#include <string>

class RelEntry;
class RelTable {
    static std::list<RelEntry*> table;		// Radim sa pokazivacem da ne bi doslo do nekih kopiranja neplaniranih

public:
    static void addEntry(RelEntry* symbolPtr);
    static int getLastOrdNum();
    static RelEntry* getLastEntry(); 
    static RelEntry* findRelEntryByOrdNum(int id);
    static void printRelTable(std::ostream& out);
    static void printRelTableForSection(std::ostream& out,std::string nameOfSection, 
                                        int sizeOfSection);
};


#endif // !REL_TABLE_H