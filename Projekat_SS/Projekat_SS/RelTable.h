#pragma once
#ifndef REL_TABLE_H
#define REL_TABLE_H

#include <list>

class RelEntry;
class RelTable {
    static std::list<RelEntry*> table;		// Radim sa pokazivacem da ne bi doslo do nekih kopiranja neplaniranih

public:
    static void addEntry(RelEntry* symbolPtr);
    static RelEntry* getLastEntry();
    static void  printRelTable();
    static RelEntry* findRelEntryById(int id);

};


#endif // !REL_TABLE_H