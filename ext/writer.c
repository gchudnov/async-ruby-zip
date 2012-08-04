#include "writer.h"
#include "archive_data.h"
#include <memory.h>
#include <stdlib.h>
#include <pthread.h>

pthread_t az_writer_thread;
pthread_once_t qt_once = PTHREAD_ONCE_INIT;

typedef struct _az_write_task_t
{
    archive_func_t func;
    void* data;
    struct _az_write_task_t* next;
} az_write_task_t;


/* Queue of tasks for processing. */
pthread_mutex_t az_writer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t az_writer_cond = PTHREAD_COND_INITIALIZER;
az_write_task_t* az_writer_queue = NULL;


/* Dumps write task */
static void dump_task(const char* title, az_write_task_t* wtask)
{
    if(wtask)
    {
        fprintf(stderr, "{%s}, WRITE_TASK: [%p], next: [%p]\n", title, wtask, wtask->next);
    }
    else
    {
        fprintf(stderr, "{%s}, WRITE_TASK: [NULL]\n", title);
    }
}

/* Push new task to front of the queue */
static void az_write_task_queue_push(az_write_task_t* wtask)
{
    //dump_task("az_write_task_queue_push(1)", wtask);
    wtask->next = az_writer_queue;
    az_writer_queue = wtask;
    //dump_task("az_write_task_queue_push(2)", az_writer_queue);
}

/* Pop next task from the queue; Returns NULL, when the queue is empty */
static az_write_task_t* az_write_task_queue_pop(void)
{
    az_write_task_t* wtask = az_writer_queue;
    if(wtask)
    {
        //dump_task("az_write_task_queue_pop(1)", wtask);
        az_writer_queue = wtask->next;
        //dump_task("az_write_task_queue_pop(2)", az_writer_queue);
    }

    return wtask;
}


/* Task processing queue */
static void* az_writer_thread_func(void* data)
{
    int is_running = 1;
    az_write_task_t* wtask = NULL;

    while(is_running)
    {
        pthread_mutex_lock(&az_writer_mutex);
        while (is_running && (wtask = az_write_task_queue_pop()) == NULL)
        {
            pthread_cond_wait(&az_writer_cond, &az_writer_mutex);
        }
        pthread_mutex_unlock(&az_writer_mutex);

        if(wtask)
        {
            if(wtask->func)
            {
                wtask->func(wtask->data);
            }

            free(wtask);
        }
    }

    return NULL;
}


/* Initialize the writer thread. The thread will wait for tasks to process */
static void az_init_writer_thread(void)
{
    pthread_create(&az_writer_thread, NULL, az_writer_thread_func, NULL);
    pthread_detach(az_writer_thread);
}

/* asynchronously invoke the func with the provided data */
void az_enqueue_task(archive_func_t func, void* data)
{
  pthread_once(&qt_once, az_init_writer_thread);

  pthread_mutex_lock(&az_writer_mutex);

  az_write_task_t* wtask = (az_write_task_t*)malloc(sizeof(az_write_task_t));
  memset(wtask, 0, sizeof(az_write_task_t));
  wtask->func = func;
  wtask->data = data;
  az_write_task_queue_push(wtask);

  pthread_mutex_unlock(&az_writer_mutex);
  pthread_cond_signal(&az_writer_cond);
}
