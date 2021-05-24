#pragma once
#ifndef SECTION_TABLE_H
#define SECTION_TABLE_H

#include <list>
#include <string>

class Section;

class SectionTable {
    static std::list<Section*> table;		// Radim sa pokazivacem da ne bi doslo do nekih kopiranja neplaniranih
    // za sada ne treba nista vise od informacija
public:
    static void addSection(Section* symbolPtr);
    static Section* getLastSection();
    static int getLastOrdNum();
    static Section* findSectionByName(std::string name);
    static void  printSections(std::ostream& out);
};


#endif // !SECTION_TABLE_H
