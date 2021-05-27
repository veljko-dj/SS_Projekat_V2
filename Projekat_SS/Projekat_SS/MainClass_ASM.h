#pragma once
#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <string>

using namespace std;

class MainClass_ASM {
private:
//param
public:
    static std::ifstream inputFile;
    static std::ofstream outputFile;
    static std::ofstream outputFileTxt;

//methods
private:
    static void openFiles(std::string input, std::string output);
    static void checkArg(int, char *argv[], std::string& inputStr, std::string& outputStr);
    static void error(std::string );
public:
    static void passArgAndOpenFiles(int argc, char *argv[]);
    static void closeInputFile();
    static void closeOutputFile();
    static void assemblyDoneSayBye();

// pomocne:
    static string getInputLine();
    static bool eofInput();

};




#endif // !MAINCLASS_H
