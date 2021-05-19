#pragma once
#ifndef RELENTRY_H
#define RELENTRY_H

#include <string>

class Symbol;
class Section;
class RelEntry {
public:
    enum Type {R_X86_64_2, R_X86_64_PC32}; // apsoltenoPC i relativno

private:
    // Ovo je da vidim za koju sekciju je vezan ovaj relZapis
    int secNum;
    int id; // Ovo je id ulaza u RelTable, za sada nepotrebno
    // Ovo su parametri koje je Sasa dao na predavanjima
    int locOffset; // Mesto na kojem se upisuje
    Type type;
    int value; // Vrednost koja treba da se upise
    int addend; // zavisi od arh. Ovo mozes odmah u value da upises

    // Ovo dalje su moji parametri, mozda nepotrevni
    Symbol* symbolInSymTable;
    Section* sectionWhereRelEntryIsCreated;

    friend class RelTable;

public:
    std::string toString();

};


#endif // !RELENTRY_H