#include "Symbol.h"

#include <iostream>
#include <string>
#include <algorithm>

Symbol::Symbol(int o_n, string n, int v_o, int s, char t, bool l, string sec) {
    ord_num = o_n;
    {
        //Ovo nije bas isproveravano ovde u simbolu
        //Nemoj da ti pobrise nesto vise, mada ime ne sme da sastoji ' '
        n.erase(remove(n.begin(), n.end(), ' '), n.end());
        n.erase(remove(n.begin(), n.end(), '\t'), n.end());
        name = n;
    }
    value_offset = v_o;
    size = s;
    type = t;
    isLocal = l;
    section = sec;
}

string Symbol::toString() {
    string abc = "ord_num: ";
    abc += to_string(ord_num);
    //std::cout << "EEE" << ord_num;
    abc += " name: ";
    string tmpName = name;	// Da bi sacuvao name bez razmaka
    tmpName.insert(tmpName.size(), 15 - tmpName.size(), ' ');
    abc += tmpName;
    abc += "\t value_offset: ";
    abc += to_string( value_offset);
    abc += " size: ";
    abc += to_string(this->size);
    abc += " type: ";
    abc += type;
    abc += " isLocal: ";
    abc += to_string(isLocal);
    abc += " section: ";
    abc += section;
    return abc;
}

int Symbol::getOrdNum() {
    return ord_num;
}

string Symbol::getName() {
    return name;
}

int Symbol::getValOff() {
    return value_offset;
}

int Symbol::getSize() {
    return this->size;
}

char Symbol::getType() {
    return type;
}

bool Symbol::getIsLocal() {
    return isLocal;
}

string Symbol::getSectionName() {
    return section;
}

void Symbol::setOrdNum(int s) {
    this->ord_num = s;
}

void Symbol::setName(string n) {
    this->name = n;
}

void Symbol::setValOff(int v_o) {
    this->value_offset = v_o;
}

void Symbol::setSize(int s) {
    this->size = s;
}

void Symbol::setType(char t) {
    this->type = t;
}

void Symbol::setIsLocal(bool l) {
    this->isLocal = l;
}

void Symbol::setSection(string s) {
    this->section = s;
}

