#ifndef EXTRA_CERROR_H
#define EXTRA_CERROR_H

#define CERROR_E_ZIP_OPEN_FAILED          1
#define CERROR_E_ZIP_FILE_NOT_FOUND       2
#define CERROR_E_ZIP_CREATE_DATA_SOURCE   3
#define CERROR_E_ZIP_ADD_FILE             4
#define CERROR_E_ZIP_CLOSE_ARCHIVE        5
#define CERROR_E_ZIP_ENTRY_COUNT          6
#define CERROR_E_ZIP_ENTRY_READ_STATS     7
#define CERROR_E_ZIP_ENTRY_READ_ENTRY     8
#define CERROR_E_ZIP_ENTRY_READ_BYTES     9
#define CERROR_E_ZIP_ENTRY_CLOSE          10
#define CERROR_E_FILE_CREATE              16
#define CERROR_E_FILE_WRITE               17



/* Structure for error reporting */
typedef struct
{
   int code;
   char* message;
} cerror_t;

/* Create & Destroy */
cerror_t* cerror_create(int code, const char* text);
void cerror_destroy(cerror_t* perr);

int cerror_asprintf(char** ppasz, const char* format, ...);
void cerror_free_message(cerror_t* perr);

int cerror_is_error(cerror_t* perr);

#endif
