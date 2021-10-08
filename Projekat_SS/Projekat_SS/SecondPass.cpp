//#include "../inc/SecondPass.h"
//#include "../inc/parsedTokens.h"
//#include "../inc/SymbolTable.h"
//#include "../inc/Section.h"
//#include "../inc/SectionTable.h"
//#include "../inc/Symbol.h"
//#include "../inc/RelEntry.h"
//#include "../inc/RelTable.h"

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
    cout << "\n\t ERROR: SP: " << msg << endl
         << "\t Na liniji: " << numOfLine << endl;
    exit(0);
}

void SecondPass::writeSymbolToMemAndCreatRelEntry(string nameOfSymbol, int numOfLinee,
        int minusValueForPcRel,string tipS="ABS") {
    RelEntry::Type tip;
    if (tipS == "PC")  tip = RelEntry::R_PC16;
    else tip = RelEntry::R_16;

    Symbol* sym = ST::findSymbolByName(nameOfSymbol);
    if (sym == nullptr) SecondPass::error("Simbol nije definisan", numOfLinee);
    // Da li je definisan, da li je globalan, sta radit? Da li je ABS?
    if (sym->getSectionName().compare("absolute") == 0)
        currSection->setWordInMemoryAt(currOffset, sym->getValOff()- minusValueForPcRel);
    else if (sym->getIsLocal()) { // trebalo bi da je vec definisan
        // Ako je lokalan, onda upisujem taj offset na to mesto i
        // dodajem rel zapis u koji staljam sekciju te ce se adresa
        // te sekcije samo dodati posle
        int sectionToZakrpiti = ST::findSymbolByName(sym->getSectionName())->getOrdNum();

        currSection->setWordInMemoryAt(currOffset, sym->getValOff() - minusValueForPcRel);
        RelEntry* relEntry = new RelEntry(currSection->getName(),
                                          RT::getLastOrdNum() + 1, currOffset,
                                          tip, sectionToZakrpiti, sym->getSectionName());
        RT::addEntry(relEntry);
    } else {	//globalan
        // Na vezbama rece da upisujem nulu i onda pravim rel zapis vezan
        // za sam taj globalan simbol

        currSection->setWordInMemoryAt(currOffset, 0 - minusValueForPcRel);
        RelEntry* relEntry = new RelEntry(currSection->getName(),
                                          RT::getLastOrdNum() + 1, currOffset,
                                          tip, sym->getOrdNum(),sym->getName());
        RT::addEntry(relEntry);
    }
}


void SecondPass::startSecondPass() {
    cout << endl<<endl<<
         "\tPoceo je drugi prolaz" << endl;

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
                if (sym == nullptr) error("xkazemo da je globalan"
                                              " nije definisan, ne postoji u tabeli simbola",
                                              token.numOfLine);
                if (sym->getType() == 'S') error("Ne mozes sekciju proglasiti globalnom",
                                                     token.numOfLine);
                if (sym->getSectionName() == "undefined")
                    error("Simbol koji proglasavas globalnim nije definisan", token.numOfLine);
                //if (sym->getIsLocal() == false) cout << "Vec si negde ovaj simbol proglasio globalnim";
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
            // Potrebno je za prethodnu sekciju zabebeleziti gde smo stali
            if (currSectionName != "undefined")
                currSection->setOffsetToContinue(currOffset);

            string nameOfSection = token.getFrontValue();
            Section* newSec = SectionTable::findSectionByName(nameOfSection);
            if (newSec == nullptr) {
                newSec = new Section(nameOfSection);
                SecT::addSection(newSec);

                currOffset = 0;
            } else {
                currOffset = newSec->getOffsetToContinue();
            }
            currSectionName = nameOfSection;
            currSecNum = newSec->getOrdNum();
            currSection = newSec;
            break;
        }
        case PT::WORD: {
            while (!token.values.empty()) {
                if (token.getFrontValue()== "NUM") {
                    int value = stoi(token.getFrontValue(),nullptr,0);
                    currSection->setWordInMemoryAt_l_endian(currOffset,value);
                } else {
                    string value= token.getFrontValue();
                    Symbol* sym = ST::findSymbolByName(value);
                    if (sym == nullptr) SecondPass::error("Simbol nije definisan",token.numOfLine);
                    // Da li je definisan, da li je globalan, sta radit? Da li je ABS?
                    if (sym->getSectionName().compare("absolute")==0)
                        currSection->setWordInMemoryAt_l_endian(currOffset, sym->getValOff());
                    else if (sym->getIsLocal()) { // trebalo bi da je vec definisan
                        // Ako je lokalan, onda upisujem taj offset na to mesto i
                        // dodajem rel zapis u koji staljam sekciju te ce se adresa
                        // te sekcije samo dodati posle
                        int sectionToZakrpiti = ST::findSymbolByName(sym->getSectionName())->getOrdNum();

                        currSection->setWordInMemoryAt_l_endian(currOffset, sym->getValOff());
                        RelEntry* relEntry = new RelEntry(currSection->getName(),
                                                          RT::getLastOrdNum()+1,currOffset,
                                                          RelEntry::R_16, sectionToZakrpiti, sym->getSectionName());
                        RT::addEntry(relEntry);
                    } else {	//globalan
                        // Na vezbama rece da upisujem nulu i onda pravim rel zapis vezan
                        // za sam taj globalan simbol

                        currSection->setWordInMemoryAt(currOffset, 0);
                        RelEntry* relEntry = new RelEntry(currSection->getName(),
                                                          RT::getLastOrdNum() + 1, currOffset,
                                                          RelEntry::R_16, sym->getOrdNum(), sym->getName());
                        RT::addEntry(relEntry);
                    }
                }
                currOffset += 2;
            }

            currSection->addNextCR(currOffset - 1);
            break;
        }
        case PT::SKIP: {
            int size = stoi(token.getFrontValue(), nullptr, 0);
            for (int i = 0; i < size; i++) {
                currSection->setByteInMemoryAt(currOffset++, 0);
            }
            //currSection->printBytesASCII();
            currSection->addNextCR(currOffset - 1);
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
            if (instr.compare("halt") == 0) {
                currSection->setByteInMemoryAt(currOffset, 0);
            } else if (instr.compare("iret") == 0) {
                currSection->setByteInMemoryAt(currOffset, 0x20);
            } else if (instr.compare("ret") == 0) {
                currSection->setByteInMemoryAt(currOffset, 0x40);
            }
            currOffset++;
            //}
            currSection->addNextCR(currOffset - 1);
            break;
        }
        case PT::INSTR1: {
            string instr = token.getFrontValue();
            if (instr.compare("not") == 0) {
                string addrType = token.getFrontValue();
                uint8_t regNum1 = stoi(token.getFrontValue().substr(1));
                if (addrType.compare("POD_REG_DIR") != 0)
                    error("Uz ovu insrukciju ide samo reg dir", token.numOfLine);

                if (regNum1 > 8) error("Preveliki broj registra, ! <8", token.numOfLine);

                uint8_t byteOpMod = 0x80;	// Not
                uint8_t byteRdRs = 0x00;	// Dest: Reg1 , Source : reg2
                byteRdRs |= (regNum1 << 4);
                byteRdRs |= 0xF;	// nebitna vrednost

                currSection->setByteInMemoryAt(currOffset, byteOpMod);
                currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);

                currOffset += 2;
            } else if (instr.compare("int") == 0) {
                string addrType = token.getFrontValue();
                if (addrType.compare("POD_REG_DIR") != 0)
                    error("Uz insrukciju int ide samo reg dir", token.numOfLine);

                uint8_t regNum = stoi(token.getFrontValue().substr(1), nullptr, 0);
                if (regNum > 255) error("Preveliki broj registra, ! <256", token.numOfLine);

                uint8_t valToWrite = 0x0F;
                valToWrite |= (regNum << 4);
                currSection->setByteInMemoryAt(currOffset, 0x10);
                currSection->setByteInMemoryAt(currOffset+1, valToWrite);
                currOffset += 2;
            } else  if (instr.compare("push") == 0) {
                string addrType = token.getFrontValue();
                if (addrType.compare("POD_REG_DIR") != 0)
                    error("Uz insrukciju push ide samo reg dir", token.numOfLine);
                uint8_t regNum = stoi(token.getFrontValue().substr(1), nullptr, 0);
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
                uint8_t regNum = stoi(token.getFrontValue().substr(1), nullptr, 0);
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
            } else {
                uint8_t byteOpMod;

                if (instr == "call") {
                    byteOpMod = 0x30;
                } else if (instr == "jmp")   {
                    byteOpMod = 0x50;
                } else if (instr == "jeq")   {
                    byteOpMod = 0x51;
                } else if (instr =="jne")  {
                    byteOpMod = 0x52;
                } else if (instr =="jgt")  {
                    byteOpMod = 0x53;
                } else error("Ovde nesto bas ne valja, nisi smeo da dodjes do ove situacije"
                                 "\n Nalazis se u INST1 a nije ni jedna instr", token.numOfLine);

                string addrType = token.getFrontValue();
                if (addrType == "ADR_REG_DIR") {
                    uint8_t regNum = stoi(token.getFrontValue().substr(1), nullptr, 0);
                    if (regNum > 8) error("Preveliki broj registra, ! <8", token.numOfLine);

                    uint8_t byteRdRs = 0xF0;	// Dest: fix , Source : Reg
                    byteRdRs |= regNum;
                    uint8_t byteUpAddr = 0x01;	// Update: /, Reg dir

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);

                    currOffset += 3;
                } else if (addrType == "ADR_REG_IND_BEZ_POM") {
                    uint8_t regNum = stoi(token.getFrontValue().substr(1), nullptr, 0);
                    if (regNum > 8) error("Preveliki broj registra, ! <8", token.numOfLine);

                    uint8_t byteRdRs = 0xF0;	// Dest: fix , Source : Reg
                    byteRdRs |= regNum;
                    uint8_t byteUpAddr = 0x02;	// Update: /, Reg ind

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);

                    currOffset += 3;

                } else if (addrType == "ADR_REG_DIR_PLUS_LIT") {
                    uint8_t regNum = stoi(token.getFrontValue().substr(1), nullptr, 0);
                    if (regNum > 8) error("Preveliki broj registra, ! <8", token.numOfLine);

                    uint8_t byteRdRs = 0xF0;	// Dest: fix , Source : Reg
                    byteRdRs |= regNum;
                    uint8_t byteUpAddr = 0x03;	// Update: /, Reg ind s pomerajem

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);
                    currSection->setWordInMemoryAt(currOffset + 3, stoi(token.getFrontValue(), nullptr, 0));
                    //											x2, Word, vrednost literala

                    currOffset += 5;
                } else if (addrType == "ADR_REG_DIR_PLUS_SIM") {
                    uint8_t regNum = stoi(token.getFrontValue().substr(1), nullptr, 0);
                    if (regNum > 8) error("Preveliki broj registra, ! <8", token.numOfLine);

                    uint8_t byteRdRs = 0xF0;	// Dest: fix , Source : Reg
                    byteRdRs |= regNum;
                    uint8_t byteUpAddr = 0x03;	// Update: /, Reg ind s pomerajem

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);

                    string symbolName = token.getFrontValue();

                    currOffset += 3;
                    SecondPass::writeSymbolToMemAndCreatRelEntry(symbolName, token.numOfLine, 0);
                    //											  Word, dodati pomeraj sekcije

                    currOffset += 2; // ukupno 5
                } else if (addrType == "ADR_MEM_SIM") {
                    string symName = token.getFrontValue();

                    uint8_t byteRdRs = 0xF0;	// Dest: fix , Source : /
                    uint8_t byteUpAddr = 0x04;	// Update: /, Mem

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);

                    currOffset += 3;
                    SecondPass::writeSymbolToMemAndCreatRelEntry(symName, token.numOfLine, 0);
                    //											  Word, dodati pomeraj sekcije

                    currOffset += 2;
                } else if (addrType == "ADR_MEM_LIT") {
                    uint16_t addr = stoi(token.getFrontValue(), nullptr, 0);

                    uint8_t byteRdRs = 0xF0;	// Dest: fix , Source : /
                    uint8_t byteUpAddr = 0x04;	// Update: /, Mem

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);
                    currSection->setWordInMemoryAt(currOffset + 3, addr); //word


                    currOffset += 5;
                } else if (addrType == "PODiADR_PC_REL_SIM") {
                    // Ovde se radi o relativnom PC adresiranju, ako odradimo
                    // registarski indirektno, to nece bas imati smisla
                    string symName = token.getFrontValue();

                    uint8_t byteRdRs = 0xF7;	// Dest: fix , Source : PC
                    uint8_t byteUpAddr = 0x05;	// Update: /, PC rel dir
                    // uint8_t byteUpAddr = 0x00;	// Update: /, NEPOSREDNO


                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);

                    currOffset += 3;
                    // oduzece 2, jer imamo instrukciju od 5B a on treba da smesti na lokaciju 4-5
                    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    SecondPass::writeSymbolToMemAndCreatRelEntry(symName, token.numOfLine, 2, "PC" );
                    // Da li ovde staviti neposredno pa dva relok zapisa? Jedan za PC a drugi za simbol,
                    // Jos nisam razjasnio, nije mi bas najjasnije kako radi linker. Posle vezbi odradi

                    currOffset += 2;
                } else if (addrType == "LIT") {
                    uint16_t addr = stoi(token.getFrontValue(), nullptr, 0);

                    uint8_t byteRdRs = 0xF0;	// Dest: fix , Source : /
                    uint8_t byteUpAddr = 0x00;	// Update: /, Neposredno

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);
                    currSection->setWordInMemoryAt(currOffset + 3, addr);

                    currOffset += 5;
                } else if (addrType == "SIM") {
                    string symName = token.getFrontValue();

                    uint8_t byteRdRs = 0xF0;	// Dest: fix , Source : /
                    uint8_t byteUpAddr = 0x00;	// Update: /, Neposredno

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);

                    currOffset += 3;
                    SecondPass::writeSymbolToMemAndCreatRelEntry(symName, token.numOfLine, 0);
                    //											  Word, dodati pomeraj sekcije

                    currOffset += 2;
                } else
                    error("Ovaj vid adresiranja za skok nije dozvoljen", token.numOfLine);

            }
            currSection->addNextCR(currOffset - 1);
            break;
        }
        case PT::INSTR2: {
            string instr = token.getFrontValue();
            if (instr == "ldr"|| instr =="str") {
                uint8_t regNum1 = stoi(token.getFrontValue().substr(1), nullptr, 0);
                if (regNum1 > 8) error("Preveliki broj registra, ! <8", token.numOfLine);

                uint8_t byteOpMod = (instr == "ldr") ? 0xA0 : 0xB0;
                uint8_t byteRdRs = 0x00;		// Dest: reg1, src:?
                byteRdRs |= (regNum1 << 4);

                string addrType = token.getFrontValue();
                if (addrType == "POD_NEPOS_LIT") {
                    uint16_t addr = stoi(token.getFrontValue(), nullptr, 0);

                    uint8_t byteUpAddr = 0x00;	// Update: /, Neposredno

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);
                    currSection->setWordInMemoryAt(currOffset + 3, addr);

                    currOffset += 5;
                } else if (addrType == "POD_NEPOS_SIM") {
                    string symName = token.getFrontValue();

                    uint8_t byteUpAddr = 0x00;	// Update: /, Neposredno

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);

                    currOffset += 3;
                    SecondPass::writeSymbolToMemAndCreatRelEntry(symName, token.numOfLine, 0);
                    //											  Word, dodati pomeraj sekcije

                    currOffset += 2;
                } else if (addrType == "SIM") {
                    string symName = token.getFrontValue();

                    uint8_t byteUpAddr = 0x04;	// Update: /, Mem

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);

                    currOffset += 3;
                    SecondPass::writeSymbolToMemAndCreatRelEntry(symName, token.numOfLine, 0);
                    //											  Word, dodati pomeraj sekcije

                    currOffset += 2;
                } else if (addrType == "LIT") {
                    uint16_t addr = stoi(token.getFrontValue(), nullptr, 0);

                    uint8_t byteUpAddr = 0x04;	// Update: /, Mem

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);
                    currSection->setWordInMemoryAt(currOffset + 3, addr); //word


                    currOffset += 5;
                } else if (addrType == "PODiADR_PC_REL_SIM") {
                    string symName = token.getFrontValue();

                    byteRdRs |= 7;				// Dest: reg , Source : PC
                    // Mislim da je ovaj red gore totalno totalno nebitan
                    // Ipak je bitno jer 0x03 nije PC rel vec reg ind a ti namesti
                    // da taj reg bude PC
                    uint8_t byteUpAddr = 0x03;	// Update: /, PC rel ind

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);

                    currOffset += 3;
                    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    SecondPass::writeSymbolToMemAndCreatRelEntry(symName, token.numOfLine, 2, "PC" );
                    //				 Mislim da ovde ne treba PC jer si PC vec ukljucio u racun preko reg7 ind?

                    currOffset += 2;
                } else if (addrType == "POD_REG_DIR") {
                    uint8_t regNum2 = stoi(token.getFrontValue().substr(1), nullptr, 0);
                    if (regNum2 > 8) error("Preveliki broj registra, ! <8", token.numOfLine);

                    byteRdRs |= regNum2;
                    uint8_t byteUpAddr = 0x01;	// Update: /, Reg dir

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);

                    currOffset += 3;
                } else if (addrType == "POD_REG_IND_BEZ_POM") {
                    uint8_t regNum2 = stoi(token.getFrontValue().substr(1), nullptr, 0);
                    if (regNum2 > 8) error("Preveliki broj registra, ! <8", token.numOfLine);

                    byteRdRs |= regNum2;
                    uint8_t byteUpAddr = 0x02;	// Update: /, Reg ind

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);

                    currOffset += 3;
                } else if (addrType == "POD_REG_DIR_PLUS_LIT") {
                    uint8_t regNum2 = stoi(token.getFrontValue().substr(1), nullptr, 0);
                    if (regNum2 > 8) error("Preveliki broj registra, ! <8", token.numOfLine);

                    byteRdRs |= regNum2;
                    uint8_t byteUpAddr = 0x03;	// Update: /, Reg ind s pomerajem

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);
                    currSection->setWordInMemoryAt(currOffset + 3, stoi(token.getFrontValue(), nullptr, 0));
                    //											x2, Word, vrednost literala

                    currOffset += 5;
                } else if (addrType == "POD_REG_DIR_PLUS_SIM") {
                    uint8_t regNum2 = stoi(token.getFrontValue().substr(1), nullptr, 0);
                    if (regNum2 > 8) error("Preveliki broj registra, ! <8", token.numOfLine);

                    byteRdRs |= regNum2;
                    uint8_t byteUpAddr = 0x03;	// Update: /, Reg ind s pomerajem

                    currSection->setByteInMemoryAt(currOffset, byteOpMod);
                    currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);
                    currSection->setByteInMemoryAt(currOffset + 2, byteUpAddr);

                    currOffset += 3;
                    string symbolName = token.getFrontValue();
                    SecondPass::writeSymbolToMemAndCreatRelEntry(symbolName, token.numOfLine, 0);
                    //											  Word, dodati pomeraj sekcije

                    currOffset += 2;
                } else
                    error("Ovaj vid adresiranja za skok nije dozvoljen", token.numOfLine);


            } else {
                uint8_t regNum1 = stoi(token.getFrontValue().substr(1));
                string addrType = token.getFrontValue();
                if (addrType.compare("POD_REG_DIR") != 0)
                    error("Uz ovu insrukciju ide samo reg dir", token.numOfLine);
                uint8_t regNum2 = stoi(token.getFrontValue().substr(1), nullptr, 0);

                if (regNum1 > 8) error("Preveliki broj registra, ! <8", token.numOfLine);
                if (regNum2 > 8) error("Preveliki broj registra, ! <8", token.numOfLine);

                uint8_t byteOpMod = 0x00;	// Mesto za kod instr
                uint8_t byteRdRs = 0x00;	// Dest: Reg1 , Source : reg2
                byteRdRs |= (regNum1 << 4);
                byteRdRs |= (regNum2);

                if (instr == "xchg") {
                    byteOpMod = 0x60;
                } else if (instr == "add") {
                    byteOpMod = 0x70;
                } else if (instr == "sub") {
                    byteOpMod = 0x71;
                } else if (instr == "mul") {
                    byteOpMod = 0x72;
                } else if (instr == "div") {
                    byteOpMod = 0x73;
                } else if (instr == "cmp") {
                    byteOpMod = 0x74;
                } else if (instr == "not") {
                    byteOpMod = 0x80;
                } else if (instr == "and") {
                    byteOpMod = 0x81;
                } else if (instr == "or") {
                    byteOpMod = 0x82;
                } else if (instr == "xor") {
                    byteOpMod = 0x83;
                } else if (instr == "test") {
                    byteOpMod = 0x84;
                } else if (instr == "shl") {
                    byteOpMod = 0x90;
                } else if (instr == "shr") {
                    byteOpMod = 0x91;
                }

                currSection->setByteInMemoryAt(currOffset, byteOpMod);
                currSection->setByteInMemoryAt(currOffset + 1, byteRdRs);

                currOffset += 2;
            }

            currSection->addNextCR(currOffset - 1);
            break;
        }
        default:
            error("nepoznata direktiva", token.numOfLine);
        }
    }



}
