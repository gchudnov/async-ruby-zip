#ifndef ASYNC_ZIP_CALLBACK_H
#define ASYNC_ZIP_CALLBACK_H

#include "archive_data.h"

void az_add_to_event_qeueue(archive_data_t* tdata);
void init_async_zip_event_thread(void);

#endif
