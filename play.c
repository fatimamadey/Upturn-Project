#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "logic.h"

//Determines if it a valid row or col, if it is returns the row/col number in an out parameter.
bool is_valid_row_col(char c, int* num){
  *num = -1;
  if(c > 47 && c < 58){
    *num = c - 48;
    return true;
  }else if(c > 64 && c < 91){
    *num = c - 65 + 10;
    return true;
  }else if(c > 96 && c < 123){
    *num = c - 97 + 26 + 10;
    return true;
  }else{
    return false;
  }
}

void play_game(game* g){
  bool done = false;
  outcome outcome_status = IN_PROGRESS; 
  while(!done){
    char val_a;
    char val_b;
    if(g -> player == BLACKS_TURN){
      printf("\nBlacks turn ");
    }else{
      printf("\nWhites turn ");
    }
    printf(": ");
    scanf("%c%c%*c", &val_a, &val_b);
    ///////printf("input is %c, %c\n\n", val_a, val_b);

    //Special Move Cases
    if(val_a == '!'){
      if(val_b == '<'){
        bool rotated = rotate(g, false);
        if(!rotated){
          printf("Rotation counterclockwise invalid try another move.\n");
          continue;
        }
      }else if(val_b == '>'){
        bool rotated_c = rotate(g, true);
        if(!rotated_c){
          printf("Rotation clockwise invalid try another move.\n");
          continue;
        }
      }else if(val_b == 'B'){
        bool uplifted = uplift(g, BLACK);
        if(!uplifted){
          printf("Uplift of black invalid try another move.\n");
          continue;
        }
      }else if(val_b == 'W'){
        bool w_uplifted = uplift(g, WHITE);
        if(!w_uplifted){
          printf("Uplift of white invalid try another move.\n");
          continue;
        }
      }else{
        printf("Not a vaild input. If you want to do a special move input <, >, B, or W.\n");
        continue;
      }
      //Determines outcome if a special move was done.
      printf("\n");
      board_show(g -> b);
      outcome_status = game_outcome(g);
      if(outcome_status != IN_PROGRESS){
        done = true;
        continue;
      }else{
        continue;
      }
    }

    //Place piece case
    int rows;
    int cols;
    if(is_valid_row_col(val_a, &rows) && is_valid_row_col(val_b, &cols)){
      if(rows >= g -> b -> height || rows < 0 || cols >= g -> b -> width || cols < 0){
        printf("Out of range position try again. \n");
        continue;
      }
      pos new_pos = make_pos(rows, cols);
      bool placed = place_piece(g, new_pos);
      if(!placed){
        printf("Invalid spot chosen. There is a piece already there. \n");
        continue;
      }
      //Determines outcome
      printf("\n");
      board_show(g -> b);
      outcome_status = game_outcome(g);
      if(outcome_status != IN_PROGRESS){
        done = true;
        continue;
      }else{
        continue;
      }
    }
  }//closes while loop
  if(outcome_status == BLACK_WIN){
    printf("\n Black is the winner! \n");
  }else if(outcome_status == WHITE_WIN){
    printf("\n White is the winner! \n");
  }else{
    printf("\n It is a draw!\n");
  }
}
  
int main(int argc, char* argv[7]){ //ex ./play 
  //MAKE A NEW GAME
  //argv[0] = function
  int rows = 0;
  int cols = 0;
  int run = 0;
  enum type type;
  for(int i = 1; i < 8; i++){
    if(!(strcmp(argv[i], "-h"))){
      rows = atoi(argv[i + 1]);
    }
    if(!(strcmp(argv[i], "-w"))){
      cols = atoi(argv[i + 1]);
    }
    if(!(strcmp(argv[i],"-r"))){
      run = atoi(argv[i + 1]);
    }
    if(!(strcmp(argv[i], "-m"))){
      type = MATRIX;
    }
    if(!(strcmp(argv[i],"-b"))){
      type = BITS;
    }
  }
  game* g = new_game(run, cols, rows, type);
  printf("\n INSTRUCTIONS: If it is your turn input 'row col' you would like to place a piece." 
      "\n If you would like to rotate the board counterclockwise or clockwise input ! < or ! > respectively." 
      "\n If you would like to uplift a piece input ! B or ! W depending on what piece you want to uplift." 
      "\n Good Luck ^_^ \n\n");
  board_show(g -> b);
  printf("\n");
  play_game(g);
  return 0;
}


