#include "MainClass_ASM.h"

#include <iostream>
#include <string>
#include <fstream>
#include <regex>

using namespace std;


// Staticke moras da navedes i u cpp fajlu
ifstream MainClass_ASM::inputFile;
ofstream MainClass_ASM::outputFile;
ofstream MainClass_ASM::outputFileTxt;

// Otvara fajlove inputFiles outputFiles
void MainClass_ASM::openFiles(string input, string output) {
    inputFile.open(input);
    outputFile.open(output, ios::out | ios::binary);
    outputFileTxt.open(output.substr(0,output.length()-2).append(".txt"));

    if (!inputFile.is_open())
        error("Neuspesno otvaranje ulaznog fajla");
    if (!outputFile.is_open())
        error("Neuspesno kreiranje izlaznog fajla");
    if (!outputFileTxt.is_open())
        error("Neuspesno kreiranje izlaznog fajla");

}

// Provera broja argumenata, da li je .s fajl
void MainClass_ASM::checkArg(int argc, char * argv[], string& inputStr, string& outputStr) {
    string opt = "-o";
    int posOfExt;


    if (argc == 2) {	// ulazni.s
        inputStr = argv[1];
        outputStr = argv[1];
        // kreiranje imena [ulazni].o
        posOfExt= outputStr.find_last_of(".s");
        if (posOfExt == string::npos) error("Ulazni fajl nije .s");
        outputStr[posOfExt] = 'o';

    } else if (argc == 4 && opt.compare(argv[1])==0) {	// -o izlazni.o ulazni.s
        inputStr = argv[3];
        outputStr = argv[2];
        posOfExt = inputStr.find_last_of(".s");
        if (posOfExt == string::npos) error("Ulazni fajl nije .s");

    } else
        error("Greska : Broj argumenata");


}

//	Ispisuje gresku na izlazu za greske i exit(-1);
void MainClass_ASM::error(std::string msg) {
    cerr << msg << endl;
    exit(-1);
}

void MainClass_ASM::passArgAndOpenFiles(int argc, char *argv[]) {
    string input, output;

    checkArg(argc, argv, input, output);

    openFiles(input, output);

}

void MainClass_ASM::closeInputFile() {
    inputFile.close();
}

void MainClass_ASM::closeOutputFile() {
    outputFile.close();
    outputFileTxt.close();
    if (!outputFile.good())
        error("Error occurred at writing time!");
    if (!outputFileTxt.good())
        error("Error occurred at writing time!");
}

void MainClass_ASM::assemblyDoneSayBye() {
    cout << endl << "\tUspesno zavrsio asembliranje" << endl;
}

string MainClass_ASM::getInputLine() {
    string ret;
    getline(inputFile, ret);
    return ret;
}

bool MainClass_ASM::eofInput() {
    return inputFile.eof();
}
