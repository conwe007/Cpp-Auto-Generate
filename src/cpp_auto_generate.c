#include "cpp_auto_generate.h"

// initializes a header data structure based on the number of methods to be defined, returns NULL on failure
header_data_t* initializeHeaderData(int num_methods)
{
    header_data_t* header_data = (header_data_t*)malloc(sizeof(header_data_t));

    strcpy(header_data->file_name_h, "\0");
    strcpy(header_data->file_name_cpp, "\0");
    strcpy(header_data->class_name, "\0");

    header_data->methods = (char***)malloc(sizeof(char**) * 3);
    header_data->methods[0] = (char**)malloc(sizeof(char*) * num_methods); // return types
    header_data->methods[1] = (char**)malloc(sizeof(char*) * num_methods); // methods (names and arguments)
    header_data->methods[2] = (char**)malloc(sizeof(char*) * num_methods); // comments
    
    for(int i = 0; i <  num_methods; i++)
    {
        header_data->methods[0][i] = (char*)malloc(sizeof(char) * SIZE_STRING);
        header_data->methods[1][i] = (char*)malloc(sizeof(char) * SIZE_STRING);
        header_data->methods[2][i] = (char*)malloc(sizeof(char) * SIZE_STRING);

        strcpy(header_data->methods[0][i], "\0");
        strcpy(header_data->methods[1][i], "\0");
        strcpy(header_data->methods[2][i], "\0");
    }

    return header_data;
}

// populates header data strucure with info extracted from header file, returns NULL on failure
header_data_t* populateHeaderData(char* const header_file_name)
{
    int num_methods = numMethods(header_file_name);
    header_data_t* header_data = initializeHeaderData(num_methods);

    char buffer[SIZE_STRING] = "\0";

    // remove file path and add header file name to header data
    strcpy(header_data->file_name_h, header_file_name);

    // remove file path and .h from header file and append .cpp
    sscanf(header_file_name, "%[^.]s", buffer);
    snprintf(header_data->file_name_cpp, SIZE_STRING, "%s.cpp", buffer);

    FILE* fp = fopen(header_file_name, "r");
    if(fp == NULL)
    {
        fprintf(stderr, "error: failed to open file '%s'\n", header_file_name);
        return NULL;
    }

    char next_char = '\0';

    // loop through file until the class is reached
    while(strcmp(buffer, "class"))
    {
        fscanf(fp, "%s", buffer);

        // if a comment is encountered, dump the rest of the line
        if(!strcmp(buffer, "//"))
        {
            while(next_char != '\n')
            {
                fscanf(fp, "%c", &next_char);
            }

            next_char = '\0';
        }
    }

    // add class name to header data
    fscanf(fp, "%s", buffer);
    strcpy(header_data->class_name, buffer);

    // loop through file until you get to the public section of the class
    while(strcmp(buffer, "public:"))
    {
        fscanf(fp, "%s", buffer);
    }

    int method_index = 0;

    char constructor_check[SIZE_STRING] = "\0";
    char destructor_check[SIZE_STRING] = "\0";

    char constructor[SIZE_STRING] = "\0";
    strcpy(constructor, header_data->class_name);

    char destructor[SIZE_STRING] = "\0";
    destructor[0] = '~';
    strcpy(destructor + 1, header_data->class_name);

    // loop through and record each method and its associated comment in public section of class
    while(fgets(buffer, sizeof(buffer), fp))
    {
        trimWhitespace((char*)&buffer);

        // exit loop if the end of the class is encountered
        if(!strcmp(buffer, "};"))
        {
            break;
        }

        // add the return type, method, and its associated comment to header data if the line is not empty
        if(buffer[0] != '\0')
        {
            // truncate the beginning of the first token to check against constructor or destructor
            snprintf(constructor_check, strlen(header_data->class_name) + 1, "%s", buffer);
            snprintf(destructor_check, strlen(header_data->class_name) + 2, "%s", buffer);

            // method is a constructor/destructor, so no return type
            if(!strcmp(constructor_check, constructor) || !strcmp(destructor_check, destructor))
            {
                // return type is empty
                strcpy(header_data->methods[0][method_index], "\0");

                // method name and arguments
                sscanf(buffer, "%[^;]s", header_data->methods[1][method_index]);

                // associated comment
                sscanf(buffer + strlen(header_data->methods[1][method_index]) + sizeof(";"), "%[^!]s", header_data->methods[2][method_index]);
            }
            
            // return type present, parse as usual
            else
            {
                // return type
                sscanf(buffer, "%[^ ]s", header_data->methods[0][method_index]);

                // method name and arguments
                sscanf(buffer + strlen(header_data->methods[0][method_index]), "%c%[^;]s", &next_char, header_data->methods[1][method_index]);

                // associated comment
                sscanf(buffer + strlen(header_data->methods[0][method_index]) + strlen(header_data->methods[1][method_index]) + sizeof(";"), "%c%[^!]s", &next_char, header_data->methods[2][method_index]);
            }

            method_index++;
        }
    }

    fclose(fp);

    return header_data;
}

// frees space allocated for header data structure
void freeHeaderData(header_data_t* header_data)
{
    free(header_data->methods);
    free(header_data);
}

// creates a cpp file based off header file, returns 1 on success, 0 on failure/error
int createCppFile(char* const header_file_name)
{
    int num_methods = numMethods(header_file_name);

    header_data_t* header_data = populateHeaderData(header_file_name);
    if(header_data == NULL)
    {
        fprintf(stderr, "error: failed to populate header data\n");
        return 0;
    }

    FILE* fp = fopen(header_data->file_name_cpp, "w");
    if(fp == NULL)
    {
        fprintf(stderr, "error: failed to open file '%s'\n", header_file_name);
        return 0;
    }

    // add the include directive pointing to the header file
    fprintf(fp, "#include \"%s\"\n\n", header_data->file_name_h);

    // loop through each method in header data and add it to the output file
    for(int method_index = 0; method_index < num_methods; method_index++)
    {
        // comment
        fprintf(fp, "%s\n", header_data->methods[2][method_index]);

        // return type
        fprintf(fp, "%s%s", header_data->methods[0][method_index], (!strcmp(header_data->methods[0][method_index], "\0") ? "" : " "));

        // class name
        fprintf(fp, "%s::", header_data->class_name);

        // method
        fprintf(fp, "%s\n", header_data->methods[1][method_index]);

        // curly brackets
        fprintf(fp, "{\n\t\n}\n\n");
    }

    fclose(fp);
    freeHeaderData(header_data);

    return 1;
}

// returns the number of methods present in class defined in header file or -1 on error
int numMethods(char* const header_file_name)
{
    FILE* fp = fopen(header_file_name, "r");
    if(fp == NULL)
    {
        fprintf(stderr, "error: failed to open file '%s'\n", header_file_name);
        return -1;
    }

    int num_methods = 0;
    char buffer[SIZE_STRING] = "\0";

    // loop through file until you get to the public section of the class
    while(strcmp(buffer, "public:"))
    {
        fscanf(fp, "%s", buffer);
    }

    // loop through and cound methods in public section of class
    while(fgets(buffer, sizeof(buffer), fp))
    {
        trimWhitespace((char*)&buffer);

        // exit loop if the end of the class is encountered
        if(!strcmp(buffer, "};"))
        {
            break;
        }

        // increment number of methods if the line is not empty
        if(buffer[0] != '\0')
        {
            num_methods++;
        }
    }

    fclose(fp);

    return num_methods;
}

// trims leading and trailing whitespace from str
char *trimWhitespace(char *str)
{
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if( str == NULL ) { return NULL; }
    if( str[0] == '\0' ) { return str; }

    len = strlen(str);
    endp = str + len;

    // Move the front and back pointers to address the first non-whitespace characters from each end.
    while( isspace((unsigned char) *frontp) ) { ++frontp; }
    if( endp != frontp )
    {
        while( isspace((unsigned char) *(--endp)) && endp != frontp ) {}
    }

    if( frontp != str && endp == frontp )
            *str = '\0';
    else if( str + len - 1 != endp )
            *(endp + 1) = '\0';

    // Shift the string so that it starts at str so that if it's dynamically allocated, we can still free it on the returned pointer. Note the reuse of endp to mean the front of the string buffer now.
    endp = str;
    if( frontp != str )
    {
            while( *frontp ) { *endp++ = *frontp++; }
            *endp = '\0';
    }

    return str;
}