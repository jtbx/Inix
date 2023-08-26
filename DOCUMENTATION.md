# Introduction
* Inix is is a small INI file handling library that will allow you to read, store and write INI file informations.
* The way INI stores the INI data is by using a custom structure called an INIstruct.
* Inix comes with a lot of usefull functions to handle INIstruct, these functions allow you to Add, Delete, Modify and read sections and keys of a struct.
* Both the INIstruct structure and the functions declaration, as well as a few usefull macros are defined in the header file.
* Finally Inix will allow you to parse an INI file and store the data in an INIstruct and write to an INI file using the INIstruct's data.
# List of functions
|Core functions|
* IniStruct *IniMake(char *name_of_the_struct)     // IniMake takes the structure name as an argument then creates a mallocated INIstruct with the given name and returns it's address.
* void IniClear(IniStruct *ini_struct)             // IniClear takes a pointer to a structure and free it then give it the adress of a new struct.
* void IniEnd(IniStruct *ini_struct)               // IniEnd takes a pointer to a structure and free it.
|Input/Output functions|
* IniSectAdd
