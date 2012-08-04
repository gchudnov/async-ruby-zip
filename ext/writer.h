#ifndef ASYNC_ZIP_WRITER_H
#define ASYNC_ZIP_WRITER_H

typedef void* (*archive_func_t)(void*);
void az_enqueue_task(archive_func_t func, void* data);

#endif
