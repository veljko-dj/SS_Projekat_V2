#include "RelEntry.h"

#include <string>
#include <iostream>

using namespace std;


string RelEntry::toString() {
    string abc = "id: ";
    abc += to_string(this->id);
    //std::cout << "EEE" << ord_num;
    abc += " secNum: ";
    abc += to_string(this->secNum);
    abc += " loc_offset: ";
    abc += to_string(this->locOffset);
    abc += " value: ";
    abc += to_string(this->value);
    abc += " addend: ";
    abc += this->addend;
    abc += " type: ";
    if (this->type == RelEntry::R_X86_64_PC32) abc += "REL";
    else abc += "ABS";
    return abc;
}
