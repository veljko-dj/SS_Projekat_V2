#pragma once
#include <string>
using namespace std;
class Section;
// Ime klase kao i sama klasa pokupljeni su sa predavanja
// strana 12/13
class Symbol {
    int ord_num;
    string name;	// Ime sekcije
    int value_offset;		// Section offset
    int size;		// Velicina u bajtovima
    char type;		// Tip, ne znam da li mi je ovo potrebno
    // Za sada je l labela, s simbol, S sekcija
    bool isLocal;	// Da li je globalan ili lokalan simbol
    string section;	// Kako staviti sekciju? String/int/Section* ?
    // Za sada neka bude samo string

public:
    Symbol(int o_n, string n, int v_o, int s, char t, bool l, string sec);
    string toString();


    //Za sada mi nisu potrebni svi ovi getteri i setteri ali jebes ga
    //getters
    int getOrdNum();
    string getName();
    int getValOff();
    int getSize();
    char getType();
    bool getIsLocal();
    string getSection();
    //setters
    void setOrdNum(int s);
    void setName(string n);
    void setValOff(int);
    void setSize(int s);
    void setType(char t);
    void setIsLocal(bool l);
    void setSection(string s) ;

};
// U tabeli simbola na 13 str vidimo da prvi ulaz je neki UNDEFINED

