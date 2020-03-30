# John Conwell

## C++ Class File Generator

### Description
This program takes a C++ header file as input and creates a matching C++ class file with empty function definitions. The acceptable format of the header file is described in the format section.

### To Compile
Open a terminal in the base directory and use the command
```bash
$ make
```
to compile this program. This program compiles on Unix with gcc 7.4.0.

### To Execute
Run the program from the terminal with the following command:
```bash
$ ./executale_name [header file name]
```
Note that the header file must be in the same directory as the executable.

### Format
The header file must follow strict formatting guidelines for this program to execute correctly. An example header file that follows the formatting guidelines can be found in the example folder. The following is a list of formatting rules:

1) The header file must contain exactly one class

2) Comments about methods must appear on the same line as the method

3) The end of the class declaration must be a single line with nothing but the characters "};"

The following is a non-comprehensive list of formatting guidelines:

1) Only public methods will be populated in the class file. The program will still execute if there are private methods, but they will not be populated in the class file

2) White space is allowed anywhere in the header file, but will be reflected in the formatting of the class file