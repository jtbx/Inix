# Introduction
* Inix is is a small INI file handling library that will allow you to read, store and write INI file informations.
* The way INI stores the INI data is by using a custom structure called an INIstruct.
* Inix comes with a lot of usefull functions to handle INIstruct, these functions allow you to Add, Delete, Modify and read sections and keys of a struct.
* Both the INIstruct structure and the functions declaration, as well as a few usefull macros are defined in the header file.
* Finally Inix will allow you to parse an INI file and store the data in an INIstruct and write to an INI file using the INIstruct's data.
# List of functions
Functions may or may not fail, some function can fail due to a faulty parameter (name too long, or presence of forbiden characters), or other things
in which case it will return either 1 or NULL.
A name must be composed of only lowercase latin letters and numbers, no specials character is allowed, not even space.
When creating a section or an element, if the name of the section/element already exists in the file/section, it will fail and return 1/NULL;

* IniMake(name)         // Takes the structure name as an argument then creates a mallocated INIstruct with the given name and returns it's address.
* IniClear(ini)         // Takes a pointer to a INIstruct and free it then give it the adress of a new struct.
* IniEnd(ini)           // Takes a pointer to a INIstruct and free it.

* IniAddSect(ini, sect_name)                         // Takes a pointer to an INIstruct and add a section to it with the given name.
* IniAddElem(ini, sect_name, key, value)             // Takes a pointer to an INIstruct and add an element to the given section in the struct.
* IniDelSect(ini, sect_name)                         // Takes a pointer to an INIstruct and delete the given section.
* IniDelElem(ini, sect_name, key)                    // Takes a pointer to an INIstruct and delete the given element in the given section.
* IniModSect(ini, sect_name, new_sect_name)          // Takes a pointer to an INIstruct and modify the name of the section.
* IniModElemKey(ini, sect_name, old_key, new_key)    // Takes a pointer to an INIstruct and mofify a key in a given section.
* IniModElemValue(ini, sect_name, key, new_value)    // Takes a pointer to an INIstruct and modify a value of a given key in a given section.
* IniReadElem(ini, sect_name, key)                   // Takes a pointer to an INIstruct and returns the value of a given key in a given section.

* IniListAll(ini)      // Takes a pointer to an INIstruct and list everything (excluding the global section).
* IniListSect(ini)     // Takes a pointer to an INIstruct and list eveything within a given section.

* IniSeekSect(ini, sect_name)         // Takes a pointer to an INIstruct and returns true/1 if a section with the given name is found
* IniSeekElem(ini, sect_name, key)    // Takes a pointer to an INIstruct and returns true/1 if an element with the given key in a section is found.

* IniAddGlob(ini, key, value)       // Takes a pointer to an INIstruct and add an element to it's global section
* IniDelGlob(ini, key)              // 

# Misc
* The INI parser of Inix is a work in progress, it does handle error, and faulty INI syntax, to avoid undefined behavior please follow these rules:
    * Only use lowercase latin letters and numbers when creating a section, a key, or a value.
    * Do not make a lines bigger than 200 digits, else undefined behavior could be could occur.
    * Do not make 2 section with the same name in a file or 2 elements with the same key in the same section.
* A few macros are available, INIX_VERSION = the version of inix, INIX_AUTHOR = the author of inix, INIX_LICENSE = the license of inix.
