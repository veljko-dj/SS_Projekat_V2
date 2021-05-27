#include "MainClass_ASM.h"
#include "firstPass.h"
#include "parsedTokens.h"
#include "Symbol.h"
#include "SymbolTable.h"

#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;


using MC = MainClass_ASM;
using PT = ParsedTokens;
using ST = SymbolTable;


FirstPass::regexi FirstPass::mojRegex;
string FirstPass::currSection = "undefined";
int FirstPass::currOffset = 0;
int FirstPass::numOfLine = 0;
bool FirstPass::end = false;

void FirstPass::error(string msg, string line) {
    cout << "\t ERROR: FP:"<<msg<< " :" << line << endl
         << "\t Na liniji: " << numOfLine << endl;
    exit(0);
}

bool FirstPass::emptyLine(string line) {
    return regex_match(line, mojRegex.emptyLine) ? true: false;
}

// Koristi se u oneOperand() i twoOperand()
bool FirstPass::checkOperand(string line) {
    smatch match;
    int posOfPlus = 0;
    //<reg>
    regex_search(line, match, mojRegex.regDir);
    if (match.size() > 0) {
        currOffset += 2;
        PT::addValueToLastToken("POD_REG_DIR");
        PT::addValueToLastToken(match[0]);
        return true;
    }
    line = line.substr(line.find_first_not_of(" \t"));	// cplusplus.com
    string lineWithoutFirst = line.substr(1);

    string  lineWithoutFirstAndSecond= (line.length()>2) ? line.substr(2) : "";

    switch (line[0]) {
    case '$': // Znaci onda je neposredno?
        // PODATAK: $<literal>
        regex_search(lineWithoutFirst, match, mojRegex.literal);
        if (match.size() > 0) {
            PT::addValueToLastToken("POD_NEPOS_LIT");
            PT::addValueToLastToken(match[0]);
            currOffset += 5;
            return true;
        }
        // PODATAK: $<simbol>
        regex_search(lineWithoutFirst, match, mojRegex.simbol);
        if (match.size() > 0) {
            PT::addValueToLastToken("POD_NEPOS_SIM");
            PT::addValueToLastToken(match[0]);
            currOffset += 5;
            return true;
        }
        error("Neposredno adr, mora biti ili simbol ili literal", line);

    case '%': // Znaci onda je PC relativno?
        // PODATAK_ADRESA : %<simbol>
        regex_search(lineWithoutFirst, match, mojRegex.simbol);
        if (match.size() > 0) {
            PT::addValueToLastToken("PODiADR_PC_REL_SIM");
            PT::addValueToLastToken(match[0]);
            currOffset += 5;
            return true;
        }
        error("Sa % mora da ide iskljucivo simbol",line);
    case '[':
        if (lineWithoutFirst.find_first_of(']') == string::npos)
            error("Ne postoje i leva i desna zaagrada [] :", line);
        if (!emptyLine(lineWithoutFirst.substr(lineWithoutFirst.find_first_of(']') + 1)))
            error("Posle ] imas jos nesto", line);

        lineWithoutFirst = lineWithoutFirst.substr(0, // Proveri keca, prepravljeno u 0
                           lineWithoutFirst.find_first_of(']'));
        // Ovde imas bug, ako imas [r0]adasda9 ne izbacuje gresku
        // PODATAK: [<reg>]
        regex_search(lineWithoutFirst, match, mojRegex.regDir);
        if (match.size() > 0) {
            currOffset += 3;
            PT::addValueToLastToken("POD_REG_IND_BEZ_POM");
            PT::addValueToLastToken(match[0]);
            return true;
        }

        posOfPlus = lineWithoutFirst.find_first_of('+');
        if (posOfPlus == string::npos)
            error("Ne postoji +, a nije samo <reg>", line);
        {
            // Ovaj blok mora, jer ako nemas switch baca gresku nedef simbola
            string beforePlus = lineWithoutFirst.substr(0, posOfPlus);
            string afterPlus = lineWithoutFirst.substr(posOfPlus+1,
                               lineWithoutFirst.length());

            // PODATAK: [<reg>+...]
            regex_search(beforePlus, match, mojRegex.regDir);
            if (match.size() > 0) {
                beforePlus = match[0];	// Privremeno sacuvano
            } else
                error("Pre + mora da bude regDir", line);
            // PODATAK: [<reg>+<literal>]
            regex_search(afterPlus, match, mojRegex.literal);
            if (match.size() > 0) {
                PT::addValueToLastToken("POD_REG_DIR_PLUS_LIT");
                PT::addValueToLastToken(beforePlus);
                PT::addValueToLastToken(match[0]);
                currOffset += 5;
                return true;
            } else {
                // PODATAK: [<reg>+<simbol>]
                regex_search(afterPlus, match, mojRegex.simbol);
                if (match.size() > 0) {
                    PT::addValueToLastToken("POD_REG_DIR_PLUS_SIM");
                    PT::addValueToLastToken(beforePlus);
                    PT::addValueToLastToken(match[0]);
                    currOffset += 5;
                    return true;
                }
                error("Posle + mora da bude literal ili simbol", line);
            }
        }

    case '*': // Znaci onda je?

        // ADRESA :*<reg>		-> reg Direktno
        regex_search(lineWithoutFirst, match, mojRegex.regDir);
        if (match.size() > 0) {
            PT::addValueToLastToken("ADR_REG_DIR");
            PT::addValueToLastToken(match[0]);
            currOffset += 3;
            return true;
        }
        // ADRESA :*<literal>	-> memorijsko
        regex_search(lineWithoutFirst, match, mojRegex.literal);
        if (match.size() > 0) {
            PT::addValueToLastToken("ADR_MEM_LIT");
            PT::addValueToLastToken(match[0]);
            currOffset += 5;
            return true;
        }
        // ADRESA :*<simbol>	-> memorijsko
        regex_search(lineWithoutFirst, match, mojRegex.simbol);
        if (match.size() > 0) {
            PT::addValueToLastToken("ADR_MEM_SIM");
            PT::addValueToLastToken(match[0]);
            currOffset += 5;
            return true;
        }

        // ADRESA :*[...]
        switch (lineWithoutFirst[0]) {
        case '[':
            if (lineWithoutFirstAndSecond.find_first_of(']') == string::npos)
                error("Ne postoje i leva i desna zaagrada[]", line);
            if (!emptyLine(lineWithoutFirst.substr(lineWithoutFirst.find_first_of(']') + 1)))
                error("Posle ] imas jos nesto", line);

            lineWithoutFirstAndSecond = lineWithoutFirstAndSecond.substr(0, // Proveri keca
                                        lineWithoutFirstAndSecond.find_first_of(']'));

            // ADRESA :*[<reg>]
            regex_search(lineWithoutFirstAndSecond, match, mojRegex.regDir);
            if (match.size() > 0) {
                currOffset += 3;
                PT::addValueToLastToken("ADR_REG_IND_BEZ_POM");
                PT::addValueToLastToken(match[0]);
                return true;
            }

            posOfPlus = lineWithoutFirstAndSecond.find_first_of('+');
            if (posOfPlus == string::npos)
                error("Ne postoji +, a nije samo <reg>", line);
            {
                string beforePlus = lineWithoutFirstAndSecond.substr(0, posOfPlus);
                string afterPlus = lineWithoutFirstAndSecond.substr(posOfPlus+1,
                                   lineWithoutFirstAndSecond.length());

                // ADRESA :*[<reg>+...]
                regex_search(beforePlus, match, mojRegex.regDir);
                if (match.size() > 0) {
                    beforePlus = match[0];	// Privremeno sacuvano
                } else
                    error("Pre + mora da bude regDir", line);
                // ADRESA :*[<reg>+<literal>]
                regex_search(afterPlus, match, mojRegex.literal);
                if (match.size() > 0) {
                    PT::addValueToLastToken("ADR_REG_DIR_PLUS_LIT");
                    PT::addValueToLastToken(beforePlus);
                    PT::addValueToLastToken(match[0]);
                    currOffset += 5;
                    return true;
                } else {
                    regex_search(afterPlus, match, mojRegex.simbol);
                    if (match.size() > 0) {
                        // ADRESA :*[<reg>+<simbol>]
                        PT::addValueToLastToken("ADR_REG_DIR_PLUS_SIM");
                        PT::addValueToLastToken(beforePlus);
                        PT::addValueToLastToken(match[0]);
                        currOffset += 5;
                        return true;
                    }
                    error("Posle + mora da bude literal ili simbol", line);
                }
            }
        default:
            error("Ko zna sta si ukucao u ovom redu", line);
        }

        error("Operand ne valja", line);

    default:
        regex_search(line, match, mojRegex.literal);
        if (match.size() > 0) {
            PT::addValueToLastToken("LIT");
            PT::addValueToLastToken(match[0]);
            currOffset += 5;
            return true;
        } else {
            regex_search(line, match, mojRegex.simbol);
            if (match.size() > 0) {
                // ADRESA :*[<reg>+<simbol>]
                PT::addValueToLastToken("SIM");
                PT::addValueToLastToken(match[0]);
                currOffset += 5;
                return true;
            }
        }
        error("Operand nije ni simbol ni literal, niti pocinje karak znakom", line);
        return false;
    }
}

bool FirstPass::label(string line) {
    smatch match;

    if (regex_match(line,mojRegex.labelLineOnly)
            || regex_match(line, mojRegex.labelLineWithCommand)) {
        regex_search(line, match, mojRegex.identfier);

        PT::addNextToken(PT::LABEL, match[0], numOfLine);
        if (currSection=="undefined")
            error("Ne mozes da definies labelu ukoliko se"
                  "ne nalazis u nekoj sekciji.", line);
        Symbol* foundSym = ST::findSymbolByName(match[0]);
        if ( foundSym!= nullptr ) {
            if (foundSym->getSectionName() != "undefined")
                error("Vec postoji labela u tabeli simbola", line);
            // Pronasao si globalan, postavi vrednosti lepo
            foundSym->setValOff(currOffset);
            foundSym->setSection(currSection);
        } else {
            // Nepostoji simbol, ubaciti ga
            int ord_num = ST::getLastOrdNum() + 1;
            string name = match[0];
            int size = 0;			// nebitno je za simbol
            char type = 'l';		// simbol
            bool isLocal = true;
            {
                // Dodavanje u tabelu simbola
                Symbol* sym = new Symbol(ord_num, name, currOffset, size, type, isLocal, currSection);
                ST::addSymbol(sym);
            }
        }
        return true;
    } else
        return false;


}

// Brise labelu iz reda koji sadrzi i labelu i komandu neku
// Vraca "" ukoliko je to red samo sa labelom
string FirstPass::deleteLabelFromCommand(string line) {
    if (regex_match(line, mojRegex.labelLineOnly))
        return "";
    if (regex_match(line, mojRegex.labelLineWithCommand)) {
        string newLine = line.substr(1+line.find(":"));	//dodajem 1 da preskocim :
        return newLine;
    }
    return "error"; // ovo postoji samo da bi se sklonio warning
}

// Vraca liniju koda bez jednolinijskog komentara
// Ukoliko ne pronadje # onda vraca ceo string jer nema komentara
string FirstPass::newLineWithoutComment(string line) {
    string newLine = line.substr(0, line.find("#"));
    return newLine;
}

bool FirstPass::globalDirective(string line) {
    smatch match;

    if (regex_match(line, mojRegex.global)) {
        // Uklanjanje global
        line = line.substr(line.find(".global")+7);
        PT::justCreateTokenWithNoValues(PT::GLOBAL, numOfLine);
        // pokupljen kod sa cplusplus.com regex_search
        while (regex_search(line, match, mojRegex.identfier)) {
            for (auto ident : match) {
                PT::addValueToLastToken(ident);

                {
                    Symbol* sym = ST::findSymbolByName(ident);
                    if (sym !=nullptr)
                        // Postoji i postavi da je globalan
                        sym->setIsLocal(false);

                    else {
                        // ne postoji, dodaj UNDEF simbol
                        int ord_num = ST::getLastOrdNum() + 1;
                        string name = ident;
                        int size = 0;
                        char type = 'g';
                        bool isLocal = false;
                        {
                            // Dodavanje u tabelu simbola
                            Symbol* sym = new Symbol(ord_num, name, 0, size, type, isLocal, "undefined");
                            ST::addSymbol(sym);
                        }
                    }
                }
            }
            line = match.suffix().str();	// bez ovoga vecna petlja
        }
        // Nije potrebno raditi u prvom prolazu, samo dodati token
        // predavanja 108/394
        // E PA JESTE POTREBNO ! ! !

        return true;
    } else
        return false;
}

bool FirstPass::externDirective(string line) {
    smatch match;

    if (regex_match(line, mojRegex.exterN)) {
        // Na predavanjima vidimo da treba da dodam u tabelu simbola,
        // Na predavanju je to poslednji ulaz u tabeli simbola
        // ali na netu ne pise da mora da bude poslednji ulaz i zato stavljam redom
        // (https://www.magmath.com/english/programming/c_programming_language/projects/two_pass_assembler.php)

        line = line.substr(line.find(".extern") + 7);
        PT::justCreateTokenWithNoValues(PT::EXTERN, numOfLine);
        // pokupljen kod sa cplusplus.com regex_search
        while (regex_search(line, match, mojRegex.identfier)) {
            for (auto ident : match) {
                PT::addValueToLastToken(ident);
                {
                    //privremeno resenje:
                    // Za extern uvozi simbole, prema vezbama(37/119), sekcija nepoznata
                    // vrednost 0, redni broj okej i GLOBAL je, sekcija UND
                    int ord_num = ST::getLastOrdNum()+1;
                    string name = ident;
                    int size = 0;			// nebitno za simbol
                    char type = 's';		// simbol
                    bool isLocal = false;
                    {
                        // Dodavanje u tabelu simbola
                        Symbol* sym = new Symbol(ord_num, name, 0/*ne znamo*/,
                                                 size, type, isLocal, "extern");
                        // "extern" naknadno dodato, nadam se da ne pravi problem nigde
                        ST::addSymbol(sym);
                    }
                }
            }
            line = match.suffix().str();	// bez ovoga vecna petlja
        }

        return true;
    } else
        return false;
}

bool FirstPass::sectionDirective(string line) {
    smatch match;

    if (regex_match(line, mojRegex.section)) {

        line = line.substr(line.find(".section") + 8);
        regex_search(line, match, mojRegex.identfier);
        PT::addNextToken(PT::SECTION, match[0], numOfLine);
        // Prethodnoj sekciji upisati trenutni offset kao velicinu
        if (currSection != "undefined") ST::findSymbolByName(currSection)->setSize(currOffset);
        if (ST::findSymbolByName(match[0]) == nullptr) {
            int ord_num = ST::getLastOrdNum() + 1;
            string name = match[0];
            int size = 0;			// Za sada je nula jer tek ubacujemo
            currSection = name;			// MENJAMO CURRSECTION
            currOffset = 0;
            char type = 'S';		// Sekcija
            bool isLocal = true;
            {
                // Dodavanje u tabelu simbola
                Symbol* sym = new Symbol(ord_num,name, currOffset, size, type, isLocal, currSection);
                ST::addSymbol(sym);
            }
        } else {
            // Vec postoji u tabeli simbola? Kako zasto?  Za sada je greska !
            // Mozda samo treba da se prebacis u tu sekciju i to je to
            error("Sekcija vec postoji sekcija u tabeli simbola", line);

        }
        return true;
    } else
        return false;
}

bool FirstPass::sectionDataDirective(string line) {
    if (regex_match(line, mojRegex.dataSection))
        return sectionDirective(".section data");
    return false;
}

bool FirstPass::sectionTextDirective(string line) {
    if (regex_match(line, mojRegex.textSection))
        return sectionDirective(".section text");
    return false;
}

bool FirstPass::sectionBssDirective(string line) {
    if (regex_match(line, mojRegex.bssSection))
        return sectionDirective(".section bss");
    return false;
}

bool FirstPass::wordDirective(string line) {
    smatch match;

    if (regex_match(line, mojRegex.word)) {
        // Fali ti deo da inicijalizujes alocirani prostor ovim identifikatorima
        line = line.substr(line.find(".word") + 5);
        PT::justCreateTokenWithNoValues(PT::WORD, numOfLine);
        // pokupljen kod sa cplusplus.com regex_search
        while (regex_search(line, match, mojRegex.identfierOrNumber)) {
            for (auto ident : match) {
                if (regex_match( (string)ident, mojRegex.literal))
                    PT::addValueToLastToken("NUM");
                else
                    PT::addValueToLastToken("SYM");
                PT::addValueToLastToken(ident);
                {
                    // Za sada samo odvajamo odredjeni prostor za svaki identifikator
                    // i nista vise ne deluje da treba? Za sada je to to.
                    currOffset += 2;
                }
            }
            line = match.suffix().str();	// bez ovoga vecna petlja
        }

        return true;
    } else
        return false;
}

bool FirstPass::skipDirective(string line) {
    smatch match;

    if (regex_match(line, mojRegex.skip)) {

        line = line.substr(line.find(".skip") + 5);
        regex_search(line, match, mojRegex.number);
        PT::addNextToken(PT::SKIP, match[0], numOfLine);
        {
            // Samo dodam na trenutni offset taj broj
            currOffset += stoi(match[0], nullptr,0);
        }
        return true;
    } else
        return false;
}

bool FirstPass::equDirective(string line) {
    smatch match;
    smatch matchLit;
    if (regex_match(line, mojRegex.equ)) {

        // Ovo afterComma je dobudzeno da bi equ proradio
        string afterComma = line.substr(line.find_first_of(',') + 1,
                                        line.length());
        line = line.substr(line.find(".equ") + 4);
        regex_search(line, match, mojRegex.identfier);
        regex_search(afterComma, matchLit, mojRegex.literal);

        PT::justCreateTokenWithNoValues(PT::EQU, numOfLine);
        PT::addValueToLastToken(match[0]);
        PT::addValueToLastToken(matchLit[0]);
        {
            // Nisam siguran sta ovde treba raditi,
            // na osnovu teksta zadatka, ubacicu u tabelu simbola,
            // stavicu apsolutnu sekciju i vrednost i boga pitaj sta dalje
            Symbol* foundSym = ST::findSymbolByName(match[0]);
            if (foundSym == nullptr) {
                int ord_num = ST::getLastOrdNum() + 1;
                string name = match[0];
                int size = 0;			// Nebitno
                char type = 's';		// e kao simbol iz equ
                bool isLocal = true;
                int val_off = stoi(matchLit[0], nullptr, 0);
                string section = "absolute";
                // "absolute" -> Predavanja -> str 12(85/349)
                // Apsolutnim simbolima ne treba relokacija
                {
                    // Dodavanje u tabelu simbola
                    Symbol* sym = new Symbol(ord_num, name, val_off, size, type, isLocal, section);
                    ST::addSymbol(sym);
                }
            } else {
                if (foundSym->getSectionName() != "undefined")
                    error("Vec postoji simbol u tabeli simbola", line);
                // nastavlja dalje ili kraj programa
                int val_off = stoi(matchLit[0], nullptr, 0);
                foundSym->setValOff(val_off);
                foundSym->setSection("absolute");
            }
        }
        return true;
    } else
        return false;

}

bool FirstPass::endDirective(string line) {
    if (regex_match(line, mojRegex.end)) {
        PT::addNextToken(PT::END,"_", numOfLine);
        if (currSection != "undefined") ST::findSymbolByName(currSection)->setSize(currOffset);
        //Symbol* lastSect = ST::getLastSection();
        //if (lastSect != nullptr) lastSect->setSize(currOffset);
        end = true;
        return true;
    } else return false;
}

bool FirstPass::noOperInstr(string line) {
    smatch match;
    if (regex_match(line, mojRegex.noOper)) {
        regex_search(line, match, mojRegex.identfier);
        if (!(match[0] == "halt" || (match[0] == "iret") || (match[0] == "ret")))
            return false;
        PT::addNextToken(PT::INSTR0, match[0], numOfLine);
        currOffset += 1;			// velicina ovih instrukcija je 1B
        return true;
    } else return false;
}

bool FirstPass::oneOperInstr(string line) {
    smatch match;
    if (regex_match(line, mojRegex.oneOper)) {
        regex_search(line, match, mojRegex.identfier);
        if (!(match[0] == "int" || (match[0] == "call") || (match[0] == "jmp")
                || (match[0] == "jeq") || (match[0] == "jne") || (match[0] == "jgt")
                || (match[0] == "push") || (match[0] == "pop") || (match[0]=="not")))
            return false;
        PT::addNextToken(PT::INSTR1, match[0], numOfLine);
        {
            // Ovo je dodato kasnije, kad sam skontao da push i pop nisu 2 bajta
            // vec su 3 bajta
            if (match[0] == "push" | match[0] == "pop")
                currOffset += 1; // a posle poveca za jos 2
        }
        line = line.substr(line.find(match[0]) + match[0].length());

        return checkOperand(line);
    } else return false;
}

bool FirstPass::twoOperInstr(string line) {
    smatch match;
    if (regex_match(line, mojRegex.twoOper)) {
        regex_search(line, match, mojRegex.identfier);
        if (!(	(match[0] == "xchg")
                || (match[0] == "add") || (match[0] == "sub") || (match[0] == "mul")
                || (match[0] == "div") || (match[0] == "cmp") /*|| (match[0] == "not")*/
                || (match[0] == "and") || (match[0] == "or") || (match[0] == "xor")
                || (match[0] == "test") || (match[0] == "shl") || (match[0] == "shr")
                || (match[0] == "ldr") || (match[0] == "str") ))
            return false;
        PT::addNextToken(PT::INSTR2, match[0], numOfLine);
        line = line.substr(line.find(match[0]) + match[0].length());

        int posOfComma = line.find_first_of(',');
        string beforeComma = line.substr(0, posOfComma);
        string afterComma = line.substr(posOfComma+1,
                                        line.length());
        //<reg>
        regex_search(beforeComma, match, mojRegex.regDir);
        if (match.size() > 0) {
            //PT::addValueToLastToken("PRVI_OPER(REG)");
            PT::addValueToLastToken(match[0]);
            return  checkOperand(afterComma);
        }
    }
    return false;
}

void FirstPass::testRegex() {
    cout << endl << "____TESTREGEX____" << endl;
    while (!MC::eofInput()) {	// do kraja fajla
        string line = MC::getInputLine();
        cout << line << endl;
        line = newLineWithoutComment(line);
        //cout << "BEZ_KOMENTARA_: " << line << endl;
        if (std::regex_match(line, mojRegex.global))
            std::cout << "\t \t \t \t uspesno uparivanje regexa"<< endl;
        else
            cout << "\t \t \t \t \t \t \t neuspesno uparivanje regexa" << endl;
    }
}

void FirstPass::startFirstPass() {
    // Da li je potrebno ubacivanje sekcije "undefined"?
    // Pa po Sasinom predavanju jeste, da bi ostala ta nula viska
    {
        int ord_num = 0;
        string name = "undefined";
        int size = 0;
        char type = 'S';
        bool isLocal = true;
        {
            Symbol* sym = new Symbol(ord_num, name, currOffset, size, type, isLocal, currSection);
            ST::addSymbol(sym);
        }
    }
    while (!MC::eofInput()) {	// do kraja fajla
        string line = MC::getInputLine();
        numOfLine++;
        //cout << line << numOfLines<< endl;
        line = newLineWithoutComment(line);
        //cout << "BEZ_KOMENTARA_: " << line << endl;
        if (!end) {
            // Ako nije bila .end direktiva analiziraj dalje
            // Ako jeste, samo kuliraj do citanja kraja fajla
            if (label(line))
                line = deleteLabelFromCommand(line);
            if (emptyLine(line)) continue;
            if (globalDirective(line)) continue;
            if (externDirective(line)) continue;
            if (sectionDirective(line)) continue;
            if (sectionBssDirective(line)) continue;
            if (sectionDataDirective(line)) continue;
            if (sectionTextDirective(line)) continue;
            if (wordDirective(line)) continue;
            if (skipDirective(line)) continue;
            if (equDirective(line)) continue;
            if (endDirective(line)) continue;
            if (noOperInstr(line)) continue;
            if (oneOperInstr(line)) continue;
            if (twoOperInstr(line)) continue;

            error("Leksicka greska, nije ni jedna instrukcija", line);

        }
    }
    cout << endl << "\tGotov Prvi prolaz";
}

