#ifndef GAME_H
#define GAME_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

#define MATRIX_WIDTH 320
#define MATRIX_HEIGHT 160

#define MAX_CELL_AGE 10
#define CELL_AGE_INCREMENT 255 / MAX_CELL_AGE

#define CELLS MATRIX_WIDTH * MATRIX_HEIGHT
#define CELL_SIZE 2
#define SPACING 1

#define WIDTH MATRIX_WIDTH * (CELL_SIZE + SPACING) + MATRIX_PADDING * 2
#define HEIGHT MATRIX_HEIGHT * (CELL_SIZE + SPACING) + MATRIX_PADDING * 2
#define DEPTH 32
#define FREQ 30

#define MATRIX_PADDING 20

#define TRUE 1
#define FALSE 0

/* Structures */

typedef struct Cell {
    int life;
    int age;
} *Cell;

typedef struct Game {
    char *title;
    SDL_Surface *screen;
    SDL_Event event;
    int running;
} *Game;

/* Global Variables */

struct Cell smatrix[CELLS];
Game myGame;

/* Control Variables */

Uint8 *keystates;

/* Function Prototypes */

/* Loading / Initializing */
int init();
int loadFiles();

/* Game Loops */
void gameLoop();
void gameEvents();
void gameLogic();
void gameRender();

/* Life detection and reproduction function */
void life();
int getNeighbors(int index);

// Get the number of living adjecent cells.
int ngetNeighbors(int index);

/* Render functions */
void drawBackground();
void drawMatrix();
void drawCell(int x, int y, struct Cell *m);

#endif
