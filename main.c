#include <stdio.h>

#include "game.h"

int main(int argc, const char *argv[]) {

    if (init() == FALSE) {
        printf("Initialization Failed!\n");
        return EXIT_FAILURE;
    }

    gameLoop();

    return EXIT_SUCCESS;
}

