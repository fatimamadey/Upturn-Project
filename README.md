# Upturn

Welcome to Upturn, the fully playable version of the classic two-player game developed as part of a project for CMSC 14300. In this project, we've implemented interactive gameplay, extended the board representation to support a more efficient bits option, and utilized pthreads for multithreading rotation operations.

## Features

### Interactive Gameplay
- Players can take turns placing pieces on the board.
- Commands are entered via the keyboard to place pieces, rotate the board, or uplift pieces.

### Command-line Arguments
- Command-line options allow users to specify game parameters such as board size, winning run length, and board representation type.
- Supported options include `-h` for height, `-w` for width, `-r` for run length, and `-m` or `-b` to choose between matrix or bits representation.

### Board Representation
- Two board representation options: matrix and bits.
- Matrix representation: Traditional two-dimensional array.
- Bits representation: Compact storage using two bits per cell.

### Multithreading
- Multithreading is used for rotating the board for matrix representation.
- Each row of the board is processed independently by a separate thread, improving performance for larger boards.

## Methods Used
- **Bit Packing**: Utilized bitwise operators to efficiently represent board states using two bits per cell.
- **Interactive Input**: Implemented scanf to read commands from the keyboard, allowing players to interact with the game.
- **Multithreading**: Leveraged pthreads to parallelize board rotation operations, improving performance for larger boards.
- **Command-line Parsing**: Used argc and argv to parse command-line arguments and configure game parameters accordingly.
- **Unit Testing**: Developed unit tests to ensure the correctness of the implementation, particularly for board manipulation and game logic.

## Getting Started
To play Upturn and explore the codebase, follow the instructions provided in the README.md file. You can clone the repository, compile the code using the provided Makefile, and run the executable to start playing the game.

## How to Play
Upturn is a two-player game where players take turns placing pieces on the board. The objective is to create a winning run of pieces horizontally, vertically, or diagonally.

### Game Setup
1. Clone the repository and compile the code using the provided Makefile.
2. Run the executable to start the game.
    - To run the executable and start the game, use the following command:
    `./play -h <height> -w <width> -r <winning_run_length> -<representation>`
    Replace `<height>`, `<width>`, and `<winning_run_length>` with the desired values for the height, width, and winning run length of the board, respectively. Additionally, choose either -m for the matrix representation or -b for the bits representation to specify the board representation type.

### Gameplay
1. The game begins with an empty board.
2. Players alternate turns, with the black player making the first move.
3. Each turn, players can perform one of the following actions:
   - Place a piece: Enter the row and column where you want to place your piece. Rows are labeled from 1 to the height of the board (e.g., 1 to 8), and columns are labeled from A to the width of the board (e.g., A to H).
   - Rotate the board: Enter the '!' character followed by '<' or '>' to rotate the board counterclockwise or clockwise, respectively.
   - Uplift a piece: Enter the '!' character followed by 'B' or 'W' to uplift the oldest black or white piece, respectively.
4. Invalid moves or inputs will prompt the player to try again without changing the turn.
5. The game continues until one player forms a winning run or the board is full.
6. The winning player is announced, and the game ends.

### Example Commands
- To place a piece in row 3 and column C: `3C`
- To rotate the board counterclockwise: `!<`
- To uplift the oldest white piece: `!W`
