/**
 * @file constants.h
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author N.I. van Laarhoven
 * @brief File with general constants needed by the program
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

//The maximum amount of Beta-reductions allowed on a lambda-expression 
//Prevents infinite reductions from halting the program
const int MAX_REDUCE = 1000;

#endif //CONSTANTS_H