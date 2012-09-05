#include "callback.h"
#include "async_zip.h"
#include "task.h"
#include "archive_data.h"
#include <pthread.h>


typedef struct _adata_wait_t
{
    archive_data_t* adata;
    int abort;
} adata_wait_t;


/* Queue of callbacks; each one invoked on a ruby thread */
pthread_mutex_t az_proc_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t az_proc_cond = PTHREAD_COND_INITIALIZER;
archive_data_t* az_proc_queue = NULL;

/* Push new callback to front of the queue */
static void az_proc_queue_push(archive_data_t* adata)
{
    fprintf(stderr, "+az_proc_queue_push\n");

    adata->next = az_proc_queue;
    az_proc_queue = adata;

    fprintf(stderr, "-az_proc_queue_push\n");
}

/* Pop next callback from the queue; Returns NULL, when the queue is empty */
static archive_data_t* az_proc_queue_pop(void)
{
    fprintf(stderr, "+az_proc_queue_pop\n");

    archive_data_t* adata = az_proc_queue;
    if(adata)
    {
        az_proc_queue = adata->next;
    }

    fprintf(stderr, "-az_proc_queue_pop\n");

    return adata;
}

/* Callback executed by Ruby Thread */
static VALUE az_handle_proc(void *d)
{
    fprintf(stderr, "+az_handle_proc\n");

    archive_data_t* adata = (archive_data_t*)d;

    // Invoke callback with task argument
    VALUE proc = (VALUE)adata->proc;

    int is_create = ((adata->zip_path && !adata->dst_path) ? 1 : 0);

    VALUE task = rb_class_new_instance(0, NULL, cTask);
    az_task_init(task, (is_create ? NULL : adata->zip_path), (is_create ? adata->zip_path : adata->dst_path), adata->err_str, adata->files_arr);
    az_free_archive_data(adata);

    rb_funcall2(proc, rb_intern("call"), 1, &task);

    rb_gc_unregister_address(&adata->proc);

    fprintf(stderr, "-az_handle_proc\n");

    return Qnil;
}


/* Wait until we have some callbacks to process  */
static VALUE az_wait_for_adata(void* w)
{
    fprintf(stderr, "+az_wait_for_adata\n");

    adata_wait_t* waiter = (adata_wait_t*)w;

    pthread_mutex_lock(&az_proc_mutex);
    while (!waiter->abort && (waiter->adata = az_proc_queue_pop()) == NULL)
    {
        pthread_cond_wait(&az_proc_cond, &az_proc_mutex);
    }
    pthread_mutex_unlock(&az_proc_mutex);

    fprintf(stderr, "-az_wait_for_adata\n");

    return Qnil;
}

/* Stop waiting for callbacks */
static void az_stop_waiting_for_adata(void* w)
{
    fprintf(stderr, "+az_stop_waiting_for_adata\n");

    adata_wait_t* waiter = (adata_wait_t*)w;

    pthread_mutex_lock(&az_proc_mutex);
    waiter->abort = 1;
    pthread_mutex_unlock(&az_proc_mutex);
    pthread_cond_signal(&az_proc_cond);

    fprintf(stderr, "-az_stop_waiting_for_adata\n");
}


/* ruby event thread, waiting for processed archives to invoke a callback */
static VALUE az_event_thread(void *unused)
{
    fprintf(stderr, "+az_event_thread\n");

    adata_wait_t waiter = { .adata = NULL, .abort = 0 };
    while (!waiter.abort)
    {
        rb_thread_blocking_region(az_wait_for_adata, &waiter, az_stop_waiting_for_adata, &waiter);
        if (waiter.adata)
        {
            rb_thread_create(az_handle_proc, waiter.adata);
        }
    }

    fprintf(stderr, "-az_event_thread\n");

    return Qnil;
}

/* Initialize Ruby Event Thread for invokation of user-provider callbacks */
void az_create_event_thread(void)
{
    fprintf(stderr, "+az_create_event_thread\n");

    rb_thread_create(az_event_thread, NULL);

    fprintf(stderr, "-az_create_event_thread\n");
}

/* Add the archive data to the event queue */
void az_add_to_event_qeueue(archive_data_t* adata)
{
    fprintf(stderr, "+az_add_to_event_qeueue\n");

    pthread_mutex_lock(&az_proc_mutex);
    az_proc_queue_push(adata);
    pthread_mutex_unlock(&az_proc_mutex);
    pthread_cond_signal(&az_proc_cond);

    fprintf(stderr, "-az_add_to_event_qeueue\n");
}

void init_async_zip_event_thread(void)
{
    az_create_event_thread();
}
