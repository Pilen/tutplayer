#include <stdlib.h>
#include <stdbool.h>

#include "assert.h"
#include "debug.h"
#include "log.h"
#include "memory.h"
#include "queue.h"

struct Queue_node_s {
    void *data;
    Queue_node *next;
};

struct Queue_s {
    Queue_node *all; /* Only used for cleaning */
    Queue_node *read;
    Queue_node *last;
    pthread_mutex_t write_lock;
    unsigned int length;
};

Queue *queue_create(void) {
    Queue *queue = TUT_MALLOCZ(Queue);
    Queue_node *node = TUT_MALLOCZ(Queue_node);
    queue -> all = node;
    queue -> read = node;
    queue -> last = node;

    pthread_mutex_init(&queue -> write_lock, NULL);

    return queue;
}

void queue_add(Queue *queue, void *data) {
    /* In the critical section, the queue is in a slightly inconsistent state
       where the length might differ*/
    t_assert(queue);

    pthread_mutex_lock(&queue -> write_lock);
    Queue_node *read;
    __atomic_load(&queue -> read, &read, __ATOMIC_SEQ_CST);
    Queue_node *node;
    if (queue -> all == read) {
        node = TUT_MALLOCZ(Queue_node);
    } else {
        /* We should never get a nullpointer before reaching read */
        node = queue -> all;
        queue -> all = node -> next;
    }
    node -> data = data;
    node -> next = NULL;

    queue -> last = node;
    __atomic_store(&queue -> last -> next, &node, __ATOMIC_SEQ_CST);
    __atomic_add_fetch(&queue -> length, 1, __ATOMIC_SEQ_CST);
    pthread_mutex_unlock(&queue -> write_lock);
}

void *queue_read(Queue *queue) {
    /* ONLY ONE READER MAY BE HERE AT A TIME */
    /* Reading `read` does not have to be atomic as only this function may change it */
    Queue_node *node = NULL;
    __atomic_load(&queue -> read -> next, &node, __ATOMIC_SEQ_CST);
    if (node == NULL) {
        /* The queue is empty */
        return NULL;
    }
    __atomic_store(&queue -> read, &node, __ATOMIC_SEQ_CST);
    __atomic_add_fetch(&queue -> length, -1, __ATOMIC_SEQ_CST);
    void *data = node -> data;
    node -> data = NULL;
    return data;
}

unsigned int queue_cleanup(Queue *queue) {
    t_assert(queue);
    unsigned int cleaned = 0;
    pthread_mutex_lock(&queue -> write_lock);
    Queue_node *node = queue -> all;
    Queue_node *read;
    __atomic_load(&queue -> read, &read, __ATOMIC_SEQ_CST);
    while (node != read) {
        /* We should never get a nullpointer before reaching read */
        node = queue -> all;
        queue -> all = node -> next;
        tut_free(node);
        cleaned++;
    }
    pthread_mutex_unlock(&queue -> write_lock);
    return cleaned;
}

unsigned int queue_approximate_length(Queue *queue) {
    unsigned int length;
    __atomic_load(&queue -> length, &length, __ATOMIC_SEQ_CST);
    return length;
}
