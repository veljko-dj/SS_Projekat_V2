//#include "../inc/Section.h"
//#include "../inc/SymbolTable.h"
//#include "../inc/Symbol.h"

#include "Section.h"
#include "SymbolTable.h"
#include "Symbol.h"

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include <set>

using namespace std;

// Proveri da li parametar moze da bude i u definiiciji i u deklaraciji =""
Section::Section(std::string nameP, std::string rwx  ) {
    Symbol* symbolSection = SymbolTable::findSymbolByName(nameP);
    if (symbolSection == nullptr) {
        cout << "\t ERROR: Section::section() :" << "Ne postoji sekcija" << endl;
        exit(0);
    }

    this->offsetToContinue = 0;
    this->name = nameP;
    this->sizeFromSymbolTable = symbolSection->getSize();
    this->ordNumInSymbolTable = symbolSection->getOrdNum();
    this->rwxNotFromSymbolTable = rwx;
    this->bytesInMemory.resize(this->sizeFromSymbolTable/*+10*/,0);	// Zasto dodajem 10, ne znam, onako

}

string Section::getName() {
    return this->name;
}

uint8_t Section::getByteInMemoryAt(int i) {
    if (i<0 || i>this->sizeFromSymbolTable) {
        cout << "\t ERROR: SECTION::getByteInMemory(int) :" << "Indeks niza nije validan" <<   endl;
        exit(0);
    }
    return this->bytesInMemory.at(i); // Moze i [i]
}

int Section::getOrdNum() {
    return this->ordNumInSymbolTable;
}

int Section::getSize() {
    return this->sizeFromSymbolTable;
}

string Section::getRWX() {
    return this->rwxNotFromSymbolTable;
}

int Section::getOffsetToContinue() {
    return this->offsetToContinue;
}

void Section::setName(std::string n) {
    this->name = n;
}

void Section::setByteInMemoryAt(int i, uint8_t b) {
    if (i<0 || i>this->sizeFromSymbolTable) {
        cout << "\t ERROR: SECTION::setByteInMemory(int) :" << "Indeks niza nije validan" << endl;
        exit(0);
    }
    this->bytesInMemory.at(i) = b; // Moze i [i]
    // Proveri ovo dobro, sta se desava ako nemas toliko elemenata jos
    // Provereno. Radi okej ali ne menja size, zato treba odraditi resize pri inicijalizaciji
}
// Ovo je metoda koja postavlja za instrukcije word,
// sto znaci nije LITTLE ENDIAN
void Section::setWordInMemoryAt(int i, uint16_t b) {
    if (i<0 || i>this->sizeFromSymbolTable) {
        cout << "\t ERROR: SECTION::setByteInMemory(int) :" << "Indeks niza nije validan" << endl;
        exit(0);
    }
    //cout << "HEX:"<<hex << b << endl;
    this->bytesInMemory.at(i) = (uint8_t)(b >> 8); // Moze i [i]
    //cout << "HEX:" << hex << (b>>8) << endl;
    this->bytesInMemory.at(i+1) = (uint8_t) b;
    //cout << "_____" << endl;
    //this->printBytes();
    //cout << "_____ASCII" << endl;
    //this->printBytesASCII();
}

void Section::setWordInMemoryAt_l_endian(int i, uint16_t b) {
    if (i<0 || i>this->sizeFromSymbolTable) {
        cout << "\t ERROR: SECTION::setByteInMemory(int) :" << "Indeks niza nije validan" << endl;
        exit(0);
    }
    //cout << "HEX:"<<hex << b << endl;
    this->bytesInMemory.at(i) = (uint8_t) b ; // Moze i [i]
    //cout << "HEX:" << hex << (b>>8) << endl;
    this->bytesInMemory.at(i+1) = (uint8_t) (b >> 8);
    //cout << "_____" << endl;
    //this->printBytes();
    //cout << "_____ASCII" << endl;
    //this->printBytesASCII();
}

void Section::setRWX(string s) {
    this->rwxNotFromSymbolTable = s;
}

void Section::setOffsetToContinue(int s) {
    this->offsetToContinue = s;
}

void Section::addNextCR(int cO) {
    nextCR.insert(cO);
}


string Section::toString() {
    string abc = "";
    abc += "SectionName: ";
    string tmpName = name;	// Da bi sacuvao name bez razmaka
    tmpName.insert(tmpName.size(), 15 - tmpName.size(), ' ');
    abc += tmpName;
    abc += "\t size(decimal): ";
    abc += to_string(this->sizeFromSymbolTable);
    // abc += " RWX: ";
    // abc += this->rwxNotFromSymbolTable;

    // std::string bytesToString(this->bytesInMemory.begin(), this->bytesInMemory.end());
    // abc += '\n';
    // abc += bytesToString;
    return abc;
}

void Section::printBytesDec(std::ostream& out) {
    out << endl << "BYTES: Dec:" << endl << "0: ";
    int num = 0;
    for (int i : this->bytesInMemory) {	// stackoverflow.com/ Converting string to ASCII
        out << setfill(' ') << setw(3) << dec << i << " ";
        bool writeCR = nextCR.find(num++) != nextCR.end();
        if (writeCR)
            out << "\n" << hex << (short) num << ": ";
        // out << (writeCR ? ("\n" + to_string(num) + ": ") : "");
    }
    cout << endl;
}
void Section::printBytesASCIIHex(std::ostream& out) {
    if (this->bytesInMemory.size()==0) return;
    out << "Data: " << endl << "\t\t0: ";
    int num = 0;
    for (int i : this->bytesInMemory) {	// stackoverflow.com/ Converting string to ASCII
        out << setfill('0') << setw(2) << hex << i << " ";
        bool writeCR = nextCR.find(num++) != nextCR.end();
        if (writeCR)
            out << "\n\t\t" << hex << (short) num << ": ";
        // out << (writeCR ? ("\n\t\t" + to_string(num) + ": ") : "");
    }
    out << endl;
}

void Section::printBytesASCIIHexSquare16(std::ostream& out) {
    out << "Data:" << endl << "\t\t 0: ";
    int num = 0;
    for (int i : this->bytesInMemory) {	// stackoverflow.com/ Converting string to ASCII
        out << setfill('0') << setw(2) << hex << i << " ";
        bool writeCR = (num++)%16 == 15;
        if (writeCR)
            out << "\n\t\t" << hex << (short) num << ": ";
        // out << (writeCR ? ("\n\t\t" + to_string(num) + ": ") : "");
    }
    out << endl;
}