#include "SecondPass.h"
#include "parsedTokens.h"
#include "SymbolTable.h"
#include "Section.h"
#include "SectionTable.h"
#include "Symbol.h"
#include "RelEntry.h"
#include "RelTable.h"

#include <string>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

//using MC = MainClass;
using PT = ParsedTokens;
using ST = SymbolTable;
using SecT = SectionTable;
using RT = RelTable;



string SecondPass::currSectionName = "undefined";
int SecondPass::currSecNum = 0;
Section* SecondPass::currSection = nullptr;
int SecondPass::currOffset = 0;
bool SecondPass::end = false;

void SecondPass::error(string msg, int numOfLine) {
    cout << "\t ERROR: SP: " << msg << endl
         << "\t Na liniji: " << numOfLine << endl;
    exit(0);
}

void instr0(PT::Tok token) {

}

void instr1(PT::Tok token) {
}

void instr2(PT::Tok token) {
}

void SecondPass::startSecondPass() {
    cout << endl<<endl<<
         "Poceo je drugi prolaz" << endl;

    while (!PT::isEmpty() && !end) {
        PT::Tok token = PT::getNextToken();
        switch (token.type) {
        case PT::LABEL: {
            // Ovde ne vidim potrebu da bilo sta raddis
            break;
        }
        case PT::GLOBAL: {
            while (!token.values.empty()) {
                string name = token.getFrontValue();
                Symbol* sym = ST::findSymbolByName(name);
                if (sym == nullptr) error("Simbol koji kazemo da je globalan"
                                              " nije definisan, ne postoji u tabeli simbola",
                                              token.numOfLine);
                if (sym->getType() == 'S') error("Ne mozes sekciju proglasiti globalnom",
                                                     token.numOfLine);
                if (sym->getIsLocal() == false) error("Vec si negde ovaj simbol proglasio globalnim",
                                                          token.numOfLine);
                sym->setIsLocal(false);
            }
            break;
        }
        case PT::EXTERN: {
            while (!token.values.empty()) {
                // Ovo napisah onako bezveze, mislim da moze sve i prazno da bude
                string name = token.getFrontValue();
                Symbol* sym = ST::findSymbolByName(name);
                if (sym == nullptr) error("Nesto je krenulo po zlu, extern nije u tab simbola",
                                              token.numOfLine);
                if (sym->getIsLocal()) error("Nesto je krenulo po zlu, extern a local",
                                                 token.numOfLine);
            }
            break;
        }
        case PT::SECTION: {
            // Ovde ce uci i za BSS sekciju !
            string nameOfSection = token.getFrontValue();
            Section* newSec = new Section(nameOfSection);
            SecT::addSection(newSec);

            currOffset = 0;
            currSectionName = nameOfSection;
            currSecNum = newSec->getOrdNum();
            currSection = newSec;
            break;
        }
        case PT::WORD: {
            while (!token.values.empty()) {
                if (token.getFrontValue()== "NUM") {
                    int value = stoi(token.getFrontValue());
                    currSection->setWordInMemoryAt(currOffset,value);
                } else {
                    string value= token.getFrontValue();
                    Symbol* sym = ST::findSymbolByName(value);
                    if (sym == nullptr) SecondPass::error("Simbol nije definisan",token.numOfLine);
                    // Da li je definisan, da li je globalan, sta radit? Da li je ABS?
                    if (sym->getSectionName().compare("absolute")==0)
                        currSection->setWordInMemoryAt(currOffset, sym->getValOff());
                    else if (sym->getIsLocal()) { // trebalo bi da je vec definisan
                        // Ako je lokalan, onda upisujem taj offset na to mesto i
                        // dodajem rel zapis u koji staljam sekciju te ce se adresa
                        // te sekcije samo dodati posle
                        int sectionToZakrpiti = ST::findSymbolByName(sym->getSectionName())->getOrdNum();

                        currSection->setWordInMemoryAt(currOffset, sym->getValOff());
                        RelEntry* relEntry = new RelEntry(currSection->getName(),
                                                          RT::getLastOrdNum()+1,currOffset,
                                                          RelEntry::R_X86_64_2, sectionToZakrpiti);
                        // JEDNOM ILI 2 puta se ovo poziva???????
                        RT::addEntry(relEntry);
                    } else {	//globalan
                        // Za sada cu isto odraditi, tehnicki mozes i da upises nulu pa
                        // relokacioni zapis koji ce referisati na tu adresu da se zakrpi
                        // novom adresom tog globalnog
                        int sectionToZakrpiti = ST::findSymbolByName(sym->getSectionName())->getOrdNum();

                        currSection->setWordInMemoryAt(currOffset, sym->getValOff());
                        RelEntry* relEntry = new RelEntry(currSection->getName(),
                                                          RT::getLastOrdNum() + 1, currOffset,
                                                          RelEntry::R_X86_64_2, sectionToZakrpiti);
                        // JEDNOM ILI 2 puta se ovo poziva???????
                        // Ovde moze da bude greska, linker ce misliti da se radi o
                        // 32memoriji
                        RT::addEntry(relEntry);
                    }
                }
                currOffset += 2;
            }
            break;
        }
        case PT::SKIP: {
            int size = stoi(token.getFrontValue());
            for (int i = 0; i < size; i++) {
                currSection->setByteInMemoryAt(currOffset++, 0);
            }
            //currSection->printBytesASCII();
            break;
        }
        case PT::EQU: {
            // Nista?
            break;
        }
        case PT::END: {
            end = true;
            break;
        }
        case PT::INSTR0: {
            //void instr0(PT::Tok token) {
            string instr = token.getFrontValue();
            cout << "INSTR " << instr << endl;
            if (instr.compare("halt") == 0) {
                currSection->setByteInMemoryAt(currOffset, 0);
            } else if (instr.compare("iret") == 0) {
                currSection->setByteInMemoryAt(currOffset, 0x20);
            } else if (instr.compare("ret") == 0) {
                currSection->setByteInMemoryAt(currOffset, 0x40);
            }
            currOffset++;
            //}
            break;
        }
        case PT::INSTR1: {
            string instr = token.getFrontValue();
            cout << "INSTR " << instr << endl;
            if (instr.compare("int") == 0) {
                string addrType = token.getFrontValue();
                if (addrType.compare("POD_REG_DIR") != 0)
                    error("Uz insrukciju int ide samo reg dir", token.numOfLine);
                uint8_t regNum = stoi(token.getFrontValue().substr(1));
                if (regNum > 255) error("Preveliki broj registra, ! <256", token.numOfLine);
                uint16_t valToWrite = 0x100F;
                valToWrite |= (regNum << 4);
                //cout << "aaa" << endl << to_string(valToWrite) << endl;
                currSection->setWordInMemoryAt(currOffset, valToWrite);
                currOffset += 2;
            } else if (instr.compare("call") == 0) {
                string addrType = token.getFrontValue();
                if (addrType == "ADR_REG_DIR") {
                    uint8_t regNum = stoi(token.getFrontValue().substr(1));
                    if (regNum > 8) error("Preveliki broj registra, ! <8", token.numOfLine);
                    uint8_t byteOpMod = 0x30;	// poziv potp
                    uint8_t byteRdRs = 0xF0;	// Dest: fix , Source : Reg
                    byteRdRs |= regNum;
                    uint8_t byteUpAddr = 0x01;	// Update: /, Reg dir

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteRdRs);

                    // Ovo ti ne valja u prvom prolazu !
                    currOffset += 3;
                } else
                    error("Ovaj vid adresiranja za skok nije dozvoljen", token.numOfLine);

            } else if (instr.compare("jmp") == 0) {
                currSection->setByteInMemoryAt(currOffset, 0x40);
            } else if (instr.compare("jeq") == 0) {
                currSection->setByteInMemoryAt(currOffset, 0x40);
            } else if (instr.compare("jne") == 0) {
                currSection->setByteInMemoryAt(currOffset, 0x40);
            } else if (instr.compare("jgt") == 0) {
                currSection->setByteInMemoryAt(currOffset, 0x40);
            } else if (instr.compare("push") == 0) {
                string addrType = token.getFrontValue();
                if (addrType.compare("POD_REG_DIR") != 0)
                    error("Uz insrukciju push ide samo reg dir", token.numOfLine);
                uint8_t regNum = stoi(token.getFrontValue().substr(1));
                if (regNum > 8) error("Preveliki broj registra, ! <8", token.numOfLine);
                uint8_t byteOpMod = 0xB0;	// Upis u mem
                uint8_t byteRdRs = 0x06;	// Dest: Reg, Source : SP
                byteRdRs |= (regNum<<4);
                uint8_t byteUpAddr = 0x12;	// Update: --SP, Reg ind

                currSection->setByteInMemoryAt(currOffset, byteOpMod);
                currSection->setByteInMemoryAt(currOffset+1, byteRdRs);
                currSection->setByteInMemoryAt(currOffset+2, byteUpAddr);

                // Ovo ti ne valja u prvom prolazu ! Trebalo bi da sam popravio
                currOffset += 3;
            } else if (instr.compare("pop") == 0) {
                string addrType = token.getFrontValue();
                if (addrType.compare("POD_REG_DIR") != 0)
                    error("Uz insrukciju push ide samo reg dir", token.numOfLine);
                uint8_t regNum = stoi(token.getFrontValue().substr(1));
                if (regNum > 8) error("Preveliki broj registra, ! <8", token.numOfLine);
                uint8_t byteOpMod = 0xA0;	// Citanje iz mem
                uint8_t byteRdRs = 0x06;	// Dest: Reg , Source : SP
                byteRdRs |= (regNum<<4);
                uint8_t byteUpAddr = 0x42;	// Update: SP++, Reg ind

                currSection->setByteInMemoryAt(currOffset, byteOpMod);
                currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);

                // Ovo ti ne valja u prvom prolazu ! Trebalo bi da sam popravio
                currOffset += 3;
            }
            break;
        }
        case PT::INSTR2: {
            instr2(token);
            break;
        }
        default:
            error("nepoznata direktiva", token.numOfLine);
        }
    }



}
