#ifndef __EXAMPLE_H
#define __EXAMPLE_H

#include <cstdlib>
#include <cstdio>
#include <cstring>

// this is an example structure, it will be ignored when generating .cpp file
typedef struct
{
    int data1; // this variable holds data1
    char* data2; // this variable holds data2
    bool data3; // this variable holds data3
} example_t;

// an example class containing data and methods using the data
class Example
{
    private:
        int data1;
        char* data2;
        bool data3;

    public:
        Example(); // default constructor
        Example(example_t* example); // this constructor takes an example_t struct as an argument
        ~Example(); // destructor

        int getData1(); // returns data1
        char* setData2(char* data); // sets data2 to data
        bool isData3(); // returns data3

        void analyzeData(int number); // analyzes the data using number
        example_t* extractData(); // extracts class data into an example struct
        bool printData(); // prints the data and returns true on success or false on failure
};

#endif