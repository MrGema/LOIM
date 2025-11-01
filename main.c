#include<stdio.h>
#include <windows.h>
#include <stdbool.h> 
#include "game.h"
#include "data.h"

//gcc main.c game.c ui.c data.c -o main.exe
int main(){
    readData();
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    startGame();
    return 0;
}