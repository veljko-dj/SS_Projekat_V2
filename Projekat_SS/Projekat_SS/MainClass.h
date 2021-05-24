#pragma once
#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <string>

using namespace std;

class MainClass {
private:
//param
public:
    static std::ifstream inputFile ;
    static std::ofstream outputFile ;

//methods
private:
    static void openFiles(std::string input, std::string output);
    static void checkArg(int, char *argv[], std::string& inputStr, std::string& outputStr);
    static void error(std::string );
public:
    static void passArgAndOpenFiles(int argc, char *argv[]);
    static void assemblyDone();
    static void closeInputFile();
    static void closeOutputFile();

// pomocne:
    static string getInputLine();
    static bool eofInput();

};




#endif // !MAINCLASS_H
