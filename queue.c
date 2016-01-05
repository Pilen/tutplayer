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
    unsigned int length;
};

Queue *queue_create(void) {
    Queue *queue = TUT_MALLOCZ(Queue);
    Queue_node *node = TUT_MALLOCZ(Queue_node);
    queue -> all = node;
    queue -> read = node;
    queue -> last = node;

    return queue;
}

void queue_add(Queue *queue, void *data) {
    t_assert(queue);
    Queue_node *node = TUT_MALLOCZ(Queue_node);
    node -> data = data;
    node -> next = NULL;

    /* Add node to queue atomically */
    Queue_node *last = queue -> last;
    Queue_node *expected = NULL;
    do {
        expected = NULL;
        while (last -> next != NULL) {
            last = last -> next;
        }
    } while (__atomic_compare_exchange(&last -> next, &expected, &node, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST) == false);

    /* From now on the queue is in a slightly inconsistent state
       the length might differ and the last pointer might not point to the actual last.
       But by following it manually and using the length as approximate it is okay*/

    __atomic_store(&queue -> last, &node, __ATOMIC_SEQ_CST);
    /* Update length */
    __atomic_add_fetch(&queue -> length, 1, __ATOMIC_SEQ_CST);
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
    t_assert(false);
    Queue_node *node = queue -> all;
    Queue_node *read;
    __atomic_load(&queue -> read, &read, __ATOMIC_SEQ_CST);
    unsigned int cleaned = 0;
    while (node != read) {
        queue -> all = node -> next;
        tut_free(node);
        node = queue -> all;
        cleaned++;
    }
    return cleaned;
}

unsigned int queue_approximate_length(Queue *queue) {
    unsigned int length;
    __atomic_load(&queue -> length, &length, __ATOMIC_SEQ_CST);
    return length;
}
