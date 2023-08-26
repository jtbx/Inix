#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inix.h"

/*
* Hidden utilities functions
*/

/* Checks if a string is in no cap, only letters and numbers and is between 1 and 32 characters, included */
int IsNameValid(char *name) {
    int i = 0;
    if (name[i] == '\0') return 0;
    while (name[i] != '\0') {
        if (!((name[i] >= 48 && name[i] <= 57) || (name[i] >= 97 && name[i] <= 122)) || i > 31)
            return 0;
        ++i;
    }
    return 1;
}

/*
* INIstruct core functions
*/

/* Initializes a new INIstruct and return the address of it */
IniStruct *IniMake(char *name) {
    if (!IsNameValid(name)) return NULL;
    IniStruct *ini = (IniStruct *)malloc(sizeof(IniStruct));
    strcpy(ini->name, name);
    ini->sects = (IniSection *)malloc(sizeof(IniSection) * 2);
    strcpy(ini->sects[0].name, "$");
    strcpy(ini->sects[1].name, "_");
    ini->sects->elems = (IniElement *)malloc(sizeof(IniElement));
    strcpy(ini->sects->elems->key, "_");
    strcpy(ini->sects->elems->value, "0");
    return ini;
}

/* Gets the adress of an INIstruct and free it */
void IniEnd(IniStruct *ini) {
    int i = 0;
    while (strcmp(ini->sects[i].name, "_") != 0) {
        free(ini->sects[i].elems);
        ++i;
    }
    free(ini->sects);
    free(ini);
    return;
}

/* Gets the adress of an INIstruct and reset it */
void IniClear(IniStruct *ini) {
    int i = 0;
    char old_name[32] = "_";
    strcpy(old_name, ini->name);
    while (strcmp(ini->sects[i].name, "_") != 0) {
        free(ini->sects[i].elems);
        ++i;
    }
    free(ini->sects);
    free(ini);
    IniStruct *new_ini = (IniStruct *)malloc(sizeof(IniStruct));
    strcpy(new_ini->name, old_name);
    new_ini->sects = (IniSection *)malloc(sizeof(IniSection) * 2);
    strcpy(new_ini->sects[0].name, "$");
    strcpy(new_ini->sects[1].name, "_");
    new_ini->sects->elems = (IniElement *)malloc(sizeof(IniElement));
    strcpy(new_ini->sects->elems->key, "_");
    strcpy(new_ini->sects->elems->value, "0");
    ini = new_ini;
    return;
}

/*
* INIstruct input/output functions
*/

/* Adds a section to a given INIstruct */
int IniAddSect(IniStruct *ini, char *sect_name) {
    if (!IsNameValid(sect_name)) return 1;
    if (IniSeekSect(ini, sect_name)) return 1;
    int i = 0;
    while (strcmp(ini->sects[i].name, "_") != 0) ++i;
    ini->sects = (IniSection *)realloc(ini->sects, (i + 2) * sizeof(IniSection));
    strcpy(ini->sects[i].name, sect_name);
    strcpy(ini->sects[i + 1].name, "_");
    ini->sects[i].elems = (IniElement *)malloc(sizeof(IniElement));
    strcpy(ini->sects[i].elems->key, "_");
    strcpy(ini->sects[i].elems->value, "0");
    return 0;
}

/* Adds an element to a given section */
int IniAddElem(IniStruct *ini, char *sect_name, char *elem_key, char *elem_value) {
    if (!(IsNameValid(sect_name) && IsNameValid(elem_key) && IsNameValid(elem_value))) return 1;
    if (IniSeekElem(ini, sect_name, elem_key)) return 1;
    int i = 0;
    while (strcmp(ini->sects[i].name, "_") != 0) {
        if (strcmp(ini->sects[i].name, sect_name) == 0) {
            int c = 0;
            while (strcmp(ini->sects[i].elems[c].key, "_") != 0) ++c;
            ini->sects[i].elems = (IniElement *)realloc(ini->sects[i].elems, (c + 2) * sizeof(IniElement));
            strcpy(ini->sects[i].elems[c].key, elem_key);
            strcpy(ini->sects[i].elems[c].value, elem_value);
            strcpy(ini->sects[i].elems[c + 1].key, "_");
            strcpy(ini->sects[i].elems[c + 1].value, "0");
            return 0;
        }
        ++i;
    }
    return 1;
}

/* Removes a section from a given struct */
int IniDelSect(IniStruct *ini, char *sect_name) {
    if (!IsNameValid(sect_name)) return 1;
    int i = 0;
    while (strcmp(ini->sects[i].name, "_") != 0) {
        if (strcmp(ini->sects[i].name, sect_name) == 0) {
            free(ini->sects[i].elems);
            while (strcmp(ini->sects[i].name, "_") != 0) {
                strcpy(ini->sects[i].name, ini->sects[i + 1].name);
                ini->sects[i].elems = ini->sects[i + 1].elems;
                ++i;
            }
            strcpy(ini->sects[i].name, "\0");
            ini->sects = (IniSection *)realloc(ini->sects, i * sizeof(IniSection));
            return 0;
        }
        ++i;
    };
    return 1;
}

/* Removes an element from a given section */
int IniDelElem(IniStruct *ini, char *sect_name, char *elem_key) {
    if (!(IsNameValid(sect_name) && IsNameValid(elem_key))) return 1;
    int i = 0;
    while  (strcmp(ini->sects[i].name, "_") != 0) {
        if (strcmp(ini->sects[i].name, sect_name) == 0) {
            int c = 0;
            while(strcmp(ini->sects[i].elems[c].key, "_") != 0) {
                if (strcmp(ini->sects[i].elems[c].key, elem_key) == 0) {
                    while(strcmp(ini->sects[i].elems[c].key, "_") != 0) {
                        strcpy(ini->sects[i].elems[c].key, ini->sects[i].elems[c + 1].key);
                        strcpy(ini->sects[i].elems[c].value, ini->sects[i].elems[c + 1].value);
                        ++c;
                    }
                    strcpy(ini->sects[i].elems[c].key, "\0");
                    strcpy(ini->sects[i].elems[c].value, "\0");
                    ini->sects[i].elems = (IniElement *)realloc(ini->sects[i].elems, c * sizeof(IniElement));
                    return 0;
                }
                ++c;
            }
            return 1;
        }
        ++i;
    }
    return 1;
}

/* Renames a section in a given INIstruct */
int IniModSect(IniStruct *ini, char *old_name, char *new_name) {
    if (!(IsNameValid(old_name) && IsNameValid(new_name))) return 1;
    if (IniSeekSect(ini, new_name)) return 1;
    int i = 0;
    while (strcmp(ini->sects[i].name, "_") != 0) {
        if (strcmp(ini->sects[i].name, old_name) == 0) {
            strcpy(ini->sects[i].name, new_name);
            return 0;
        }
        ++i;
    }
    return 1;
}

/* Modifies the key of an element in a given section */
int IniModElemKey(IniStruct *ini, char *sect_name, char *old_key, char *new_key) {
    if (!(IsNameValid(sect_name) && IsNameValid(old_key) && IsNameValid(new_key))) return 1;
    if (IniSeekElem(ini, sect_name, new_key)) return 1;
    int i = 0;
    while (strcmp(ini->sects[i].name, "_") != 0) {
        if (strcmp(ini->sects[i].name, sect_name) == 0) {
            int c = 0;
            while (strcmp(ini->sects[i].elems[c].key, "_") != 0) {
                if (strcmp(ini->sects[i].elems[c].key, old_key) == 0) {
                    strcpy(ini->sects[i].elems[c].key, new_key);
                    return 0;
                }
                ++c;
            }
            return 1;
        }
        ++i;
    }
    return 1;
}

/* Modifies the value of an element in a given section */
int IniModElemValue(IniStruct *ini, char *sect_name, char *elem_key, char *elem_value) {
    if (!(IsNameValid(sect_name) && IsNameValid(elem_key) && IsNameValid(elem_value))) return 1;
    int i = 0;
    while (strcmp(ini->sects[i].name, "_") != 0) {
        if (strcmp(ini->sects[i].name, sect_name) == 0) {
            int c = 0;
            while (strcmp(ini->sects[i].elems[c].key, "_") != 0) {
                if (strcmp(ini->sects[i].elems[c].key, elem_key) == 0) {
                    strcpy(ini->sects[i].elems[c].value, elem_value);
                    return 0;
                }
                ++c;
            }
            return 1;
        }
        ++i;
    }
    return 1;
}

/* Reads the value of a given element in a given section */
char *IniReadElem(IniStruct *ini, char *sect_name, char*elem_key) {
    if (!(IsNameValid(sect_name) && IsNameValid(elem_key))) return NULL;
    int i = 0;
    while (strcmp(ini->sects[i].name, "_") != 0) {
        if (strcmp(ini->sects[i].name, sect_name) == 0) {
            int c = 0;
            while (strcmp(ini->sects[i].elems[c].key, "_") != 0) {
                if (strcmp(ini->sects[i].elems[c].key, elem_key) == 0)
                    return ini->sects[i].elems[c].value;
                ++c;
            }
            return NULL;
        }
        ++i;
    }
    return NULL;
}

/*
* INIstruct listing functions
*/

/* Lists all sections and elements in a given INIstruct */
void IniListAll(IniStruct *ini) {
    int i = 1;
    printf("\n{%s}\n", ini->name);
    while (strcmp(ini->sects[i].name, "_") != 0) {
        int c = 0;
        printf("[%s]\n", ini->sects[i].name);
        while (strcmp(ini->sects[i].elems[c].key, "_") != 0) {
            printf("%s = %s\n", ini->sects[i].elems[c].key, ini->sects[i].elems[c].value);
            ++c;
        }
        ++i;
    }
    return;
}

/* Lists all elements in a given INIstruct */
int IniListSect(IniStruct *ini, char* sect_name) {
    if (!IsNameValid(sect_name)) return 1;
    int i = 0;
    printf("\n{%s}\n", ini->name);
    while (strcmp(ini->sects[i].name, "_") != 0) {
        if (strcmp(ini->sects[i].name, sect_name) == 0) {
            int c = 0;
            printf("[%s]\n", ini->sects[i].name);
            while (strcmp(ini->sects[i].elems[c].key, "_") != 0) {
                printf("%s = %s\n", ini->sects[i].elems[c].key, ini->sects[i].elems[c].value);
                ++c;
            }
            return 0;
        }
        ++i;
    }
    return 1;
}

/*
* INIstruct seek functions
*/

/* Checks if the name of the section already exists in the struct, and returns one if yes */
int IniSeekSect(IniStruct *ini, char *sect_name) {
    if (!IsNameValid(sect_name)) return 0;
    int i = 0;
    while (strcmp(ini->sects[i].name, "_") != 0) {
        if (strcmp(ini->sects[i].name, sect_name) == 0) return 1;
        ++i;
    }
    return 0;
}

/* Checks if the name of a key already exists in the given section, and returns one if yes */
int IniSeekElem(IniStruct *ini, char *sect_name, char *elem_key) {
    if (!(IsNameValid(sect_name) && IsNameValid(elem_key))) return 0;
    int i = 0;
    while (strcmp(ini->sects[i].name, "_") != 0) {
        if (strcmp(ini->sects[i].name, sect_name) == 0) {
            int c = 0;
            while (strcmp(ini->sects[i].elems[c].key, "_") != 0) {
                if (strcmp(ini->sects[i].elems[c].key, elem_key) == 0) return 1;
                ++c;
            }
            return 0;
        }
        ++i;
    }
    return 0;
}

/*
* INI Global functions
*/

/* Checks if the given elements exists in global */
int IniSeekGlob(IniStruct *ini, char *elem_key) {
    if (!IsNameValid(elem_key)) return 0;
    int i = 0;
    while (strcmp(ini->sects[0].elems[i].key, "_") != 0) {
        if (strcmp(ini->sects[0].elems[i].key, elem_key) == 0) return 1;
        ++i;
    }
    return 0;
}

/* Adds an element to the global section */
int IniAddGlob(IniStruct *ini, char *elem_key, char *elem_value) {
    if (!(IsNameValid(elem_key) && IsNameValid(elem_value))) return 1;
    if (IniSeekGlob(ini, elem_key)) return 1;
    int i = 0;
    while (strcmp(ini->sects[0].elems[i].key, "_") != 0) ++i;
    ini->sects[0].elems = (IniElement *)realloc(ini->sects[0].elems, (i + 2) * sizeof(IniElement));
    strcpy(ini->sects[0].elems[i].key, elem_key);
    strcpy(ini->sects[0].elems[i].value, elem_value);
    strcpy(ini->sects[0].elems[i + 1].key, "_");
    strcpy(ini->sects[0].elems[i + 1].value, "0");
    return 0;
}

/* Deletes an element from the global section */
int IniDelGlob(IniStruct *ini, char *elem_key) {
    if (!IsNameValid(elem_key)) return 1;
    int i = 0;
    while (strcmp(ini->sects[0].elems[i].key, "_") != 0) {
        if (strcmp(ini->sects[0].elems[i].key, elem_key) == 0) {
            while(strcmp(ini->sects[0].elems[i].key, "_") != 0) {
                strcpy(ini->sects[0].elems[i].key, ini->sects[0].elems[i + 1].key);
                strcpy(ini->sects[0].elems[i].value, ini->sects[0].elems[i + 1].value);
                ++i;
            }
            strcpy(ini->sects[0].elems[i].key, "\0");
            strcpy(ini->sects[0].elems[i].value, "\0");
            ini->sects[0].elems = (IniElement *)realloc(ini->sects[0].elems, i * sizeof(IniElement));
            return 0;
        }
        ++i;
    }
    return 1;
}

/* Modifies the key of an element in the global sections */
int IniModGlobKey(IniStruct *ini, char *old_key, char *new_key) {
    if (!(IsNameValid(old_key) && IsNameValid(new_key))) return 1;
    if (IniSeekGlob(ini, new_key)) return 1;
    int i = 0;
    while (strcmp(ini->sects[0].elems[i].key, "_") != 0) {
        if (strcmp(ini->sects[0].elems[i].key, old_key) == 0) {
            strcpy(ini->sects[0].elems[i].key, new_key);
            return 0;
        }
        ++i;
    }
    return 1;
}

/* Modifies the value of an element in the global sections */
int IniModGlobValue(IniStruct *ini, char *elem_key, char *elem_value) {
    if (!(IsNameValid(elem_key) && IsNameValid(elem_value))) return 1;
    int i = 0;
    while (strcmp(ini->sects[0].elems[i].key, "_") != 0) {
        if (strcmp(ini->sects[0].elems[i].key, elem_key) == 0) {
            strcpy(ini->sects[0].elems[i].value, elem_value);
            return 0;
        }
        ++i;
    }
    return 1;
}

/* Reads the value from an element in the global section */
char *IniReadGlob(IniStruct *ini, char*elem_key) {
    if (!IsNameValid(elem_key)) return NULL;
    int i = 0;
    while (strcmp(ini->sects[0].elems[i].key, "_") != 0) {
        if (strcmp(ini->sects[0].elems[i].key, elem_key) == 0)
            return ini->sects[0].elems[i].value;
        ++i;
    }
    return NULL;
}

/* Lists all the element in the global section */
int IniListGlob(IniStruct *ini) {
    int i = 0;
    printf("\n{%s}\n", ini->name);
    printf("[$global]\n");
    while (strcmp(ini->sects[0].elems[i].key, "_") != 0) {
        printf("%s = %s\n", ini->sects[0].elems[i].key, ini->sects[0].elems[i].value);
        ++i;
    }
    return 0;
}

/*
* File related functions
*/

IniStruct *IniReadFile(char *path, char *struct_name) {
    
    /* Variable declaration */
    FILE *file = fopen(path, "r");
    char buffer[BUFFER_SIZE] = "_";
    IniStruct *ini = IniMake(struct_name);
    char section[32] = "$";
    char key[32] = "_";
    char value[32] = "_";
    int i = 0;
    int c = 0;

    /* Parser code */
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {

        i = 0;

        /* Ignores all whitespaces until a character is reached, stops if i is too big */
        while (buffer[i] == ' ') {
            ++i;
        }

        /* Starts the section parsing code when '[' character encoutered */
        if (buffer[i] == '[') {
            ++i;
            c = 0;
            strcpy(section, "_");
            while(buffer[i] != ']') {
                section[c] = buffer[i];
                ++i;
                ++c;
            }
            section[c] = '\0';
            IniAddSect(ini, section);
            continue;
        }

        /* Starts the key parsing code when any no cap letter or number is encountered */
        if ((buffer[i] >= 48 && buffer[i] <= 57) || (buffer[i] >= 97 && buffer[i] <= 122)) {
            c = 0;
            strcpy(key, "_");
            while(buffer[i] != ' ' && buffer[i] != '=') {
                key[c] = buffer[i];
                ++i;
                ++c;
            }
            key[c] = '\0';
            if (buffer[i] == ' ') {
                while (buffer[i] != '=') ++i;
            }
            ++i;
            while (buffer[i] == ' ') ++i;
            c = 0;
            strcpy(value, "_");
            while ((buffer[i] >= 48 && buffer[i] <= 57) || (buffer[i] >= 97 && buffer[i] <= 122)) {
                value[c] = buffer[i];
                ++i;
                ++c;
            }
            value[c] = '\0';
            if (strcmp(section, "$") == 0) IniAddGlob(ini, key, value);
            else IniAddElem(ini, section, key, value);
            continue;
        }

        /* Skips the line and continue the loop when encountering comment or other EOL character */
        if (buffer[i] == ';' || buffer[i] == '#' || buffer[i] == '\n' || buffer[i] == '\0') continue;

        /* If the character encountered is none of the above then it's an error, return NULL and free INI */
        else {
            IniEnd(ini);
            return NULL;
        }
    }
    return ini;
}

void IniWriteFile(IniStruct *ini, char *path) {
    
    /* Variable declaration */
    FILE *file = fopen(path, "w");
    int i = 1;
    int c = 0;

    /* Writes to the file */
    while (strcmp(ini->sects[0].elems[c].key, "_") != 0) {
        fprintf(file, "%s = %s\n", ini->sects[0].elems[c].key, ini->sects[0].elems[c].value);
        ++c;
    }

    c = 0;
    fprintf(file, "\n");

    while (strcmp(ini->sects[i].name, "_") != 0) {
        c = 0;
        fprintf(file, "[%s]\n", ini->sects[i].name);
        while (strcmp(ini->sects[i].elems[c].key, "_") != 0) {
            fprintf(file, "%s = %s\n", ini->sects[i].elems[c].key, ini->sects[i].elems[c].value);
            ++c;
        }
        fprintf(file, "\n");
        ++i;
    }
    return;
}
