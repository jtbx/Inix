#ifndef INIX_H
#define INIX_H

/******************************************************************
*
* Inix: a free and open source INI parser C library licensed under
* the GNU General Public License, version 3.
*
* Inix is a C library that will allow you to parse INI files.
* It is coded in pure C99 with no compiler extensions,
* only using C standard libraries. For more info about how
* the parser works, please checkout the documentation.
*
******************************************************************/



/*
* Inix macros
*/

#define INIX_VERSION 1.0
#define INIX_AUTHOR "PINGY BOY"
#define INIX_LICENSE "GPL3"

#define BUFFER_SIZE 200             /* Defines the maximum buffer size when parsing lines */

/*
* Structures to store INI files
*/

typedef struct IniElement {     /* The elements storing a key and a value */
    char  key[32];
    char  value[32];
} IniElement;

typedef struct IniSection {     /* The sections storing an array of elements and a name */
    char        name[32];
    IniElement  *elems;
} IniSection;

typedef struct IniStruct {      /* The main IniStruct storing an array of sections and a name */
    char            name[32];
    IniSection      *sects;
} IniStruct;

/*
* Functions prototypes for manipulating an IniStruct
*/

/* Core functions (create, reset and free IniStructs) */
IniStruct *IniMake(char *);     /* Returns a pointer to a new IniStruct */
void IniEnd(IniStruct *);       /* Frees the given IniStruct */
void IniClear(IniStruct *);     /* Resets the given IniStruct */

/* Input/Output functions */
int   IniAddSect(IniStruct *, char *);                        /* Adds a section to an IniStruct */
int   IniAddElem(IniStruct *, char *, char *, char *);        /* Adds an element to a given section */
int   IniDelSect(IniStruct *, char *);                        /* Deletes a section from an IniStruct */
int   IniDelElem(IniStruct *, char *, char *);                /* Deletes an element from a given section */
int   IniModSect(IniStruct *, char *, char *);                /* Modifies the name of a given section */
int   IniModElemValue(IniStruct *, char *, char *, char *);   /* Modifies the key of a given element in a given section */
int   IniModElemKey(IniStruct *, char *, char *, char *);     /* Modifies the value of a given element in a given section */
char *IniReadElem(IniStruct *, char *, char *);              /* Returns the value of an element in a given section */

/* Listing functions (lists everything in the whole struct or in a specific section) */
void IniListAll(IniStruct *);           /* List all elements and sections in a given IniStruct */
int  IniListSect(IniStruct *, char *);   /* List all elements of a given section */

/* Seek utils functions (returns 1 on success, otherwise 0) */
int IniSeekSect(IniStruct *, char *);              /* Checks if a given section exists in a given IniStruct */
int IniSeekElem(IniStruct *, char *, char *);      /* Checks if a given key exists in a given section in a given IniStruct */

/* Global section functions (utilities to handle the global part of an IniStruct) */
int   IniAddGlob(IniStruct *, char *, char *);      /* Adds a global element */
int   IniDelGlob(IniStruct *, char *);              /* Deletes a global element */
int   IniModGlobKey(IniStruct *, char *, char *);   /* Modifies a global element's key */
int   IniModGlobValue(IniStruct *, char *, char *); /* Modifies a global element's value */
char *IniReadGlob(IniStruct *, char*);              /* Returns the value of a global element */
int   IniListGlob(IniStruct *);                     /* Lists all elements of the global section */
int   IniSeekGlob(IniStruct *, char *);             /* Returns 1 if the given element exists in
													   the global section, otherwise 0 */

/* File functions, read from/write to an INI file */
IniStruct *IniReadFile(char *, char *);     /* Reads the INI data from a file and returns a new IniStruct containing that data */
void IniWriteFile(IniStruct *, char *);     /* Writes the content of an IniStruct into an INI file */

#endif
