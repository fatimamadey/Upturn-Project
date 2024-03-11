#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>
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

/* Tracks most recent rotation if any to prevent a player from immediately
   counteracting the rotation made on the previous turn by opponent */
enum rotation {
    NO_ROTATION,
    CLOCKWISE,
    COUNTERCLOCKWISE
};

typedef enum rotation rotation;

/* Tracks the required length of a winning run of pieces. Houses the board. 
   Tracks the order in which pieces are placed by each player in quest to 
   facilitate the implementation of the uplift move. Stores the player whose 
   turn it is. Tracks what rotation, if any, was preformed during the preceding
   turn.
*/
struct game {
    unsigned int run;
    board* b;
    posqueue *black_queue, *white_queue;
    turn player;
    rotation last_rotation;
};

typedef struct game game;

/* Creates a new game with the specified size and configuration (Matrix or Bits)
   (Currently only matrix is supported will raise error if type is Bits)
*/
game* new_game(unsigned int run, unsigned int width,
               unsigned int height, enum type type);

/* Completly frees a game struct */
void game_free(game* g);

/* Places a piece belonging to the player whose turn it is in the specified location. 
   If the spot is already occupied does nothing and returns false else returns true.
   Moves on to the next turn. Updates queue on piece most recently placed. If an illegal 
   move is requested, the player does not forfeit their turn.
 */
bool place_piece(game* g, pos p);

/* Changes the board within the game, turning it 90 degrees clockwise or 
   counterclockwise. Makes a new board copies over contents of previous board. 
   Adjust coordinates of all queue entries as well. Record most recent move as a 
   rotation. Advance turn to next player. Return true if rotation was successful, 
   returns false if illegal rotation requested such as undoing the rotation of the
   previous turn. 
 */
bool rotate(game* g, bool clockwise);

/* The uplift function should uplift the oldest piece of the specified color. Advance the 
   turn. Record that the just-completed move was not a rotation. Change the piece just 
   uplifted to the most-recently-placed piece for that color. This function will usually 
   return true, but if no pieces of the specified color exist, do nothing and return false.
*/
bool uplift(game* g, cell c);

/* Report the outcome of the completed game or if the game is still in progress */
outcome game_outcome(game* g);

#endif /* LOGIC_H */
