#pragma once
#ifndef RELENTRY_H
#define RELENTRY_H

#include <string>

class Symbol;
class Section;
class RelEntry {
public:
    enum Type {R_X86_64_PC32, R_X86_64_32, R_X86_64_16}; //  Ovo poslednje sam dodao za .word

private:
    // Ovo je da vidim za koju sekciju je vezan ovaj relZapis
    std::string secName;
    int ordNum; // Ovo je id ulaza u RelTable, za sada nepotrebno
    // Ovo su parametri koje je Sasa dao na predavanjima
    int locOffset; // Mesto na kojem se upisuje
    Type type;
    int symbolOrdNum; // Redni broj simbola u tab. simbola
    // std::string symbolName; Ovo uklanjam da ne zbunjuje
    //int addend; // zavisi od arh. Ovo mozes odmah u value da upises

    // Ovo dalje su moji parametri, mozda nepotrevni
    Symbol* symbolInSymTable;
    Section* sectionWhereRelEntryIsCreated;

    friend class RelTable;

public:
    RelEntry(std::string secName, int ordNum, int locOffset, Type type, int symbolOrdNum/*, std::string symbolName, int addend*/);
    std::string toString();

};


#endif // !RELENTRY_H