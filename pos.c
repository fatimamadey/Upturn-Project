#include <stdio.h>
#include <stdlib.h>

struct pos {
    unsigned int r, c;
};

typedef struct pos pos;

typedef struct pq_entry pq_entry;

struct pq_entry {
    pos p;
    pq_entry* next;
};

struct posqueue {
    pq_entry *head, *tail;
    unsigned int len;
};

typedef struct posqueue posqueue;

pos make_pos(unsigned int r, unsigned int c){
  pos position = {r, c};
  return position;
}

posqueue* posqueue_new(){
  posqueue* position_queue = (posqueue*)malloc(sizeof(posqueue));
  if(position_queue == NULL){
    fprintf(stderr, "position_queue: Allocation failed\n");
    exit(1);
  }
  position_queue -> head = NULL;
  position_queue -> tail = NULL;
  position_queue -> len = 0;
  
  return position_queue;
}

void pos_enqueue(posqueue* q, pos p){
  pq_entry* new_entry = (pq_entry*)malloc(sizeof(pq_entry));
  if (new_entry == NULL){
    fprintf(stderr, "new_entry: Allocation failed\n");
    exit(1);
    }
    new_entry -> p = p;
    new_entry -> next = NULL;
  if(q -> tail == NULL){ //empty queue case
    q -> head = new_entry;
    q -> tail = new_entry;
  }else{
    q -> tail -> next = new_entry;
    q -> tail = new_entry;
  }
  q -> len += 1;
}

pos pos_dequeue(posqueue* q){
  pos position = q -> head -> p;
  pq_entry* temp = q -> head; //save to free later
  q -> head = q -> head -> next;
  if (q -> head == NULL){ //when dequeueing forms an empty queue so update tail to reflect that
    q -> tail = NULL;
  }
  free(temp);
  q -> len--;
  return position;
}

void posqueue_free(posqueue* q){
  pq_entry* q_head = q -> head;
  pq_entry* next;
  while (q_head != NULL){
    next = q_head -> next;
    free(q_head);
    q_head = next;
  }
  free(q);
}
