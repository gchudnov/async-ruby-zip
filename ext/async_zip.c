#include "async_zip.h"
#include "archive.h"
#include "task.h"
#include "callback.h"

VALUE mAsyncZip, eZipError;

/* Initialize extension */
void Init_async_zip_ext()
{
	fprintf(stderr, "+Init_async_zip_ext\n");

    mAsyncZip = rb_define_module("AsyncZip");
    eZipError = rb_define_class_under(mAsyncZip, "Error", rb_eStandardError);

    //
    init_async_zip_archive();
    init_async_zip_task();
    init_async_zip_event_thread();

	fprintf(stderr, "-Init_async_zip_ext\n");
}
