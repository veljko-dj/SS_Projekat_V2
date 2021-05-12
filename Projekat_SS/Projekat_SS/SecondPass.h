#pragma once
#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <string>
using namespace std;

class SecondPass {

    static string currSection;
    static int currOffset;
    static int numOfToken; // nepotrebno, cisto onako da znas

    static void error(string msg, string line);

public:
    static void startSecondPass();
};


#endif // !SECOND_PASS_H
