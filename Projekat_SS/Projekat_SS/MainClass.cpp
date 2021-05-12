#include "MainClass.h"
#include <iostream>
#include <string>
#include <fstream>
#include <regex>
using namespace std;


// Staticke moras da navedes i u cpp fajlu
ifstream MainClass::inputFile ;
ofstream MainClass::outputFile ;

// Otvara fajlove inputFiles outputFiles, baca gresku
void MainClass::openFiles(string input, string output) {
    inputFile.open(input);
    outputFile.open(output);

    if (!inputFile.is_open())
        error("Neuspesno otvaranje ulaznog fajla");
    if (!outputFile.is_open())
        error("Neuspesno kreiranje izlaznog fajla");

    outputFile << "Evo teksta";
}

//	Proerava broj argumenata, da li je .s fajl i prosledjuje
void MainClass::checkArg(int argc, char * argv[], string& inputStr, string& outputStr) {
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
void MainClass::error(std::string msg) {
    cerr << msg << endl;
    exit(-1);
}

void MainClass::passArgAndOpenFiles(int argc, char *argv[]) {
    string input, output;

    checkArg(argc, argv, input, output);

    openFiles(input, output);

}

void MainClass::assemblyDone() {
    outputFile.close();
}

void MainClass::firstPassDone_CloseInputFile() {
    inputFile.close();
}

string MainClass::getInputLine() {
    string ret;
    getline(inputFile, ret);
    return ret;
}

bool MainClass::eofInput() {

    return inputFile.eof();
}


