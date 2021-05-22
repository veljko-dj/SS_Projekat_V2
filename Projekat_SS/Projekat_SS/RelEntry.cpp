#include "RelEntry.h"

#include <string>
#include <iostream>

using namespace std;


RelEntry::RelEntry(std::string secNum1, int id1, int locOffset1, Type type1, int symbolOrdNum1 ) {
    secName = secNum1;
    ordNum = id1;
    locOffset= locOffset1;
    type= type1;
    symbolOrdNum = symbolOrdNum1;
}

string RelEntry::toString() {
    string abc = "ordNum: ";
    abc += to_string(this->ordNum);
    //std::cout << "EEE" << ord_num;
    abc += " secName: ";
    abc += this->secName;
    abc += " loc_offset: ";
    abc += to_string(this->locOffset);
    abc += " Sym to ref in SymTable: ";
    abc += to_string(this->symbolOrdNum);
    abc += " type: ";
    if (this->type == RelEntry::R_X86_64_PC32) abc += "REL";
    else if (this->type == RelEntry::R_X86_64_16) abc += "D16";
    else abc += "D32"; // ABS nisam stavio zbog apsolutne sekcije
    return abc;
}
