#include "async_zip.h"
#include "archive.h"

VALUE mAsyncZip, eZipError;

/* Initialize extension */
void Init_async_zip_ext()
{
    mAsyncZip = rb_define_module("AsyncZip");
    eZipError = rb_define_class_under(mAsyncZip, "Error", rb_eStandardError);

    //
    init_async_zip_archive();
}
