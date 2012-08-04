#include "cfilesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


// Recursive directory creation
static void _mkdir(const char *dir, mode_t mode) {

    char tmp[512];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",dir);
    len = strlen(tmp);
    if(tmp[len - 1] == '/')
            tmp[len - 1] = 0;
    for(p = tmp + 1; *p; p++)
            if(*p == '/') {
                    *p = 0;
                    mkdir(tmp, mode);
                    *p = '/';
            }
    mkdir(tmp, S_IRWXU);
}


// Remove file or directory
void cfilesystem_remove(const char* filepath)
{
    remove(filepath);
}

// Returns a positive value if path is found, otherwise 0
int cfilesystem_exists(const char* filepath)
{
    return (access(filepath, F_OK) == 0 ? 1 : 0);
}

// Get the filename for the given file path
char* cfilesystem_filename(const char* filepath)
{
    char* result = NULL;
    char* last = strrchr(filepath, '/');
    if(last)
    {
        result = strdup(last);
    }

    return result;
}

// Create directories
void cfilesystem_create_directories(const char* filepath)
{
    _mkdir(filepath, S_IRWXU | S_IRWXG | S_IRWXO);
}

// Get a temporary filename.
char* cfilesystem_tempfilename()
{
    char buffer[L_tmpnam];
    tmpnam(buffer);
    return strdup(buffer);
}

// Append one path to the other.
// Returns the combined path
char* cfilesystem_combine(const char* path1, const char* path2)
{
    char* result = NULL;

    if(!path1 && !path2) {
        // no-op
    }
    else if(!path2 || !path2[0]) {
        result = strdup(path1);
    }
    else if(!path1 || !path1[0]) {
        result = strdup(path2);
    }
    else {
        char* ptr;
        size_t len1 = strlen(path1);
        size_t len2 = strlen(path2);

        size_t newlen = len1 + len2 + 1 + ((path1[len1 - 1] != '/' && path2[0] != '/') ? 1 : 0);
        result = (char*)malloc(newlen);
        ptr = result;

        strcpy(ptr, path1);
        ptr += len1;

        if((ptr[-1] != '/') && (path2[0] != '/'))
        {
            ptr[0] = '/';
            ++ptr;
        }

        strcpy(ptr, path2);
    }

    return result;
}
