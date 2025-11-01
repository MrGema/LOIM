#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game.h"
#include "ui.h"
#include "data.h"

void startGame(){
    printf("game started");
    drawWelcomeScreen();
    int choise;
    scanf("%d", &choise);

    switch (choise)
    {
    case 1: //start new game
        playGame();
        break;
    case 2: //leaderboard
        drawLeaderboard();

        while (getchar() != '\n');

        getchar();

        startGame();
        break;
    case 3: //exit
        exit(0);
        break;
    case 4: 
        getchar();
        startGame();
    default: 
        printf("invalid");
        break;
    }
}

void playGame(){
    char playerName[]="";
    printf("Kerlek add meg a neved: ");
    scanf("%c", playerName);
    drawCategorySelection(categories, categoryCount);

    int selected[categoryCount];
    int count=getUserSelections(selected, categoryCount);

    drawDifficultyMenu();

    //gameLoop();
}

int getUserSelections(int *outArray, int maxCount) {
    char input[256];
    fgets(input, sizeof(input), stdin);

    for (int i = 0; input[i]; i++) {
        if (input[i] == ',') input[i] = ' ';
    }

    char *token = strtok(input, " \t\n");
    int count = 0;
    if(input[0]=='0'){
        for(int i=0; i<maxCount; i++){
            outArray[i]=i+1;
        }
        return maxCount;
    }   

    while (token && count < maxCount) {
        while (!isdigit(*token) && *token != '\0') 
            token++;

        outArray[count] = atoi(token);
        count++;
        token = strtok(NULL, " \t\n");
    }

    return count;
}

void gameLoop(){
    int answer;
    bool correct=true;
    while(correct){
        //drawQuestionScreen();
    }
}