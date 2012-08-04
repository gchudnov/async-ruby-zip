#include "archive_data.h"

// Create a new archive-data structure
archive_data_t* az_make_archive_data(const char* zip_path, const char* dst_path, carray_str_t* files_arr)
{
    archive_data_t* adata = (archive_data_t*)malloc(sizeof(archive_data_t));
    if(adata)
    {
        memset(adata, 0, sizeof(archive_data_t));

        if(zip_path)
        {
            adata->zip_path = strdup(zip_path);
        }

        if(dst_path)
        {
            adata->dst_path = strdup(dst_path);
        }

        if(files_arr)
        {
            adata->files_arr = files_arr;
        }
    }

    return adata;
}

// Free archive-data structure
void az_free_archive_data(archive_data_t* adata)
{
    if(!adata)
        return;

    if(adata->zip_path)
    {
        free(adata->zip_path);
        adata->zip_path = NULL;
    }

    if(adata->dst_path)
    {
        free(adata->dst_path);
        adata->dst_path = NULL;
    }

    if(adata->err_str)
    {
        free(adata->err_str);
        adata->err_str = NULL;
    }

    if(adata->files_arr)
    {
        carray_str_destroy(adata->files_arr);
        adata->files_arr = NULL;
    }

    free(adata);
}
