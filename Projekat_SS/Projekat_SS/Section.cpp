#include "Section.h"
#include "SymbolTable.h"
#include "Symbol.h"

#include <string>
#include <iostream>
#include <vector>

using namespace std;

// Proveri da li parametar moze da bude i u definiiciji i u deklaraciji =""
Section::Section(std::string nameP, std::string rwx  ) {
    Symbol* symbolSection = SymbolTable::findSymbolByName(nameP);
    if (symbolSection == nullptr) {
        cout << "\t ERROR: Section::section() :" << "Ne postoji sekcija" << endl;
        exit(0);
    }

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
void Section::setWordInMemoryAt(int i, uint16_t b) {
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

string Section::toString() {
    string abc = "";
    abc += "name: ";
    string tmpName = name;	// Da bi sacuvao name bez razmaka
    tmpName.insert(tmpName.size(), 15 - tmpName.size(), ' ');
    abc += tmpName;
    abc += "\t size: ";
    abc += to_string(this->sizeFromSymbolTable);
    abc += " RWX: ";
    abc += this->rwxNotFromSymbolTable;
    std::string bytesToString(this->bytesInMemory.begin(), this->bytesInMemory.end());
    // Ovo dobro proveri u jednom online kompajleru izbacuje space u nedodeljenim lokacijama
    // a u drugom kao da nista ne postoji pa ti stoti elem kod praznog vektora kad ispises bude prvi
    abc += '\n';
    abc += bytesToString;
    return abc;
}

void Section::printBytes() {
    std::string bytesToString(this->bytesInMemory.begin(), this->bytesInMemory.end());
    cout << endl << "|" <<bytesToString << "|"<<endl;
}
void Section::printBytesASCII() {
    cout << endl << "BYTES: ASCII:" << endl << "|";
    for (int i : this->bytesInMemory) {	// stackoverflow.com/ Converting string to ASCII
        std::cout << i   << ".";
    }
    cout << "|" << endl;
}
