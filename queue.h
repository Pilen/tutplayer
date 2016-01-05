#ifndef TUT_OUTPUT_H
#define TUT_OUTPUT_H

typedef struct Queue_s Queue;
typedef struct Queue_node_s Queue_node;

/* void queue_init(void); */
Queue *queue_create(void);
void queue_add(Queue *queue, void *data);
void *queue_read(Queue *queue);
unsigned int queue_cleanup(Queue *queue);
unsigned int queue_approximate_length(Queue *queue); /* Approximate */

#endif
