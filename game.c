#include <stdlib.h>
#include <time.h>
#include "game.h"

int init() {
    // Exterminate all life on the matrix
    int i;

    srand(time(NULL));

    for (i = 0; i < CELLS; i++) {
        if (rand() % 100 > 92) {
            smatrix[i].life = TRUE;
        } else {
            smatrix[i].life = FALSE;
        }
    }

    if ((myGame = malloc(sizeof(struct Game))) != NULL) { 
        myGame->title = "Game of Lifez";
        myGame->running = TRUE;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL\n");
        return FALSE;
    }

    if(!(myGame->screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE))) {
        printf("Failed to set video mode (%dx%d@%d)\n", WIDTH, HEIGHT, DEPTH);
        return FALSE;
    }

    SDL_WM_SetCaption(myGame->title, NULL);

    keystates = SDL_GetKeyState(NULL);

    printf("Initialization successful\n");

    return TRUE;

}

void gameLoop() {

    while(myGame->running == TRUE) {
        gameEvents();
        gameLogic();
        gameRender();
    }

}

void gameEvents() {
    while(SDL_PollEvent( &(myGame->event) )) {
        if (myGame->event.type == SDL_KEYDOWN) {
            switch( myGame->event.key.keysym.sym ) {
                case SDLK_q: myGame->running = FALSE; break;
                default: break;
            }
        }
    }
}

void gameLogic() {
    life();
}

void life() {
    int i;
    // For every cell in the matrix:
    for(i = CELLS; i > 0; i--) {

        // Get the number of live adjecent cells.
        int neighbors = ngetNeighbors(i);

        if (smatrix[i].life == TRUE) {
            //1. Any live cell with fewer than two live neighbours dies, as if caused by under-population.
            if (neighbors < 2) {
                smatrix[i].life = FALSE;
            //2. Any live cell with two or three live neighbours lives on to the next generation.
            } else if (neighbors >= 2 && neighbors < 4) {
                smatrix[i].age += CELL_AGE_INCREMENT;
                // Own Rule 2.5: if the age of the cell is more than MAX_CELL_AGE - die.
                if (smatrix[i].age >= 255) {
                    smatrix[i].life = FALSE;
                }
            //3. Any live cell with more than three live neighbours dies, as if by overcrowding.
            } else if (neighbors > 3) {
                smatrix[i].life = FALSE;
            } else {
                printf("This should never happen\n");
            }
        } else {
            //4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
            if (ngetNeighbors(i) == 3) {
                smatrix[i].life = TRUE;
                smatrix[i].age = 0;
            }
        }
    }
}

int ngetNeighbors(int index) {

    // Note: we have some overlapping cases in the corners, but corners are special, and fuck them right now.
    // Todo: check if the cell is in the corner aswell, 

    // First, check that we are within bounds
    if (index > 0 && index < MATRIX_WIDTH * MATRIX_HEIGHT - 1) {

        // Check if we are at either corner, sloppily return 0 in that case. (there are only four corners so whatever)
        if (index == 0 || index == MATRIX_WIDTH || index == (MATRIX_HEIGHT * MATRIX_WIDTH) || index == ((MATRIX_HEIGHT * MATRIX_WIDTH) - MATRIX_WIDTH)) {
            // BUG: Despite the fact that this evaluates true more often than it should, the result is pretty good.
            printf("WOOOOOW, we hit a corner, index == %d\n", index);
            return 0;
        } else {

            // if the cell is along the top, dont check the rows above
            if (index <= MATRIX_WIDTH) {
                return smatrix[ index - 1 ].life + smatrix[ index + 1].life + \
                    smatrix[ index + MATRIX_WIDTH - 1].life + smatrix[ index + MATRIX_WIDTH ].life + smatrix[ index + MATRIX_WIDTH + 1 ].life;
            }

            // if the cell is along the left edge, dont check the cells to the left
            else if (index % MATRIX_WIDTH == 0) {
                return smatrix[ index - MATRIX_WIDTH ].life + smatrix[ index - MATRIX_WIDTH + 1].life + \
                    smatrix[ index + 1].life + \
                    smatrix[ index + MATRIX_WIDTH ].life + smatrix[ index + MATRIX_WIDTH + 1 ].life;
            } 

            // if the cell is along the right edge, dont check the cells to the right
            else if ((index + 1) % MATRIX_WIDTH == 0) {
                return smatrix[ index -MATRIX_WIDTH - 1 ].life + smatrix[ index - MATRIX_WIDTH ].life  + \
                    smatrix[ index - 1 ].life + \
                    smatrix[ index + MATRIX_WIDTH - 1].life + smatrix[ index + MATRIX_WIDTH ].life;
            }

            // if the cell is along the bottom edge, dont check the cells bellow
            else if (index > ((MATRIX_HEIGHT * MATRIX_WIDTH) - MATRIX_WIDTH)) {
                return smatrix[ index -MATRIX_WIDTH - 1 ].life + smatrix[ index - MATRIX_WIDTH ].life + smatrix[ index - MATRIX_WIDTH + 1].life + \
                    smatrix[ index - 1 ].life + smatrix[ index + 1].life;
            }

            // if we are not at corners or edges, we are in the middle, return the sum of all the adjecent cells (8 cells)
            else {
                return smatrix[ index -MATRIX_WIDTH - 1 ].life + smatrix[ index - MATRIX_WIDTH ].life + smatrix[ index - MATRIX_WIDTH + 1].life + \
                    smatrix[ index - 1 ].life + smatrix[ index + 1].life + \
                    smatrix[ index + MATRIX_WIDTH - 1].life + smatrix[ index + MATRIX_WIDTH ].life + smatrix[ index + MATRIX_WIDTH + 1 ].life;
            }
        }
    }
    else {
        printf("OUt of bounds? last cell migh be w/e, index = %d", index);
    }

    return 0;
}


// Gets all the neighbors, but has a bug/feature
int getNeighbors(int index) {
    int topleft = index - MATRIX_WIDTH - 1;
    int botright = index + MATRIX_WIDTH + 1;

    if (topleft > 0 && botright < CELLS) {
        return smatrix[ index -MATRIX_WIDTH - 1 ].life + smatrix[ index - MATRIX_WIDTH ].life + smatrix[ index - MATRIX_WIDTH + 1].life + \
            smatrix[ index - 1 ].life + smatrix[ index + 1].life + \
            smatrix[ index + MATRIX_WIDTH - 1].life + smatrix[ index + MATRIX_WIDTH ].life + smatrix[ index + MATRIX_WIDTH + 1 ].life;
    } else {
        return 0;
    }
}


void gameRender() {

    drawBackground();

    drawMatrix();

    SDL_Delay(1000 / FREQ);
    SDL_Flip(myGame->screen);

}

void drawBackground() {
    // to draw background image:
    // applySurface(0, 0, background, myGame->screen, NULL);
    // to just draw a solid color:
    SDL_FillRect(myGame->screen, &myGame->screen->clip_rect, SDL_MapRGB(myGame->screen->format, 0x05, 0x05, 0x05));
}

void drawMatrix() {
    int startx = MATRIX_PADDING;
    int starty = MATRIX_PADDING;
    int curx = startx;
    int cury = starty;

    int i;

    int rowNum = 0;

    for (i = 0; i < CELLS; i++) {
        drawCell(curx, cury, &smatrix[i]);
        curx += CELL_SIZE + SPACING;
        rowNum++;
        if (rowNum == MATRIX_WIDTH) {
            cury += CELL_SIZE + SPACING;
            curx = startx;
            rowNum = 0;
        }
    }

}

void drawCell(int x, int y, struct Cell *m) {
    SDL_Rect rect = {x, y, CELL_SIZE, CELL_SIZE};
    if (m->life == TRUE) {
        // Drraw the live cells, use the age of the cell to determine the color.
        // Older cells are darker.
        SDL_FillRect(myGame->screen, &rect, SDL_MapRGB(myGame->screen->format, (255 - m->age), 0x05, (255 / 2 - m->age / 2)));
    } else {
        SDL_FillRect(myGame->screen, &rect, SDL_MapRGB(myGame->screen->format, 0x05, 0x05, 0x05));
    }
}

