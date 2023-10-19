#include "errors.h"

void throwException(EErrorType type){
    std::cerr << "ERROR: " << errorMessages[type-1] << std::endl;
    exit(type);
} //throwException