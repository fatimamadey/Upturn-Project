#ifndef BOARD_H
#define BOARD_H

#include "pos.h"

/* Represents the contents of a given location on thhe board */
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

/* Creates a fully empty board of desired size. Representation type can 
   be either Matrix or Bits, once type is chosen it cannot be changed. 
   (Currently only matrix is supported will raise error if type is Bits)
*/
board* board_new(unsigned int width, unsigned int height, enum type type);

/* Fully frees the board, including any internal representation.
   (Currently only matrix is supported will raise error if type is Bits) */
void board_free(board* b);

/* Prints board to screen including row and column headers for rows/columns 
   greater than 9 use Capital Letters than lowercase, if greater than that 
   use ? for all remaining rows/columns to indicate ran out of labels.
   (Currently only matrix is supported will raise error if type is Bits)
*/
void board_show(board* b);

/* Retrieve cells within the board. USE WHENEVER YOU WANT TO ACCESS GRID LOCATIONS
   (Currently only matrix is supported will raise error if type is Bits) 
   Add bound checking print error if out of bound*/
cell board_get(board* b, pos p);

/* Modifies cells within the board. USE WHENEVER YOU WANT TO CHANGE GRID LOCATIONS
   (Currently only matrix is supported will raise error if type is Bits) 
   Add bound checking to print error if out of bounds*/
void board_set(board* b, pos p, cell c);

#endif /* BOARD_H */
