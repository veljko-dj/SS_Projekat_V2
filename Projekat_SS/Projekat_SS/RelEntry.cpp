#include "../h/RelEntry.h"
#include "../h/SymbolTable.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>

using namespace std;


RelEntry::RelEntry(std::string secNum1, int id1, int locOffset1, Type type1, int symbolOrdNum1, string symbolName1) {
    secName = secNum1;
    ordNum = id1;
    locOffset= locOffset1;
    type= type1;
    symbolOrdNum = symbolOrdNum1;
    symbolName = symbolName1;
}

void RelEntry::toString(ostream &out) {
    out << "\t" << setw(8) << to_string(this->ordNum);
    out << "\t" << setw(8) << to_string(this->locOffset);

    string abc;
    if (this->type == RelEntry::R_PC16) abc += "REL";
    else if (this->type == RelEntry::R_16) abc += "D16";
    else abc += "D32"; // ABS nisam stavio zbog apsolutne sekcije


    out << "\t" << setw(7) << abc;
    out << "\t" << setw(8) << to_string(this->symbolOrdNum) << " ("
        << this->symbolName << ")";
    out << "\t" << endl;

}
