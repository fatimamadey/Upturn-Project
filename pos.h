#ifndef POS_H
#define POS_H

/* Represents a location on the board game, using zero-based indexing */
struct pos {
    unsigned int r, c;
};

typedef struct pos pos;

typedef struct pq_entry pq_entry;

/* Has each invidual entry in the list, holds position and a next pointer */
struct pq_entry {
    pos p;
    pq_entry* next;
};

/* Represents a list that keeps track of the pointers to the first 
   and last element of the linked list of postitions in the order of 
   piece placement.
   USED FOR ANY OPERATIONS ON QUEUE
*/
struct posqueue {
    pq_entry *head, *tail;
    unsigned int len;
};

typedef struct posqueue posqueue;

/* Creates a pos value */
pos make_pos(unsigned int r, unsigned int c);

/* Allocate a new empty queue */
posqueue* posqueue_new();

/* Add a postion to the queue at the back */
void pos_enqueue(posqueue* q, pos p);

/* Removes and returns the element at the front of the queue. 
   Raises error if queue is empty. */
pos pos_dequeue(posqueue* q);

/* Entirely deallocate an existing queue */
void posqueue_free(posqueue* q);

#endif /* POS_H */
