#ifndef EXTRA_CFILESYSTEM_H
#define EXTRA_CFILESYSTEM_H

void cfilesystem_remove(const char* filepath);
int cfilesystem_exists(const char* filepath);
char* cfilesystem_filename(const char* filepath);
void cfilesystem_create_directories(const char* filepath);
char* cfilesystem_tempfilename();
char* cfilesystem_combine(const char* path1, const char* path2);

#endif
