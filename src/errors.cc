#include "errors.h"

void printError(EErrorType type){
    std::cerr << "ERROR: " << errorMessages[type] << std::endl;
    exit(type);
} //printError