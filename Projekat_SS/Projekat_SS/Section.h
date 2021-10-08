#pragma once
#ifndef SECTION_H
#define SECTION_H

#include <vector>
#include <string>
#include <set>


class Section {
    // Ne koristim glupu listu jer nema get, niz da koristim je zeznuto
    // a vector ima get, Nisam siguran da mora std::
    //Zasto nece byte?
    std::vector<std::uint8_t> bytesInMemory ; //Bytes	//LITTLE ENDIAN
    std::string name;
    // ovo dole sada su mozda nebitne info ali zasto da ne postoje
    int offsetToContinue;
    int ordNumInSymbolTable;
    int sizeFromSymbolTable;
    std::string rwxNotFromSymbolTable;

    std::set<int> nextCR;	// carriage return // OVO TI NE TREBA ! MIKSA ! 

public:
    Section(std::string nameP,std::string rwx = "");

    //getters
    std::string getName();
    uint8_t getByteInMemoryAt(int i);
    int getOrdNum();
    int getSize();
    std::string getRWX();
    int getOffsetToContinue();
    //setters
    void setName(std::string n);
    void setByteInMemoryAt(int i, uint8_t b);
    void setWordInMemoryAt(int i, uint16_t b);
    void setWordInMemoryAt_l_endian(int i, uint16_t b);
    void setRWX(std::string s);
    void setOffsetToContinue(int s);
    //methods
    void addNextCR(int);
    std::string toString();
    void printBytesDec(std::ostream& out);
    void printBytesASCIIHex(std::ostream& out);
    void printBytesASCIIHexSquare16(std::ostream& out);
};
#endif // !SECTION_H


