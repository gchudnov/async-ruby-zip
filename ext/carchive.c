#include "carchive.h"
#include "carray.h"
#include "cfilesystem.h"
#include "cerror.h"
#include <zip.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


// Format error message from ZIP-archive
void cerror_zip_asprintf(char** ppmsg, struct zip* z, const char* text)
{
    if(!z)
        return;

    if(text)
    {
        cerror_asprintf(ppmsg, "%s: %s", text, zip_strerror(z));
    }
    else
    {
        cerror_asprintf(ppmsg, "%s", zip_strerror(z));
    }
}


// Put files to the archive
cerror_t carchive_create(const char* zip_path, const carray_str_t* parr)
{
    cerror_t result = { 0 };

    cfilesystem_remove(zip_path);

    int err = 0;
    struct zip* z = zip_open(zip_path, ZIP_CREATE, &err);
    if(!z)
    {
        result.code = CERROR_E_ZIP_OPEN_FAILED;
        cerror_asprintf(&result.message, "cannot open zip archive for writing: '%s'", zip_path);
        return result;
    }

    size_t i;
    for(i = 0; i != parr->size; ++i)
    {
        const char* filepath = carray_str_get(parr, i);
        if(!cfilesystem_exists(filepath))
        {
            result.code = CERROR_E_ZIP_FILE_NOT_FOUND;
            cerror_asprintf(&result.message, "file not found: '%s'", filepath);
            break;
        }

        struct zip_source* s = zip_source_file(z, filepath, 0, -1);
        if(!s)
        {
            result.code = CERROR_E_ZIP_CREATE_DATA_SOURCE;
            cerror_zip_asprintf(&result.message, z, "cannot create data source");
            break;
        }

        char* filename = cfilesystem_filename(filepath);
        err = zip_add(z, filename, s);
        if(err < 0)
        {
            result.code = CERROR_E_ZIP_ADD_FILE;
            cerror_zip_asprintf(&result.message, z, "cannot add file to archive");
            free(filename);
            break;
        }

        free(filename);
    }

    if(cerror_is_error(&result))
    {
        zip_unchange_all(z);
        zip_close(z);
    }
    else
    {
        err = zip_close(z);
        if(err < 0)
        {
            result.code = CERROR_E_ZIP_CLOSE_ARCHIVE;
            cerror_zip_asprintf(&result.message, z, "cannot close zip archive");
        }
    }

    return result;
}


// Extract files from the archive
cerror_t carchive_extract(const char* zip_path, const char* dest_path, carray_str_t** pparr)
{
    cerror_t result = { 0 };

    int err = 0;
    struct zip* z = zip_open(zip_path, 0, &err);
    if(!z)
    {
        result.code = CERROR_E_ZIP_OPEN_FAILED;
        cerror_asprintf(&result.message, "cannot open zip archive for reading: '%s'", zip_path);
        return result;
    }

    uint64_t num = zip_get_num_entries(z, 0);
    if(num == (uint64_t)-1)
    {
        result.code = CERROR_E_ZIP_ENTRY_COUNT;
        cerror_zip_asprintf(&result.message, z, "cannot get the number of entities in archive");
    }
    else
    {
        if(!cfilesystem_exists(dest_path))
            cfilesystem_create_directories(dest_path);

        *pparr = carray_str_create((size_t)num);

        struct zip_stat st = { 0 };
        uint64_t i;
        for(i = 0; i != num; ++i)
        {
            zip_stat_init(&st);
            err = zip_stat_index(z, i, 0, &st);
            if(err < 0)
            {
                result.code = CERROR_E_ZIP_ENTRY_READ_STATS;
                cerror_zip_asprintf(&result.message, z, "cannot read statistics for entry");
                break;
            }

            if(st.valid & ZIP_STAT_SIZE)
            {
                struct zip_file* f = zip_fopen_index(z, i, 0);
                if(!f)
                {
                    result.code = CERROR_E_ZIP_ENTRY_READ_ENTRY;
                    cerror_zip_asprintf(&result.message, z, "cannot read entry");
                    break;
                }

                char* filename = (st.valid & ZIP_STAT_NAME ? strdup(st.name) : cfilesystem_tempfilename());
                char* dest_filepath = cfilesystem_combine(dest_path, filename);

                carray_str_set(*pparr, (size_t)i, dest_filepath);

                FILE* fp = fopen(dest_filepath, "w");
                if(!fp)
                {
                    result.code = CERROR_E_FILE_CREATE;
                    cerror_asprintf(&result.message, "cannot create file for writing: '%s', [%s]", dest_filepath, strerror(errno));
                    free(filename);
                    free(dest_filepath);
                    break;
                }

                char* bytes = (char*)malloc(st.size);
                err = zip_fread(f, bytes, st.size);
                if(err < 0)
                {
                    result.code = CERROR_E_ZIP_ENTRY_READ_BYTES;
                    cerror_zip_asprintf(&result.message, z, "cannot read entry byts");
                    free(bytes);
                    free(filename);
                    free(dest_filepath);
                    fclose(fp);
                    break;
                }

                err = fwrite(bytes, st.size, 1, fp);
                if(!err)
                {
                    result.code = CERROR_E_FILE_WRITE;
                    cerror_asprintf(&result.message, "cannot writing data to file: '%s', [%s]", dest_filepath, strerror(errno));
                    free(bytes);
                    free(filename);
                    free(dest_filepath);
                    fclose(fp);
                    break;
                }

                fclose(fp);

                free(bytes);
                free(filename);
                free(dest_filepath);

                err = zip_fclose(f);
                if(err)
                {
                    result.code = CERROR_E_ZIP_ENTRY_CLOSE;
                    cerror_zip_asprintf(&result.message, z, "cannot close zip entry");
                    break;
                }
            }
        }
    }

    if(cerror_is_error(&result))
    {
        zip_close(z);
    }
    else
    {
        err = zip_close(z);
        if(err < 0)
        {
            result.code = CERROR_E_ZIP_CLOSE_ARCHIVE;
            cerror_zip_asprintf(&result.message, z, "cannot close zip archive");
        }
    }

    return result;
}
