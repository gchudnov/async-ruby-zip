#ifndef EXTRA_CARCHIVE_H
#define EXTRA_CARCHIVE_H

#include "carray.h"
#include "cerror.h"

cerror_t carchive_create(const char* zip_path, const carray_str_t* parr);
cerror_t carchive_extract(const char* zip_path, const char* dest_path);

#endif
