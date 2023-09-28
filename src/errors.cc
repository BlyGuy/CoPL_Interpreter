#include "errors.h"

void printError(EErrorType type){
    std::cerr << "ERROR: " << errorMessages[type-1] << std::endl;
    exit(type);
} //printError