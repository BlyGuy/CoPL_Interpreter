/**
 * @file constants.h
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author Noah I. van Laarhoven
 * @brief File with general constants needed by the program
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

//The maximum amount of Beta-reductions allowed on a lambda-expression 
//Prevents infinite reductions from halting the program
const int MAX_REDUCTIONS = 100;

//exit code for when the amount of reductions reached its limit
const int EXIT_LIMIT_REACHED = 2;

#endif //CONSTANTS_H