#include "archive.h"
#include "carchive.h"
#include "cerror.h"
#include "carray.h"
#include "archive_data.h"
#include "callback.h"
#include "writer.h"


static VALUE az_create(VALUE self, VALUE zip_path, VALUE files);
static VALUE az_extract(VALUE self, VALUE zip_path, VALUE dest_path);


static void* az_archive_thread_func(void* data)
{
    archive_data_t* adata = (archive_data_t*)data;
    if(!adata)
        return NULL;

    cerror_t err = { 0 };

    if(adata->zip_path && !adata->dst_path) // create
    {
        err = carchive_create(adata->zip_path, adata->files_arr);
    }
    else if(adata->zip_path && adata->dst_path) // extract
    {
        err = carchive_extract(adata->zip_path, adata->dst_path, &adata->files_arr);
    }

    // error
    if(cerror_is_error(&err))
    {
        adata->err_str = strdup(err.message);
    }
    cerror_free_message(&err);

    az_add_to_event_qeueue(adata);

    return NULL;
}


/* Add files to archive */
static VALUE az_create(VALUE self, VALUE files, VALUE zip_path)
{
    rb_need_block();
    VALUE proc = rb_block_proc();

    int len = RARRAY_LEN(files);
    if(len > 0)
    {
        carray_str_t* parr = carray_str_create(len);

        int i;
        for(i = 0; i != len; ++i)
        {
            VALUE current = rb_ary_entry(files, i);
            if(rb_respond_to(current, rb_intern("to_s")))
            {
                VALUE name = rb_funcall(current, rb_intern("to_s"), 0);

                carray_str_set(parr, i, StringValuePtr(name));
            }
            else
            {
                carray_str_set(parr, i, NULL);
            }
        }

        archive_data_t* adata = az_make_archive_data(StringValuePtr(zip_path), NULL, parr);
        adata->proc = proc;

        rb_gc_register_address(&adata->proc);

        //
        az_enqueue_task(az_archive_thread_func, adata);
    }

    return self;
}

/* Extract files from archive */
static VALUE az_extract(VALUE self, VALUE zip_path, VALUE dest_path)
{
    rb_need_block();
    VALUE proc = rb_block_proc();

    archive_data_t* adata = az_make_archive_data(StringValuePtr(zip_path), StringValuePtr(dest_path), NULL);
    adata->proc = proc;

    rb_gc_register_address(&adata->proc);

    //
    az_enqueue_task(az_archive_thread_func, adata);

    return self;
}


/* Initialize zip pipeline */
void init_async_zip_archive(void)
{
    rb_define_singleton_method(mAsyncZip, "create", az_create, 2);
    rb_define_singleton_method(mAsyncZip, "extract", az_extract, 2);
}
