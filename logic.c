#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "pos.h"
#include "board.h"

enum turn {
    BLACKS_TURN,
    WHITES_TURN
};

typedef enum turn turn;


enum outcome {
    IN_PROGRESS,
    BLACK_WIN,
    WHITE_WIN,
    DRAW
};

typedef enum outcome outcome;


enum rotation {
    NO_ROTATION,
    CLOCKWISE,
    COUNTERCLOCKWISE
};

typedef enum rotation rotation;


struct game {
    unsigned int run;
    board* b;
    posqueue *black_queue, *white_queue;
    turn player;
    rotation last_rotation;
};

typedef struct game game;


game* new_game(unsigned int run, unsigned int width,
               unsigned int height, enum type type){
  if(run > width && run > height){
    fprintf(stderr, "Game is impossible to win");
    exit(1);
  }
  game* game_new = (game*)malloc(sizeof(game));
  game_new -> run = run;
  game_new -> b = board_new(width, height, type);
  game_new -> black_queue = posqueue_new();
  game_new -> white_queue = posqueue_new();
  game_new -> player = BLACKS_TURN;
  game_new -> last_rotation = NO_ROTATION;
  return game_new;
}

void game_free(game* g){
  posqueue_free(g -> black_queue);
  posqueue_free(g -> white_queue);
  board_free(g -> b);
  free(g);
}

bool place_piece(game* g, pos p){
  //Returns false if position is occupied
  cell cell_status = board_get(g -> b, p);
  if(cell_status != EMPTY){
    return false;
  }
  //Place piece
  if(g -> player == BLACKS_TURN){
    board_set(g -> b, p, BLACK);
  }else{
    board_set(g -> b, p, WHITE);
  }
  //Update Queue
  if(g -> player == BLACKS_TURN){
    pos_enqueue(g -> black_queue, p);
  }else{
    pos_enqueue(g -> white_queue, p);
  }
  //Update Rotation
  g -> last_rotation = NO_ROTATION;
  //Update Turn
  if(g -> player == BLACKS_TURN){
    g -> player = WHITES_TURN;
  }else{
    g -> player = BLACKS_TURN;
  }
  return true;
}

struct rotation_args{
	game* g;
	bool clockwise;
	int row;
	board *new_board;
};

/*HELPER: Function executed by each thread to create a rotatated row 
  when rotating using matrix representation to ensure a faster rotation 
  matix representation. */	
void *thread(void *args){
	struct rotation_args *r  = (struct rotation_args*)args;
	int row = r -> row;
	board* old_board = r -> g -> b;
	board* rotated_b = r -> new_board;
	bool clockwise = r -> clockwise; //true if clockwise false otherwise
	unsigned int num_rows = r -> g -> b -> height;
  unsigned int num_cols = r -> g -> b -> width;
	
  for(int col = 0; col < num_rows; col++){
		pos pos_new, old_pos;		
		if(clockwise){
			pos_new = make_pos(col, num_rows - row - 1);
			old_pos = make_pos(row, col);
		}else{
			pos_new = make_pos(num_cols - col - 1, row);
			old_pos = make_pos(row, col);
		}
		cell c = board_get(old_board, old_pos);
		board_set(rotated_b, pos_new, c);
  }
  return NULL;
}

bool rotate(game* g, bool clockwise){
  if((g -> last_rotation == CLOCKWISE && !clockwise) || (g -> last_rotation == 
        COUNTERCLOCKWISE && clockwise)){
    return false;
  }
  board* rotated_b = board_new(g -> b -> height, g -> b -> width, g -> b -> type);
  posqueue* new_black_queue = posqueue_new();
  posqueue* new_white_queue = posqueue_new();
  //BIT REPRESENTATION Clockwise Rotation
  if(clockwise && g -> b -> type == BITS){
    while(g -> black_queue -> head != NULL){
      pos old_pos = pos_dequeue(g -> black_queue);
      pos pos_new = make_pos(old_pos.c, (g -> b -> height - old_pos.r - 1));
      board_set(rotated_b, pos_new, BLACK);
      pos_enqueue(new_black_queue, pos_new);
    }
    while(g -> white_queue -> head != NULL){
      pos old_pos = pos_dequeue(g -> white_queue);
      pos pos_new = make_pos(old_pos.c, (g -> b -> height - old_pos.r - 1));
      board_set(rotated_b, pos_new, WHITE);
      pos_enqueue(new_white_queue, pos_new);
    }
  }
  //BIT REPRESENTATION Counterclockwise Rotation
  if(!clockwise && g -> b -> type == BITS){
    while(g -> black_queue -> head != NULL){
      pos old_pos = pos_dequeue(g -> black_queue);
      pos pos_new = make_pos((g -> b -> width - old_pos.c - 1), old_pos.r);
      board_set(rotated_b, pos_new, BLACK);
      pos_enqueue(new_black_queue, pos_new);
    }
    while(g -> white_queue -> head != NULL){
      pos old_pos = pos_dequeue(g -> white_queue);
      pos pos_new = make_pos((g -> b -> width - old_pos.c - 1), old_pos.r);
      board_set(rotated_b, pos_new, WHITE);
      pos_enqueue(new_white_queue, pos_new);
    }
  }
  if(g -> b -> type == MATRIX){
    //Rotate Board
    unsigned int num_rows = g -> b -> height;
    pthread_t threads[num_rows];
    struct rotation_args *r[num_rows]; //array to store r pointers
    for(int row = 0; row < num_rows; row++){
      r[row] = (struct rotation_args*)malloc(sizeof(struct rotation_args));
      r[row] -> g = g;
      r[row] -> clockwise = clockwise;
      r[row] -> new_board = rotated_b;
      r[row] -> row = row;
      pthread_create(&threads[row], NULL, thread, r[row]);
    }
    for(int i = 0; i < num_rows; i++){
      pthread_join(threads[i], NULL);
      free(r[i]);
    }
    //Update Queues
    if(clockwise){
      while(g -> black_queue -> head != NULL){
        pos old_pos = pos_dequeue(g -> black_queue);
        pos pos_new = make_pos(old_pos.c, (g -> b -> height - old_pos.r - 1));
        pos_enqueue(new_black_queue, pos_new);
      }
      while(g -> white_queue -> head != NULL){
        pos old_pos = pos_dequeue(g -> white_queue);
        pos pos_new = make_pos(old_pos.c, (g -> b -> height - old_pos.r - 1));
        pos_enqueue(new_white_queue, pos_new);
      }
    }else{
      while(g -> black_queue -> head != NULL){
        pos old_pos = pos_dequeue(g -> black_queue);
        pos pos_new = make_pos((g -> b -> width - old_pos.c - 1), old_pos.r);
        pos_enqueue(new_black_queue, pos_new);
      }
      while(g -> white_queue -> head != NULL){
        pos old_pos = pos_dequeue(g -> white_queue);
        pos pos_new = make_pos((g -> b -> width - old_pos.c - 1), old_pos.r);
        pos_enqueue(new_white_queue, pos_new);
      }
    }
  }
  //Update game board and queues
  posqueue* temp_black = g -> black_queue;
  posqueue* temp_white = g -> white_queue;
  board* temp_board = g -> b;
  g -> black_queue = new_black_queue;
  g -> white_queue = new_white_queue;
  g -> b = rotated_b;
  posqueue_free(temp_black);
  posqueue_free(temp_white);
  board_free(temp_board);

  //Update rotation status and turn
  if(clockwise){
    g -> last_rotation = CLOCKWISE;
  }else{
    g -> last_rotation = COUNTERCLOCKWISE;
  }
  if(g -> player == BLACKS_TURN){
    g -> player = WHITES_TURN;
  }else{
    g -> player = BLACKS_TURN;
  }
  return true;
}

bool uplift(game* g, cell c){
  posqueue* queue;
  //Select Queue, if queue selected is empty returns false
  if(c == BLACK && g -> black_queue -> len != 0){
    queue = g -> black_queue;
  }else if(c == WHITE && g -> white_queue -> len != 0){
    queue = g -> white_queue;
  }else{
    return false;
  }

  pos uplift_piece_pos = pos_dequeue(queue);
  unsigned int col = uplift_piece_pos.c;
  
  pos cur_pos;
  unsigned int row = 0;
  //If row is 0 cannnot uplift so keep at 0, if row not 0, sets the pos to check to the row above
  if(uplift_piece_pos.r == 0){
    cur_pos = make_pos(row, col);
  }else{
    row = uplift_piece_pos.r - 1;
    cur_pos = make_pos(row, col);
  }
  //Keeps uplifting until it hits the top or another piece
  while(cur_pos.r != 0 && board_get(g -> b, cur_pos) == EMPTY){
    row -= 1;
    cur_pos.r = row;
  }
  pos_enqueue(queue, cur_pos);
  if(board_get(g -> b, cur_pos) != EMPTY){
    cur_pos.r += 1;
  }
      
  if(uplift_piece_pos.r != 0){
    board_set(g -> b, cur_pos, board_get(g -> b, uplift_piece_pos));
    board_set(g -> b, uplift_piece_pos, EMPTY);
  }
  
  //Updates Turn and Rotation status
  if(g -> player == BLACKS_TURN){
    g -> player = WHITES_TURN;
  }else{
    g -> player = BLACKS_TURN;
  }
  g -> last_rotation = NO_ROTATION;
  return true;
}

enum direction {
    S, E, SE, SW
};
typedef enum direction direction;
/* HELPER searches one direction for a given cell, returns the count 
   of how many of that cell are in a row in that direction */
int dir_search(direction dir, game* g, pos p, cell c){
  pos checking = p;
  if(dir == S && p.r < g -> b -> height - 1){ //(r + 1, c)
    checking = make_pos(p.r + 1, p.c);
  }
  if(dir == E && p.c < g -> b -> width - 1){ //(r, c + 1)
    checking = make_pos(p.r, p.c + 1);
  }
  if(dir == SE && p.r < g -> b -> height - 1 && p.c < g -> b -> width - 1){ //(r + 1, c + 1)
    checking = make_pos(p.r + 1, p.c + 1);
  }
  if(dir == SW && p.r < g -> b -> height - 1 && p.c > 0){ //(r + 1, c - 1)
    checking = make_pos(p.r + 1, p.c - 1);
  }
  int count = 0;
  if((checking.r == p.r && checking.c == p.c) || board_get(g -> b, checking) != c){
    return count;
  }else{
    count += 1;
    return count + dir_search(dir, g, checking, c);
  }
}


outcome game_outcome(game* g){
  bool black_win = false;
  bool white_win = false;
  //Checking Black's queue:
  pq_entry* bq_head = g -> black_queue -> head;
  pq_entry* bq_current = bq_head;
  while (bq_current != NULL && !black_win){
    pos p_b = bq_current -> p;
    //Adds 1 to count returned by dir_search to account for cell on currently
    if( dir_search(S, g, p_b, BLACK) + 1 >= g -> run || 
        dir_search(E, g, p_b, BLACK) + 1 >= g -> run || 
        dir_search(SE, g, p_b, BLACK) + 1 >= g -> run || 
        dir_search(SW, g, p_b, BLACK) + 1 >= g -> run){
      black_win = true;
    }
    bq_current = bq_current -> next;
  }

  pq_entry* wq_head = g -> white_queue -> head;
  pq_entry* wq_current = wq_head;
  while(wq_current != NULL && !white_win){
    pos p_w = wq_current -> p;
    if(dir_search(S, g, p_w, WHITE) + 1 >= g -> run ||
        dir_search(E, g, p_w, WHITE) + 1 >= g -> run ||
        dir_search(SE, g, p_w, WHITE) + 1 >= g -> run ||
        dir_search(SW, g, p_w, WHITE) + 1 >= g -> run){
      white_win = true;
    }
    wq_current = wq_current -> next;
  }
  
  if(black_win){
    return BLACK_WIN;
  }else if(white_win){
    return WHITE_WIN;
  }else if((g -> black_queue -> len + g -> white_queue -> len) == 
      (g -> b -> width * g -> b -> height)){
    return DRAW;
  }else{
    return IN_PROGRESS;
  }
}



