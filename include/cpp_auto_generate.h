#ifndef __CPP_AUTO_GENERATE_H
#define __CPP_AUTO_GENERATE_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_STRING 1024

// this strucure contains data about a cpp header file with exactly one class
typedef struct
{
    char file_name_h[SIZE_STRING]; // name of the header file
    char file_name_cpp[SIZE_STRING]; // name of the header file with .cpp instead of .h
    char class_name[SIZE_STRING]; // name of class
    char*** methods; // list of return types(methods[0], if applicable), methods (methods[1]), and corresponding comments (methods[2], if any)
} header_data_t;

header_data_t* initializeHeaderData(int num_methods); // initializes a header data structure based on the number of methods to be defined, returns NULL on failure
header_data_t* populateHeaderData(char* const header_file_name); // populates header data strucure with info extracted from header file, returns NULL on failure
void freeHeaderData(header_data_t* header_data); // frees space allocated for header data structure

int createCppFile(char* const header_file_name); // creates a cpp file based off header data structure

int numMethods(char* const header_file_name); // returns the number of methods present in class defined in header file
char* trimWhitespace(char* str); // trims leading and trailing whitespace from str

#endif