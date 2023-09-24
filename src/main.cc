#include <iostream>
#include "errors.h"
#include "tokens.h"

int main() {
    Token myToken = {VAR, "DeEersteVarBP"};

    std::cout << myToken.varName << std::endl;
    
    EErrorType myError = ALLOCATION_ERROR;
    printError(myError);

    return 0;
}