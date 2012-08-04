#ifndef ASYNC_ZIP_TASK_H
#define ASYNC_ZIP_TASK_H

#include "async_zip.h"
#include "carray.h"

extern VALUE cTask;

void az_task_init(VALUE self, const char* src_path, const char* dst_path, const char* err_str, const carray_str_t* files_arr);

void init_async_zip_task();

#endif
