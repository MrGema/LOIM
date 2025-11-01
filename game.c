#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "data.h"
#include "ui.h"
#include "game.h"

static void playGame(void);
static void gameLoop(const char* player, bool* fiftyFifty, bool* phone, bool* audience);
static int getUserSelections(int *outArray, int maxCount);

void startGame() {
    int choice = 0;

    while (1) {
        drawWelcomeScreen();
        
        if (scanf("%d", &choice) != 1) {
            choice = -1;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                drawLeaderboard();
                printf("\nNyomj egy entert a tovabblepeshez...");
                getchar();
                continue;
            case 3:
                exit(0);
            default:
                printf("Ervenytelen valasztas! Probald ujra.\n");
                break;
        }
        break;
    }
}

void playGame() {
    bool fiftyfifty=true;
    bool phone=true;
    bool audiance=true;
    char playerName[51];
    int diffChoice = 0;

    printf("\nKerlek, add meg a jatekos nevet (max 50 karakter): ");
    fgets(playerName, sizeof(playerName), stdin);
    playerName[strcspn(playerName, "\n")] = 0;

    drawDifficultyMenu();
    char inputBuffer[10];
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    sscanf(inputBuffer, "%d", &diffChoice);

    int minDiff, maxDiff;
    switch(diffChoice) {
        case 1: minDiff = 1; maxDiff = 5; break;
        case 2: minDiff = 6; maxDiff = 10; break;
        case 3: minDiff = 11; maxDiff = 15; break;
        default: printf("Ervenytelen nehezsegi szint. Visszateres a fomenube.\n"); return;
    }

    //Kérdések előszűrése nehézség szerint
    buildPoolByDifficulty(minDiff, maxDiff);

    if (poolCount == 0) {
        printf("Sajnos ezen a nehezsegi szinten nincsenek elerheto kerdesek.\n");
        return;
    }

    getCategoriesFromPool(questionPool, poolCount);
    drawCategorySelection(categories, categoryCount);

    int selectedCategories[categoryCount];
    int selectedCount = getUserSelections(selectedCategories, categoryCount);

    //Kérdések végső szűrése kategóriák szerint
    filterPoolBySelectedCategories(selectedCategories, selectedCount);

    if (poolCount == 0) {
        printf("Sajnos a valasztott kriteriumoknak egyetlen kerdes sem felelt meg.\n");
        return;
    }
    
    gameLoop(playerName, &fiftyfifty, &phone, &audiance);
}

int getUserSelections(int *outArray, int maxCount) {
    char input[256];
    fgets(input, sizeof(input), stdin);

    if(input[0] == '0'){
        for(int i=0; i < maxCount; i++){
            outArray[i] = i + 1;
        }
        return maxCount;
    }   

    for (int i = 0; input[i]; i++) {
        if (input[i] == ',') input[i] = ' ';
    }

    char *token = strtok(input, " \t\n");
    int count = 0;
    while (token && count < maxCount) {
        while (*token != '\0' && !isdigit(*token)) {
            token++;
        }
        if (*token == '\0') continue;

        outArray[count] = atoi(token);
        count++;
        token = strtok(NULL, " \t\n");
    }

    return count;
}

void gameLoop(const char* player, bool* fiftyFifty, bool* phone, bool* audience) {
    int questionNumber = 1;
    int correctAnswers = 0;

    while(poolCount > 0) {
        int randomIndex = rand() % poolCount;
        Question* currentQuestion = questionPool[randomIndex];

        drawQuestionScreen(player, 1, correctAnswers * 1000, currentQuestion, &fiftyFifty, &phone, &audience);

        //ez meg nincs kesz annyira
        char answer;
        printf("Valaszod (A/B/C/D): "); //todo: 1 2 3 4 is A B C D legyen mert balfaszok az emberek
        scanf(" %c", &answer);
        if(answer=='1') answer='A';
        else if(answer=='2') answer='B';
        else if(answer=='3') answer='C';
        else if(answer=='4') answer='D';
        while (getchar() != '\n');
        if (toupper(answer) == toupper(currentQuestion->correctAnswer[0])) {
            printf("Helyes valasz!\n");
            correctAnswers++;
        } else {
            printf("Rossz valasz! A helyes valasz: %s\n", currentQuestion->correctAnswer);
            break;
        }
        questionPool[randomIndex] = questionPool[poolCount - 1];
        poolCount--;

        questionNumber++;
        printf("Nyomj entert a kovetkezo kerdeshez...");
        getchar();
    }

    printf("\nJatek vege! Osszesen %d helyes valaszt adtal.\n", correctAnswers);
    //restart option?
    // updateLeaderboard meg ilyenek
}