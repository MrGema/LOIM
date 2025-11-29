
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "game.h"
#include "data.h"

#ifdef _WIN32
#include <windows.h>
#endif

//gcc main.c game.c ui.c data.c leaderboard.c -o main.exe
int main() { 
    srand(time(NULL));
    readData();

#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    startGame();
    freeAllMemory();
    return 0;
}
