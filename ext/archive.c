#include "archive.h"
//#include "carchive.h"
//#include "cerror.h"
#include "carray.h"
#include "archive_data.h"
#include <pthread.h>

static VALUE az_create(VALUE self, VALUE zip_path, VALUE files);
static VALUE az_extract(VALUE self, VALUE zip_path, VALUE dest_path);


/* Add files to archive */
static VALUE az_create(VALUE self, VALUE zip_path, VALUE files)
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

        archive_data_t* adata = av_make_archive_data(StringValuePtr(zip_path), NULL, parr);
        adata->proc = proc;

        rb_gc_register_address(&adata->proc);

        //az_enqueue_task(az_archive_thread_func, adata);


//        // create
//        cerror_t err = carchive_create("./output.zip", parr);
//        if(cerror_is_error(&err))
//        {
//            printf("ERROR: %s\n", err.message);
//        }
//
//        cerror_free_message(&err);
//        carray_str_destroy(parr);
    }

    return self;
}

/* Extract files from archive */
static VALUE az_extract(VALUE self, VALUE zip_path, VALUE dest_path)
{
    const char* zip_path_str;
    const char* dest_path_str;

    rb_need_block();
    VALUE proc = rb_block_proc();

    archive_data_t* adata = av_make_archive_data(StringValuePtr(zip_path), StringValuePtr(dest_path), NULL);
    adata->proc = proc;

    rb_gc_register_address(&adata->proc);

    //az_enqueue_task(az_archive_thread_func, adata);



    // EXTRACT

//    err = carchive_extract("./output.zip", "./extracted");
//    if(cerror_is_error(&err))
//    {
//        printf("ERROR: %s\n", err.message);
//    }
//    cerror_free_message(&err);

    return self;
}


/* Initialize zip pipeline */
void init_async_zip_archive(void)
{
    rb_define_singleton_method(mAsyncZip, "create", az_create, 2);
    rb_define_singleton_method(mAsyncZip, "extract", az_extract, 2);
}
