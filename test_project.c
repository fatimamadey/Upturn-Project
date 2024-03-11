#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include "pos.h"
#include "board.h"
#include "logic.h"

/********** TESTS FOR POS ***********/ 
//HELPER FUNTION to make a pq_entry to make it easier to build posqueues.
pq_entry* make_pq_entry(pos p, pq_entry* next){
  pq_entry* entry = (pq_entry*)malloc(sizeof(pq_entry));
  if (entry == NULL){
    fprintf(stderr, "entry: Allocation Failed\n");
    exit(1);
  }
  entry -> p = p;
  entry -> next = next;
  return entry;
}
//HELPER FUNCTION to make a posqueue to lessen repeated code
posqueue* make_posqueue(pq_entry* head, pq_entry* tail, unsigned int len){
  posqueue* queue = (posqueue*)malloc(sizeof(posqueue));
  if (queue == NULL){
    fprintf(stderr, "queue: Allocation Failed\n");
    exit(1);
  }
  queue -> head = head;
  queue -> tail = tail;
  queue -> len = len;
  return queue;
}
//HELPER FUNCTION checks if 2 pq_entry are equal
int pq_entry_eq(pq_entry* p1, pq_entry* p2){
  if (p1 == NULL || p2 == NULL){
    return 1;
  }else if (p1 -> p.r != p2 -> p.r && p1 -> p.c != p2 -> p.c){
    return 0;
  }else{
    return pq_entry_eq(p1 -> next, p2 -> next);
  }
}
//HELPER FUNCTION checks if 2 posqueues are equal
int posqueue_eq(posqueue* q1, posqueue* q2){
   if (pq_entry_eq(q1 -> head, q2 -> head) && pq_entry_eq(q1 -> tail, q2 -> tail) 
       && q1 -> len == q2 -> len){
     return 1;
   }else{
     return 0;
   }
}

//TEST for posqueue_free
Test(posqueue_free, crash_test){
  pos p0 = make_pos(0, 0), p1 = make_pos(1, 1), p2 = make_pos(2, 2), 
      p3 = make_pos(3, 3);
  pq_entry* head = make_pq_entry(p0, make_pq_entry(p1, make_pq_entry(p2,
          make_pq_entry(p3, NULL))));
  pq_entry* tail = make_pq_entry(p3, NULL);
  unsigned int len = 4;
  posqueue* queue = make_posqueue(head, tail, len);
  posqueue_free(queue);
  cr_assert(1);
}

//TEST for make_pos
Test(make_pos, test1){
  pos new_pos = make_pos(2, 3);
  pos expected = {2, 3};
  if (new_pos.r == expected.r && new_pos.c == expected.c){
    cr_assert(1);
  }else{
    cr_assert(0);
  }
}

//TEST for posqueue_new
Test(posqueue_new, test1){
  posqueue* new_queue = posqueue_new();
  posqueue* expected = make_posqueue(NULL, NULL, 0);
  if (posqueue_eq(new_queue, expected)){ 
    posqueue_free(expected);
    posqueue_free(new_queue);
    cr_assert(1);
  }else{
    posqueue_free(expected);
    posqueue_free(new_queue);
    cr_assert(0);
  }
}

//TESTS for pos_enqueue
Test(pos_enqueue, test0){
  posqueue* new_queue = posqueue_new();
  pos new_pos = make_pos(2, 2);
  pos_enqueue(new_queue, new_pos);

  pq_entry* expected_head = make_pq_entry(new_pos, NULL);
  pq_entry* expected_tail = make_pq_entry(new_pos, NULL);
  unsigned int expected_len = 1;
  posqueue* expected = make_posqueue(expected_head, expected_tail, expected_len);
  if(posqueue_eq(new_queue, expected)){
    posqueue_free(expected);
    cr_assert(1);
  }else{
    posqueue_free(expected);
    cr_assert(0);
  }
}

Test(pos_enqueue, test2){
  pos p0 = make_pos(4, 2), p1 = make_pos(1, 3), p2 = make_pos(2, 1), 
      p3 = make_pos(4, 3), p4 = make_pos(3, 2);
  pq_entry* head = make_pq_entry(p4, make_pq_entry(p3, make_pq_entry(p2,
          make_pq_entry(p1, make_pq_entry(p0, NULL)))));
  pq_entry* tail = make_pq_entry(p0, NULL);
  unsigned int len = 5;
  posqueue* new_queue = make_posqueue(head, tail, len);
  pos new_pos = make_pos(2, 6);
  pos_enqueue(new_queue, new_pos);

  pq_entry* expected_head = make_pq_entry(p4, make_pq_entry(p3, make_pq_entry(p2,
        make_pq_entry(p1, make_pq_entry(p0, make_pq_entry(new_pos, NULL))))));
  pq_entry* expected_tail = make_pq_entry(new_pos, NULL);
  unsigned int expected_len = 6;
  posqueue* expected = make_posqueue(expected_head, expected_tail, expected_len);
  if (posqueue_eq(new_queue, expected)){
    posqueue_free(new_queue);
    posqueue_free(expected);
    cr_assert(1);
  }else{
    posqueue_free(new_queue);
    posqueue_free(expected);
    cr_assert(0);
  }
}

//TESTS for pos_dequeue
Test(pos_dequeue, test0){
  pos p0 = make_pos(0, 0);

  pq_entry* head = make_pq_entry(p0, NULL);
  pq_entry* tail = make_pq_entry(p0, NULL);
  unsigned int len = 1;
  posqueue* new_queue = make_posqueue(head, tail, len);
  pos_dequeue(new_queue);
  
  posqueue* expected = posqueue_new();

  if (posqueue_eq(new_queue, expected)){
    posqueue_free(new_queue);
    posqueue_free(expected);
    cr_assert(1);
  }else{
    posqueue_free(new_queue);
    posqueue_free(expected);
    cr_assert(0);
  }
}

Test(pos_dequeue, test1){
  pos p0 = make_pos(0, 0), p1 = make_pos(1, 1), p2 = make_pos(3, 3), p3 = make_pos(3, 1), p4 = make_pos(3, 2);

  pq_entry* head = make_pq_entry(p4, make_pq_entry(p3, make_pq_entry(p2,
          make_pq_entry(p1, make_pq_entry(p0, NULL)))));
  pq_entry* tail = make_pq_entry(p0, NULL);
  unsigned int len = 5;
  posqueue* new_queue = make_posqueue(head, tail, len);

  pos_dequeue(new_queue);

  pq_entry* expected_head = make_pq_entry(p3, make_pq_entry(p2, make_pq_entry(p1, 
          make_pq_entry(p0, NULL))));
  pq_entry* expected_tail = make_pq_entry(p0, NULL);
  unsigned int expected_len = 4;
  posqueue* expected = make_posqueue(expected_head, expected_tail, expected_len);

  if (posqueue_eq(new_queue, expected)){
    posqueue_free(new_queue);
    posqueue_free(expected);
    cr_assert(1);
  }else{
    posqueue_free(new_queue);
    posqueue_free(expected);
    cr_assert(0);
  }
}

/************** TESTS FOR BOARD ****************/
//HELPER for board equivalence, evaluates if 2 boards are equivalent
int board_eq(board* b1, board* b2){
  if(b1 -> type != b2 -> type){
    printf("board_eq: Board types are not the same");
    return 0;
  }
  unsigned int b1_rows = b1 -> height, b2_rows = b2 -> height;
  unsigned int b1_cols = b1 -> width, b2_cols = b2 -> width;
  if (b1_rows != b2_rows || b1_cols != b2_cols || b1 -> type != b2 -> type){
    printf("board_eq: Boards are not the same size");
    return 0;
  }
  if(b1 -> type == MATRIX){
    cell** m1 = b1 -> u.matrix;
    cell** m2 = b2 -> u.matrix;
    for (unsigned int i = 0; i < b1_rows; i++){
      for (unsigned int j = 0; j < b1_cols; j++){
        if (m1[i][j] != m2[i][j]){
          return 0;
        }
      }
    }
    return 1;
  }else{
    unsigned int arr_len = ((b1_cols * b1_rows * 2) + 31) / 32;
    unsigned int* bit_arr1 = b1 -> u.bits;
    unsigned int* bit_arr2 = b2 -> u.bits;
    for(int i = 0; i < arr_len; i++){
      if(*bit_arr1 != *bit_arr2){
        return 0;
      }
      bit_arr1++;
      bit_arr2++;
    }
    return 1;
  }
}
//HELPER makes a matrix to lessen repeted code
cell** make_matrix(unsigned int width, unsigned int height){
  cell** new_matrix = (cell**)malloc(sizeof(cell*) * height);
  if(new_matrix == NULL){
    fprintf(stderr, "new_matrix: Allocation Failed\n");
    exit(1);
  }
  for(unsigned int i = 0; i < height; i++){
    new_matrix[i] = (cell*)malloc(sizeof(cell) * width);
    if(new_matrix[i] == NULL){
      fprintf(stderr, "new matrix column: Allocation Failed\n");
      exit(1);
    }
    for(unsigned int j = 0; j < width; j++){
      new_matrix[i][j] = EMPTY;
    }
  }
  return new_matrix;
}

//TEST for board_new
Test(board_new, matrix1){
  board* board_a = board_new(3, 4, MATRIX);
  board* expected = (board*)malloc(sizeof(board));
  if(expected == NULL){
    fprintf(stderr, "expected: Allocation failed\n");
    exit(1);
  }
  expected -> width = 3;
  expected -> height = 4;
  expected -> type = MATRIX;
  expected -> u.matrix = make_matrix(3, 4);
  if (board_eq(board_a, expected)){
    board_free(board_a);
    board_free(expected);
    cr_assert(1);
  }else{
    board_free(board_a);
    board_free(expected);
    cr_assert(0);
  }
}
Test(board_new, bits1){
  board* board_a = board_new(3, 6, BITS);
  board* expected = (board*)malloc(sizeof(board));
  if(expected == NULL){
    fprintf(stderr, "expected: Allocation failed\n");
    exit(1);
  }
  expected -> width = 3;
  expected -> height = 6;
  expected -> type = BITS;
  int num_elements = 2; 
  unsigned int* bits_arr = (unsigned int*)malloc(sizeof(unsigned int) * num_elements); 
  for(int i = 0; i < num_elements; i++){
    bits_arr[i] = 0;
  }
  expected -> u.bits = bits_arr;
  if (board_eq(board_a, expected)){
    board_free(board_a);
    board_free(expected);
    cr_assert(1);
  }else{
    board_free(board_a);
    board_free(expected);
    cr_assert(0);
  }
}
//TEST for board_free
Test(board_free, crash_test){
  board* test = (board*)malloc(sizeof(board));
  if(test == NULL){
    fprintf(stderr, "expected: Allocation failed\n");
    exit(1);
  }
  test -> width = 5;
  test -> height = 7;
  test -> type = MATRIX;
  test -> u.matrix = make_matrix(5, 7);
  board_free(test);
  board* test_bit = board_new(5, 4, BITS);
  board_free(test_bit);
  cr_assert(1);
}

//TESTS for board_show done visually, left commented out
Test(board_show, numbers){
  board* b = board_new(10, 10, MATRIX);
  board* c = board_new(10, 10, BITS);
  //board_show(c);
  board_free(b);
  board_free(c);
}
Test(board_show, capital_letters){
  board* b = board_new(36, 36, MATRIX);
  board* c = board_new(36, 36, BITS);
  //board_show(c);
  board_free(b);
  board_free(c);
}
Test(board_show, lowercase_letters){
  board* b = board_new(62, 62, MATRIX);
  board* c = board_new(62, 62, BITS);
  //board_show(c);
  board_free(b);
  board_free(c);
}
Test(board_show, question_marks){
  board* b = board_new(73, 73, MATRIX);
  board* c = board_new(73, 73, BITS);
  //board_show(c);
  board_free(b);
  board_free(c);
}

//TESTS for board_get
Test(board_get, test1){
  board* b1 = board_new(3, 5, MATRIX);
  b1 -> u.matrix[1][0] = BLACK;
  b1 -> u.matrix[0][1] = WHITE;
  b1 -> u.matrix[2][0] = BLACK;
  b1 -> u.matrix[1][3] = WHITE;
  pos p0 = make_pos(2, 0);
  cell ans = board_get(b1, p0);
  if(ans == BLACK){
    board_free(b1);
    cr_assert(1);
  }else{
    board_free(b1);
    cr_assert(0);
  }
}

Test(board_get, edge_case){
  board* b1 = board_new(2, 2, MATRIX);
  b1 -> u.matrix[0][1] = BLACK;
  b1 -> u.matrix[1][0] = WHITE;
  b1 -> u.matrix[0][0] = BLACK;
  b1 -> u.matrix[1][1] = WHITE;
  pos p0 = make_pos(0, 1), p1 = make_pos(1, 0), p2 = make_pos(0, 0), 
      p3 = make_pos(1, 1);
  cell ans0 = board_get(b1, p0);
  cell ans1 = board_get(b1, p1);
  cell ans2 = board_get(b1, p2);
  cell ans3 = board_get(b1, p3);
  if(ans0 == BLACK && ans1 == WHITE && ans2 == BLACK && ans3 == WHITE){
    board_free(b1);
    cr_assert(1);
  }else{
    board_free(b1);
    cr_assert(0);
  }
}
Test(board_get, test_bit1){
  board *b = board_new(4, 3, BITS);
  pos p = make_pos(2, 3);
  cell res = board_get(b, p);
  if(res == EMPTY){
    board_free(b);
    cr_assert(1);
  }else{
    board_free(b);
    cr_assert(0);
  }
}

Test(board_get, edge_case_bits){
  board *b = board_new(2, 2, BITS);
  pos p0 = make_pos(0, 0), p1 = make_pos(0, 1), p2 = make_pos(1, 0), p3 = make_pos(1, 1);
  board_set(b, p0, BLACK); //NOTE: tested board_set to ensure it works before using here
  board_set(b, p1, WHITE);
  board_set(b, p2, BLACK);
  board_set(b, p3, WHITE);
  cell res0 = board_get(b, p0);
  cell res1 = board_get(b, p1);
  cell res2 = board_get(b, p2);
  cell res3 = board_get(b, p3);
  if(res0 == BLACK && res1 == WHITE && res2 == BLACK && res3 == WHITE){
    board_free(b);
    cr_assert(1);
  }else{
    board_free(b);
    cr_assert(0);
  }
}

//TESTS for board_set
Test(board_set, test1){
  board* b1 = board_new(4, 3, MATRIX);
  pos p1 = make_pos(1, 2);
  board_set(b1, p1, BLACK);
  board* expected = board_new(4, 3, MATRIX);
  expected -> u.matrix[1][2] = BLACK;
  if (board_eq(b1, expected)){
    board_free(b1);
    board_free(expected);
    cr_assert(1);
  }else{
    board_free(b1);
    board_free(expected);
    cr_assert(0);
  }
}

Test(board_set, test2){
  board* b1 = board_new(7, 8, MATRIX);
  pos p1 = make_pos(1, 2), p2 = make_pos(6, 2), p3 = make_pos(0, 0), 
      p4 = make_pos(3, 4), p5 = make_pos(4, 6);
  board_set(b1, p1, BLACK);
  board_set(b1, p2, WHITE);
  board_set(b1, p3, WHITE);
  board_set(b1, p4, BLACK);
  board_set(b1, p5, BLACK);
  board* expected = board_new(7, 8, MATRIX);
  expected -> u.matrix[1][2] = BLACK;
  expected -> u.matrix[6][2] = WHITE;
  expected -> u.matrix[0][0] = WHITE;
  expected -> u.matrix[3][4] = BLACK;
  expected -> u.matrix[4][6] = BLACK;
  if (board_eq(b1, expected)){
    board_free(b1);
    board_free(expected);
    cr_assert(1);
  }else{
    board_free(b1);
    board_free(expected);
    cr_assert(0);
  }
}

Test(board_set, edge_case){
  board* b1 = board_new(2, 2, MATRIX);
  pos p1 = make_pos(1, 0), p2 = make_pos(0, 1), p3 = make_pos(0, 0), 
      p4 = make_pos(1, 1);
  board_set(b1, p1, BLACK);
  board_set(b1, p2, WHITE);
  board_set(b1, p3, WHITE);
  board_set(b1, p4, BLACK);
  board* expected = board_new(2, 2, MATRIX);
  expected -> u.matrix[1][0] = BLACK;
  expected -> u.matrix[0][1] = WHITE;
  expected -> u.matrix[0][0] = WHITE;
  expected -> u.matrix[1][1] = BLACK;
  if (board_eq(b1, expected)){
    board_free(b1);
    board_free(expected);
    cr_assert(1);
  }else{
    board_free(b1);
    board_free(expected);
    cr_assert(0);
  }
}
Test(board_set, test1_bit){
  board* b1 = board_new(2, 2, BITS);
  pos p0 = make_pos(0, 0), p1 = make_pos(1, 1);
  board_set(b1, p0, BLACK);
  board_set(b1, p1, WHITE);
  board* expected = board_new(2, 2, BITS);
  unsigned int* bit_arr = expected -> u.bits;
  int mask = 1;
  int mask2 = 2 << (4 + 2);
  bit_arr[0] = bit_arr[0] | mask;
  bit_arr[0] = bit_arr[0] | mask2;
  expected -> u.bits[0] = bit_arr[0];
  if (board_eq(b1, expected)){
    board_free(b1);
    board_free(expected);
    cr_assert(1);
  }else{
    board_free(b1);
    board_free(expected);
    cr_assert(0);
  }
}
Test(board_set, test2_bit){
  board* b1 = board_new(5, 3, BITS); //(width, height)
  pos p0 = make_pos(1, 4), p1 = make_pos(2, 3); //(row, col)
  board_set(b1, p0, BLACK);
  board_set(b1, p1, WHITE);
  board* expected = board_new(5, 3, BITS);
  unsigned int* bit_arr = expected -> u.bits;
  int mask = 1 << 18;
  int mask2 = 2 << 26;
  bit_arr[0] = bit_arr[0] | mask;
  bit_arr[0] = bit_arr[0] | mask2;
  expected -> u.bits[0] = bit_arr[0];
  if (board_eq(b1, expected)){
    board_free(b1);
    board_free(expected);
    cr_assert(1);
  }else{
    board_free(b1);
    board_free(expected);
    cr_assert(0);
  }
}



/**************** TESTS FOR LOGIC ****************/
//HELPER checks if 2 games are equal
unsigned int game_eq(game* g1, game* g2){
  unsigned int equal = 1;
  if(g1 -> run != g2 -> run){
    equal = 0;
  }
  if(!board_eq(g1 -> b, g2 -> b)){
    printf("boards are not equal\n");
    //board_show(g1 -> b);
    //board_show(g2 -> b);
    equal = 0;
  }
  if(!posqueue_eq(g1 -> black_queue, g2 -> black_queue) || 
      !posqueue_eq(g1 -> white_queue, g2 -> white_queue)){
    printf("posqueues are not equal\n");
    equal = 0;
  }
  if(g1 -> player != g2 -> player){
    equal = 0;
  }
  if(g1 -> last_rotation != g2 -> last_rotation){
    equal = 0;
  }
  return equal;
}

//TEST for game_free and new_game to ensure they do not crash
Test(game_new_game_free, crash_test){
  game* g_new = new_game(5, 8, 9, MATRIX);
  game_free(g_new);
  cr_assert(1);
}

//TESTS for place_piece
Test(place_piece, edge_case){
  game* expected = new_game(2, 2, 2, MATRIX);

  pos p0 = make_pos(0, 0);
  board_set(expected -> b, p0, BLACK);
  pos_enqueue(expected -> black_queue, p0);
  expected -> player = WHITES_TURN;
  expected -> last_rotation = NO_ROTATION;

  pos p1 = make_pos(1, 1);
  board_set(expected -> b, p1, WHITE);
  pos_enqueue(expected -> white_queue, p1);
  expected -> player = BLACKS_TURN;
  expected -> last_rotation = NO_ROTATION;

  pos p2 = make_pos(0, 1);
  board_set(expected -> b, p2, BLACK);
  pos_enqueue(expected -> black_queue, p2);
  expected -> player = WHITES_TURN;
  expected -> last_rotation = NO_ROTATION;

  pos p3 = make_pos(1, 0);
  board_set(expected -> b, p3, WHITE);
  pos_enqueue(expected -> white_queue, p3);
  expected -> player = BLACKS_TURN;
  expected -> last_rotation = NO_ROTATION;

  game* g_new = new_game(2, 2, 2, MATRIX);
  bool ans0 = place_piece(g_new, p0);
  bool ans1 = place_piece(g_new, p1);
  bool ans2 = place_piece(g_new, p2);
  bool ans3 = place_piece(g_new, p3);

  if(ans0 && ans1 && ans2 && ans3 && game_eq(g_new, expected)){
    game_free(g_new);
    game_free(expected);
    cr_assert(1);
  }else{
    game_free(g_new);
    game_free(expected);
    cr_assert(0);
  }
}

Test(place_piece, test1){
  game* expected = new_game(6, 7, 6, MATRIX);
  
  pos p0 = make_pos(3, 4);
  board_set(expected -> b, p0, BLACK);
  pos_enqueue(expected -> black_queue, p0);
  expected -> player = WHITES_TURN;
  expected -> last_rotation = NO_ROTATION;

  pos p1 = make_pos(5, 3);
  board_set(expected -> b, p1, WHITE);
  pos_enqueue(expected -> white_queue, p1);
  expected -> player = BLACKS_TURN;
  expected -> last_rotation = NO_ROTATION;
  
  pos p2 = make_pos(2, 1);
  board_set(expected -> b, p2, BLACK);
  pos_enqueue(expected -> black_queue, p2);
  expected -> player = WHITES_TURN;
  expected -> last_rotation = NO_ROTATION;
  
  pos p3 = make_pos(1, 1);
  board_set(expected -> b, p3, WHITE);
  pos_enqueue(expected -> white_queue, p3);
  expected -> player = BLACKS_TURN;
  expected -> last_rotation = NO_ROTATION;
  
  game* g_new = new_game(6, 7, 6, MATRIX);
  bool ans0 = place_piece(g_new, p0);
  bool ans1 = place_piece(g_new, p1);
  bool ans2 = place_piece(g_new, p2);
  bool ans3 = place_piece(g_new, p3);

  if(ans0 && ans1 && ans2 && ans3 && game_eq(g_new, expected)){
    game_free(g_new);
    game_free(expected);
    cr_assert(1);
  }else{
    game_free(g_new);
    game_free(expected);
    cr_assert(0);
  }
}

Test(place_piece, occupied_place){
  game* expected = new_game(3, 5, 4, MATRIX);

  pos p0 = make_pos(1, 2);
  board_set(expected -> b, p0, BLACK);
  pos_enqueue(expected -> black_queue, p0);
  expected -> player = WHITES_TURN;
  expected -> last_rotation = NO_ROTATION;

  pos p1 = make_pos(2, 2);
  board_set(expected -> b, p1, WHITE);
  pos_enqueue(expected -> white_queue, p1);
  expected -> player = BLACKS_TURN;
  expected -> last_rotation = NO_ROTATION;

  pos p2 = make_pos(3, 1);
  board_set(expected -> b, p2, BLACK);
  pos_enqueue(expected -> black_queue, p2);
  expected -> player = WHITES_TURN;
  expected -> last_rotation = NO_ROTATION;

  pos p3 = make_pos(1, 1);
  board_set(expected -> b, p3, WHITE);
  pos_enqueue(expected -> white_queue, p3);
  expected -> player = BLACKS_TURN;
  expected -> last_rotation = NO_ROTATION;

  game* g_new = new_game(3, 5, 4, MATRIX);
  bool ans0 = place_piece(g_new, p0);
  bool ans1 = place_piece(g_new, p1);
  bool ans2 = place_piece(g_new, p2);
  bool ans3 = place_piece(g_new, p3);
  bool ans4 = place_piece(g_new, p1);

  if(ans0 && ans1 && ans2 && ans3 && !ans4 && game_eq(g_new, expected)){
    game_free(g_new);
    game_free(expected);
    cr_assert(1);
  }else{
    game_free(g_new);
    game_free(expected);
    cr_assert(0);
  }
}

//TESTS for rotate
Test(rotate, clockwise){
  pos p0 = make_pos(1, 2), p1 = make_pos(0, 1), p2 = make_pos(0, 0), 
      p3 = make_pos(1, 1);
  game* g_new = new_game(3, 4, 3, MATRIX);
  place_piece(g_new, p0); //Starts w/Black
  place_piece(g_new, p1);
  place_piece(g_new, p2);
  place_piece(g_new, p3);
  bool success = rotate(g_new, true);
  game* expected = new_game(3, 3, 4, MATRIX);
  pos p0r = make_pos(2, 1), p1r = make_pos(1, 2), p2r = make_pos(0, 2), 
      p3r = make_pos(1, 1);
  place_piece(expected, p0r);
  place_piece(expected, p1r);
  place_piece(expected, p2r);
  place_piece(expected, p3r);
  expected -> last_rotation = CLOCKWISE;
  expected -> player = WHITES_TURN;
  if(success && game_eq(g_new, expected)){
    game_free(g_new);
    game_free(expected);
    cr_assert(1);
  }else{
    game_free(g_new);
    game_free(expected);
    cr_assert(0);
  }
}

Test(rotate, counterclockwise){
  pos p0 = make_pos(1, 2), p1 = make_pos(0, 1), p2 = make_pos(0, 0), 
      p3 = make_pos(1, 1);
  game* g_new = new_game(3, 4, 3, MATRIX);
  place_piece(g_new, p0); //Starts w/Black
  place_piece(g_new, p1);
  place_piece(g_new, p2);
  place_piece(g_new, p3);
  bool success = rotate(g_new, false);
  game* expected = new_game(3, 3, 4, MATRIX);
  pos p0r = make_pos(1, 1), p1r = make_pos(2, 0), p2r = make_pos(3, 0), 
      p3r = make_pos(2, 1);
  place_piece(expected, p0r);
  place_piece(expected, p1r);
  place_piece(expected, p2r);
  place_piece(expected, p3r);
  expected -> last_rotation = COUNTERCLOCKWISE;
  expected -> player = WHITES_TURN;

  if(success && game_eq(g_new, expected)){
    game_free(g_new);
    game_free(expected);
    cr_assert(1);
  }else{
    game_free(g_new);
    game_free(expected);
    cr_assert(0);
  }
}

Test(rotate, illegal_rotation){
  pos p0 = make_pos(1, 2), p1 = make_pos(0, 1), p2 = make_pos(0, 0), 
      p3 = make_pos(1, 1);
  game* g_new = new_game(3, 4, 3, MATRIX);
  place_piece(g_new, p0); //Starts w/Black
  place_piece(g_new, p1);
  place_piece(g_new, p2);
  place_piece(g_new, p3);
  g_new -> last_rotation = COUNTERCLOCKWISE;
  bool success = rotate(g_new, true);

  game* expected = new_game(3, 4, 3, MATRIX);
  pos p0r = make_pos(1, 2), p1r = make_pos(0, 1), p2r = make_pos(0, 0), 
      p3r = make_pos(1, 1);
  place_piece(expected, p0r);
  place_piece(expected, p1r);
  place_piece(expected, p2r);
  place_piece(expected, p3r);
  expected -> last_rotation = COUNTERCLOCKWISE;
  expected -> player = BLACKS_TURN;
  if(!success && game_eq(g_new, expected)){
    game_free(g_new);
    game_free(expected);
    cr_assert(1);
  }else{
    game_free(g_new);
    game_free(expected);
    cr_assert(0);
  }
}

//TESTS for uplift
Test(uplift, test1){
  game* g_new = new_game(3, 3, 4, MATRIX);
  pos p0 = make_pos(1, 2), p1 = make_pos(1, 1), p2 = make_pos(2, 0), 
      p3 = make_pos(1, 0);
  place_piece(g_new, p0);
  place_piece(g_new, p1);
  place_piece(g_new, p2);
  place_piece(g_new, p3);
  bool success = uplift(g_new, WHITE);
  
  game* expected = new_game(3, 3, 4, MATRIX);
  pos p0u = make_pos(1, 2), p1u = make_pos(0, 1), p2u = make_pos(2, 0), 
      p3u = make_pos(1, 0);
  place_piece(expected, p0u);
  expected -> player = BLACKS_TURN;
  place_piece(expected, p2u);
  place_piece(expected, p3u);
  expected -> player = WHITES_TURN;
  place_piece(expected, p1u);
  expected -> player = WHITES_TURN;
  
  if(success && game_eq(g_new, expected)){
    game_free(g_new);
    game_free(expected);
    cr_assert(1);
  }else{
    game_free(g_new);
    game_free(expected);
    cr_assert(0);
  }
}

Test(uplift, test2){
  game* g_new = new_game(3, 3, 4, MATRIX);
  pos p0 = make_pos(0, 2), p1 = make_pos(1, 1), p2 = make_pos(1, 2), 
      p3 = make_pos(1, 0);
  place_piece(g_new, p0);
  place_piece(g_new, p1);
  place_piece(g_new, p2);
  place_piece(g_new, p3);
  bool success = uplift(g_new, BLACK);

  game* expected = new_game(3, 3, 4, MATRIX);
  pos p0u = make_pos(0, 2), p1u = make_pos(1, 1), p2u = make_pos(1, 2), 
      p3u = make_pos(1, 0);
  expected -> player = WHITES_TURN;
  place_piece(expected, p1u);
  place_piece(expected, p2u);
  place_piece(expected, p3u);
  place_piece(expected, p0u);
  expected -> player = WHITES_TURN;

  if(success && game_eq(g_new, expected)){
    game_free(g_new);
    game_free(expected);
    cr_assert(1);
  }else{
    game_free(g_new);
    game_free(expected);
    cr_assert(0);
  }
}

//TESTS for game_outcome
Test(game_outcome, white_wins){
  game* g_new = new_game(3, 3, 4, MATRIX);
  pos p0 = make_pos(1, 2), p1 = make_pos(1, 1), p2 = make_pos(1, 0), 
      p3 = make_pos(0, 0), p4 = make_pos(2, 1), p5 = make_pos(2, 2);
  place_piece(g_new, p0);
  place_piece(g_new, p1);
  place_piece(g_new, p2);
  place_piece(g_new, p3);
  place_piece(g_new, p4);
  place_piece(g_new, p5);
  outcome winner = game_outcome(g_new);
  if(winner == WHITE_WIN){
    game_free(g_new);
    cr_assert(1);
  }else{
    game_free(g_new);
    cr_assert(0);
  }
}

Test(game_outcome, black_win){
  game* g_new = new_game(3, 3, 3, MATRIX);
  pos p0 = make_pos(0, 1), p1 = make_pos(0, 0), p2 = make_pos(1, 0), 
      p3 = make_pos(0, 2), p4 = make_pos(1, 2), p5 = make_pos(2, 0),
      p6 = make_pos(1, 1);
  place_piece(g_new, p0);
  place_piece(g_new, p1);
  place_piece(g_new, p2);
  place_piece(g_new, p3);
  place_piece(g_new, p4);
  place_piece(g_new, p5);
  place_piece(g_new, p6);
  outcome winner = game_outcome(g_new);
  if(winner == BLACK_WIN){
    game_free(g_new);
    cr_assert(1);
  }else{
    game_free(g_new);
    cr_assert(0);
  }
}
Test(game_outcome, in_progress){
  game* g_new = new_game(3, 3, 3, MATRIX);
  pos p0 = make_pos(0, 1), p1 = make_pos(0, 0), p2 = make_pos(1, 0), 
      p3 = make_pos(0, 2), p4 = make_pos(1, 2), p5 = make_pos(2, 0);
  place_piece(g_new, p0);
  place_piece(g_new, p1);
  place_piece(g_new, p2);
  place_piece(g_new, p3);
  place_piece(g_new, p4);
  place_piece(g_new, p5);
  outcome winner = game_outcome(g_new);
  if(winner == IN_PROGRESS){
    game_free(g_new);
    cr_assert(1);
  }else{
    game_free(g_new);
    cr_assert(0);
  }
}
Test(game_outcome, draw){
  game* g_new = new_game(3, 3, 3, MATRIX);
  pos p0 = make_pos(0, 1), p1 = make_pos(0, 0), p2 = make_pos(1, 0), 
      p3 = make_pos(0, 2), p4 = make_pos(1, 2), p5 = make_pos(1, 1), 
      p6 = make_pos(2, 0), p7 = make_pos(2, 1), p8 = make_pos(2, 2);
  place_piece(g_new, p0);
  place_piece(g_new, p1);
  place_piece(g_new, p2);
  place_piece(g_new, p3);
  place_piece(g_new, p4);
  place_piece(g_new, p5);
  place_piece(g_new, p6);
  place_piece(g_new, p7);
  place_piece(g_new, p8);
  outcome winner = game_outcome(g_new);
  if(winner == DRAW){
    game_free(g_new);
    cr_assert(1);
  }else{
    game_free(g_new);
    cr_assert(0);
  }
}


