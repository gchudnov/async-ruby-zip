#ifndef ASYNC_ZIP_ARCHIVE_DATA_H
#define ASYNC_ZIP_ARCHIVE_DATA_H

#include "ruby.h"
#include "carray.h"

typedef struct _archive_data_t
{
    char* zip_path; // source zip-file
    char* dst_path; // destination folder to extract zip-file { used only for extraction }

    carray_str_t* files_arr; // array of files to archive or files that were extracted

    VALUE proc;
    char* err_str;

    struct _archive_data_t* next;
} archive_data_t;


archive_data_t* av_make_archive_data(const char* zip_path, const char* dst_path, carray_str_t* files_arr);
void av_free_archive_data(archive_data_t* adata);

#endif

