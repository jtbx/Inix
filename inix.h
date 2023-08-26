#ifndef INIX_H
#define INIX_H

/**************************************************************************************************
*
* The Inix library is a free and open source INI parser C library delivered under the GPL3 license.
*
* The Inix library is a C library that will allow you to parse INI files.
* It is coded in pure C99 with no compiler extensions, only using C standard libraries.
* For more info about how the parser works, please checkout the doucmentation.
*
***************************************************************************************************/



/*
* Inix macros
*/

#define INIX_VERSION 1.0;           /* The verion of the library */
#define INIX_AUTHOR "PINGY BOY";    /* The author of the library */
#define INIX_LICENSE "GPL3"         /* The license of the library */

#define BUFFER_SIZE 200             /* Defines the maximum buffer size when parsing lines */

/*
* INI structure to store and write INI files
*/

typedef struct IniElement {     /* The elements storing a key and a value */
    char  key[32];
    char  value[32];
} IniElement;

typedef struct IniSection {     /* The sections storing an array of elements and a name */
    char        name[32];
    IniElement  *elems;
} IniSection;

typedef struct IniStruct {      /* The main INIstruct storing an array of sections and a name */
    char            name[32];
    IniSection      *sects;
} IniStruct;

/*
* Functions Prototypes to manage an INIstruct
*/

/* Core functions, Create, Reset and Free INIstructs */
IniStruct *IniMake(char *);     /* Creates an empty INIstruct and return it's address */
void IniEnd(IniStruct *);       /* Takes a pointer to an INIstruct and free it */
void IniClear(IniStruct *);     /* Takes a pointer to an INIstruct and reset it */

/* Input/Output functions, read from/write to an INIstruct */
int IniAddSect(IniStruct *, char *);                        /* Adds a section to a given INIstruct */
int IniAddElem(IniStruct *, char *, char *, char *);        /* Adds an element to a given section */
int IniDelSect(IniStruct *, char *);                        /* Deletes a section from a given INIstruct */
int IniDelElem(IniStruct *, char *, char *);                /* Deletes an element form a given section */
int IniModSect(IniStruct *, char *, char *);                /* Modifies the name of a given section */
int IniModElemValue(IniStruct *, char *, char *, char *);   /* Modifies the key of a given element in a given section */
int IniModElemKey(IniStruct *, char *, char *, char *);     /* Modifies the value of a given elemnt in a given section */
char *IniReadElem(IniStruct *, char *, char*);              /* Returns the value from a given elements in a given section */

/* Listing functions, Lists everything in the whole struct or in a specific section */
void IniListAll(IniStruct *);           /* List all elements and sections in a given INIstruct */
int IniListSect(IniStruct *, char *);   /* List all elements of a given struct */

/* Seek utils functions, return true/false */
int IniSeekSect(IniStruct *, char *);              /* Checks if a given section exists in a given INIstruct */
int IniSeekElem(IniStruct *, char *, char *);      /* Checks if a given key exists in a given section in a given INIstruct */

/* Global section functions, utilities to handle the global part of an INIstruct */
int IniAddGlob(IniStruct *, char *, char *);        /* Adds a global element */
int IniDelGlob(IniStruct *, char *);                /* Deletes a global element */
int IniModGlobKey(IniStruct *, char *, char *);     /* Modifies a global element's key */
int IniModGlobValue(IniStruct *, char *, char *);   /* Modifies a global element's value */
char *IniReadGlob(IniStruct *, char*);              /* Reads from a global element's and returns it's value */
int IniListGlob(IniStruct *);                       /* Lists all elements of the global section */
int IniSeekGlob(IniStruct *, char *);               /* Seeks an elements in a given section and returns true if it finds it */

/* File functions, read from/write to an INI file */
IniStruct *IniReadFile(char *, char *);     /* Reads from a file and puts the data in an INIstruct and returns the adress of it */
void IniWriteFile(IniStruct *, char *);     /* Writes the content of an INIstruct into an INIfile */

#endif