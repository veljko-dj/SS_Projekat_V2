#include "SecondPass.h"
#include "parsedTokens.h"

#include <string>
#include <iostream>

using namespace std;

void SecondPass::error(string msg, string line) {
    cout << "\t ERROR: SECCOND PASS:"<<endl
         << msg << " :" << line << endl;
    exit(0);
}

void SecondPass::startSecondPass() {
    cout << endl<<endl<<
         "Poceo je drugi prolaz" << endl;
}
