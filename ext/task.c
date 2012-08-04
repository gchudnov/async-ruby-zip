#include "task.h"
#include "carray.h"

VALUE cTask;

/* Get the source parameter */
static VALUE az_task_src(VALUE self)
{
    return rb_iv_get(self, "@src");
}

/* Get the destination parameter */
static VALUE az_task_dst(VALUE self)
{
    return rb_iv_get(self, "@dst");
}

/* Get the error parameter */
static VALUE az_task_error(VALUE self)
{
    return rb_iv_get(self, "@error");
}


/* Initialize the object, called before the callback is invoked */
void az_task_init(VALUE self, const char* src_path, const char* dst_path, const char* err_str, const carray_str_t* files_arr)
{
    rb_iv_set(self, "@src", (src_path ? rb_str_new2(src_path) : Qnil));
    rb_iv_set(self, "@dst", (dst_path ? rb_str_new2(dst_path) : Qnil));
    rb_iv_set(self, "@error", (err_str ? rb_str_new2(err_str) : Qnil));

    // create an array
    VALUE file_list = rb_ary_new();
    size_t len = carray_str_size(files_arr);
    if(len > 0)
    {
        size_t i;
        for(i = 0; i != len; ++i)
        {
            const char* s = carray_str_get(files_arr, i);
            rb_ary_push(file_list, rb_str_new2(s));
        }
    }

    rb_iv_set(self, "@files", file_list);
}


void init_async_zip_task()
{
    cTask = rb_define_class_under(mAsyncZip, "Task", rb_cObject);

    rb_define_method(cTask, "src", az_task_src, 0);
    rb_define_method(cTask, "dst", az_task_dst, 0);
    rb_define_method(cTask, "error", az_task_error, 0);
}
