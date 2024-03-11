#include <stdio.h>
#include <stdlib.h>
#include "pos.h"
enum cell {
    EMPTY,
    BLACK,
    WHITE
};

typedef enum cell cell;


union board_rep {
    enum cell** matrix;
    unsigned int* bits;
};

typedef union board_rep board_rep;

enum type {
    MATRIX, BITS
};


struct board {
    unsigned int width, height;
    enum type type;
    board_rep u;
};

typedef struct board board;


board* board_new(unsigned int width, unsigned int height, enum type type){
  board* new_board = (board*)malloc(sizeof(board));
  if(new_board == NULL){
    fprintf(stderr, "new_board: Dynamic Memory Allocation Failed\n");
    exit(1);
  }
  new_board -> width = width;
  new_board -> height = height;
  new_board -> type = type;
  //MATRIX REPRESENTATION
  if(type == MATRIX){
    cell** new_matrix = (cell**)malloc(sizeof(cell*) * height);
    if(new_matrix == NULL){
      fprintf(stderr, "new_matrix: Dynamic Memory Allocation Failed\n");
      exit(1);
    }
    for(unsigned int i = 0; i < height; i++){
      new_matrix[i] = (cell*)malloc(sizeof(cell) * width);
      if(new_matrix[i] == NULL){
        fprintf(stderr, "new_matrix columns: Dynamic Memory Allocation Failed\n");
        exit(1);
      }
      for(unsigned int j = 0; j < width; j++){
        new_matrix[i][j] = EMPTY;
      }
    }
    new_board -> u.matrix = new_matrix;
  }
  //BITS REPRESENTATION
  if(type == BITS){
    int total_bits = width * height * 2;
    int num_elements = (total_bits + 31) / 32; //calculates number of unsigned int elements needed
    unsigned int* bits_arr = (unsigned int*)malloc(sizeof(unsigned int) * num_elements);
    if(bits_arr == NULL){
      fprintf(stderr, "bits_arr: Dynamic Memory Allocation Failed\n");
      exit(1);
    }
    for(int i = 0; i < num_elements; i++){
      bits_arr[i] = 0; //set each integer in the array to 0 because the bit rep of 0 is equivalent to binary rep of all 32 0's
    }
    new_board -> u.bits = bits_arr;
  }
  return new_board;
}

void board_free(board* b){
  if(b -> type == MATRIX){
    unsigned int rows = b -> height;
    cell** matrix = b -> u.matrix;
    for(unsigned int i = 0; i < rows; i++){
      free(matrix[i]);
    }
    free(matrix);
    free(b);
  }else{
    free(b -> u.bits);
    free(b);
  }
}

void board_show(board* b){
  printf("  ");
  for(unsigned int c = 0; c < b -> width; c++){
    if (c < 10){
      printf("%c", 48 + c);
    }else if (9 < c && c < 36){
      printf("%c", 65 + (c - 10));
    }else if (35 < c && c < 62){
      printf("%c", 97 + (c - 36));
    }else{
      printf("?");
    }
  }
  printf("\n\n");
  if(b -> type == MATRIX){
    for(unsigned int i = 0; i < b -> height; i++){
      if(i < 10){
        printf("%c ", 48 + i);
      }else if (9 < i && i < 36){
        printf("%c ", 65 + (i - 10));
      }else if (35 < i && i < 62){
        printf("%c ", 97 + (i - 36));
      }else{
        printf("? ");
      }
      cell** matrix = b -> u.matrix;
      for(unsigned int j = 0; j < b -> width; j++){
        cell current_cell = matrix[i][j];
        if (current_cell == EMPTY){
          printf(".");
        }
        if (current_cell == BLACK){
          printf("*");
        }
        if (current_cell == WHITE){
          printf("o");
        }
      }
      printf("\n");
    }
  }else{
    int num_bits = b -> width * b -> height * 2;
    int len_arr = (num_bits + 31)/32;
    int cells_printed = 0;
    int row = 0;
    for(int i = 0; i < len_arr; i++){
      unsigned int num = b-> u.bits[i];
      
      for(int j = 31; j >= 0; j -= 2){
        int index = i * 32 + 31 - j; //31
        if(index >= num_bits){
          //If reached or exceeded total number of bits break
          break;
        }
        if((cells_printed % (b -> width)) == 0){
          if(row < 10){
            printf("%c ", 48 + row);
          }else if (9 < row && row < 36){
            printf("%c ", 65 + (row - 10));
          }else if (35 < row && row < 62){
            printf("%c ", 97 + (row - 36));
          }else{
            printf("? "); 
          }
          row++;
        }
        int bit_offset = index;
        int mask = 3;
        int res = (num >> bit_offset) & mask;
        if(res == 2){
          printf("o");
        }else if(res == 1){
          printf("*");
        }else{
          printf(".");
        }
        cells_printed++;
        if(cells_printed % b -> width == 0){
          printf("\n");
        }
      }
    }
    printf("\n");
  }
}

cell board_get(board* b, pos p){
  if(p.r >= b -> height || p.c >= b -> width){
    fprintf(stderr, "board_get: Out of bound position\n");
    exit(1);
  }
  cell pos_cell;
  if(b -> type == MATRIX){
    cell** matrix = b -> u.matrix;
    pos_cell = matrix[p.r][p.c];
  }else{
    unsigned int* bit_arr = b -> u.bits;
    int index = ((p.r * 2 * b -> width) + (p.c * 2));
    int bit_offset = index % 32;
    int arr_index = index/32;
    int mask = 3 << bit_offset; //To read 2 bits at the same time
    int res = (bit_arr[arr_index] & mask) >> bit_offset;
    if(res == 2){
      pos_cell = WHITE;
    }else if(res == 1){
      pos_cell = BLACK;
    }else{
      pos_cell = EMPTY;
    }
  }
  return pos_cell;
}

void board_set(board* b, pos p, cell c){
  if(p.r >= b -> height || p.c >= b -> width){
    fprintf(stderr, "board_set: Out of bound position\n");
    exit(1);
  }
  if(b -> type == MATRIX){
    cell** matrix = b -> u.matrix;
    matrix[p.r][p.c] = c;
  }else{
    unsigned int* bit_arr = b -> u.bits;
    int index = ((p.r * 2 * b -> width) + (p.c * 2));
    int bit_offset = index % 32;
    int arr_index = index/32;
    int color;
    if(c == BLACK){
      color = 1;
    }else if(c == WHITE){
      color = 2;
    }else{
      color = 3;
    }
    int mask = color << bit_offset;
    if(color == 1 || color == 2){
      bit_arr[arr_index] = bit_arr[arr_index] | mask;
    }else{
      bit_arr[arr_index] = bit_arr[arr_index] & (~mask); //set cell back to empty
    }
    b -> u.bits[arr_index] = bit_arr[arr_index];
  }
}
