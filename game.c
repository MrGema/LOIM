#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#include "data.h"
#include "ui.h"
#include "game.h"
#include "debugmalloc.h"
#include "leaderboard.h"

static void playGame(void);
static void gameLoop(const char* player);
static int getUserSelections(int *outArray, int maxCount);
static void PhoneAFriend(Question* question);
static void AskTheAudience(Question* question);
static void FiftyFifty(Question* question);
static int getCorrectAnswerIndex(Question* question);
bool fiftyfifty=true;
bool phone=true;
bool audience=true;

void startGame() {
    int choice = 0;
    loadLeaderboard();

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
                drawLeaderboard(leaderboardHead);
                printf("\nNyomj egy entert a tovabblepeshez...");
                getchar();
                continue;
            case 3:
                freeAllMemory();
                exit(0);
            default:
                printf("Ervenytelen valasztas! Probald ujra.\n");
                break;
        }
    }
}

void playGame() {
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

    getCategoriesFromPool();
    drawCategorySelection(categories, categoryCount);

    int selectedCategories[categoryCount];
    int selectedCount = getUserSelections(selectedCategories, categoryCount);

    //Kérdések végső szűrése kategóriák szerint
    filterPoolBySelectedCategories(selectedCategories, selectedCount);

    if (poolCount == 0) {
        printf("Sajnos a valasztott kriteriumoknak egyetlen kerdes sem felelt meg.\n");
        return;
    }
    
    gameLoop(playerName);
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
        if (input[i] == ',') 
            input[i] = ' ';
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

void gameLoop(const char* player) {
    fiftyfifty=true;
    phone=true;
    audience=true;
    clock_t startTime = clock();
    int questionNumber = 1;
    int correctAnswers = 0;

    while (poolCount > 0 && correctAnswers < 15) {
        int randomIndex = rand() % poolCount;
        Question* currentQuestion = questionPool[randomIndex];
        drawQuestionScreen(player, questionNumber, correctAnswers * 1000, currentQuestion, fiftyfifty, phone, audience); //EZ MEG MINDIG GECIRE NEM IRJA KI A JOT

        char input;
        bool answerGiven = false;

        do {
            printf("\nValaszod (A/B/C/D) vagy segitseg (F, K, T): ");
            scanf(" %c", &input);
            while (getchar() != '\n');

            input = toupper(input);

            switch (input) {
                case 'F':
                    FiftyFifty(currentQuestion);
                    break;
                case 'K':
                    AskTheAudience(currentQuestion);
                    break;
                case 'T':
                    PhoneAFriend(currentQuestion);
                    break;
                case '1': 
                    input = 'A'; 
                    break;
                case '2': 
                    input = 'B'; 
                    break;
                case '3': 
                    input = 'C'; 
                    break;
                case '4': 
                    input = 'D'; 
                    break;
            }

            if (input >= 'A' && input <= 'D') {
                answerGiven = true;
            } else if (input != 'F' && input != 'K' && input != 'T') {
                printf("Ervenytelen bemenet! Probald ujra.\n");
            }

        } while (!answerGiven);

        if (input == currentQuestion->correctAnswer[0]) {
            printf("Helyes valasz!\n");
            correctAnswers++;
            questionNumber++;
            printf("Nyomj entert a kovetkezo kerdeshez...");
            getchar();
        } else {
            startTime = clock() - startTime;
            printf("Rossz valasz! A helyes valasz: %s\n", currentQuestion->correctAnswer);
            printf("\nJatek vege! Osszesen %d helyes valaszt adtal.\n", correctAnswers);
            printf("Nyomj entert a menübe visszatéréshez...");
            getchar();
            break; 
        }

        questionPool[randomIndex] = questionPool[poolCount - 1];
        poolCount--;
    }
    
    updateLeaderboard(player, correctAnswers * 1000, (clock() - startTime) / CLOCKS_PER_SEC);
}

void PhoneAFriend(Question* question) {
    if(!phone){
        printf("Ezt a segitseget mar felhasznaltad!\n");
        return;
    }

    int rnd= rand() % 100;
    if(rnd<80){
        printf("A baratod szerint a helyes valasz a(z) %s\n", question->correctAnswer);
    } else {
        //veletlenszeruen valaszt egy rossz valaszt
        int correctIndex=getCorrectAnswerIndex(question);
        int incorrectIndex;
        do {
            incorrectIndex = rand() % 4;
        } while (incorrectIndex == correctIndex);

        switch (incorrectIndex)
        {
            case 0:
                printf("A barátod szerint a helyes válasz a(z) %s\n", question->answer1);
                break;
            case 1:
                printf("A barátod szerint a helyes válasz a(z) %s\n", question->answer2);
                break;
            case 2:
                printf("A barátod szerint a helyes válasz a(z) %s\n", question->answer3);
                break;
            case 3:
                printf("A barátod szerint a helyes válasz a(z) %s\n", question->answer4);
                break;
        }
    }
    phone=false;
}

void AskTheAudience(Question* question) {
    if(!audience){
        printf("Ezt a segitseget mar felhasznaltad!\n");
        return;
    }
    int rnd= rand() % 100;
    if(rnd<80){
        printf("A kozonseg szavazatai alapjan a helyes valasz a(z) %s\n", question->correctAnswer);
    } else {
        int correctIndex=getCorrectAnswerIndex(question);
        int incorrectIndex;
        do {
            incorrectIndex = rand() % 4;
        } while (incorrectIndex == correctIndex);

        switch (incorrectIndex)
        {
            case 0:
                printf("A közönség szerint a helyes valasz a(z) %s\n", question->answer1);
                break;
            case 1:
                printf("A közönség szerint a helyes valasz a(z) %s\n", question->answer2);
                break;
            case 2:
                printf("A közönség szerint a helyes valasz a(z) %s\n", question->answer3);
                break;
            case 3:
                printf("A közönség szerint a helyes valasz a(z) %s\n", question->answer4);
                break;
        }
    }
    audience=false;
}

void FiftyFifty(Question* question) {
    if(!fiftyfifty){
        printf("Ezt a segitseget mar felhasznaltad!\n");
        return;
    }

    int correctIndex=getCorrectAnswerIndex(question);
    if(question->correctAnswer[0]=='A')
        printf("Az egyik helyes válasz lehetőség a(z) %s\n", question->answer1);
    else if(question->correctAnswer[0]=='B')
        printf("Az egyik helyes válasz lehetőség a(z) %s\n", question->answer2);
    else if(question->correctAnswer[0]=='C')
        printf("Az egyik helyes válasz lehetőség a(z) %s\n", question->answer3);
    else if(question->correctAnswer[0]=='D')
        printf("Az egyik helyes válasz lehetőség a(z) %s\n", question->answer4);
    int incorrectIndex;
    do {
        incorrectIndex = rand() % 4;
        } while (incorrectIndex == correctIndex);
        switch (incorrectIndex)
        {
            case 0:
                printf("A másik helyes válasz lehetőség a(z) %s\n", question->answer1);
                break;
            case 1:
                printf("A másik helyes válasz lehetőség a(z) %s\n", question->answer2);
                break;
            case 2:
                printf("A másik helyes válasz lehetőség a(z) %s\n", question->answer3);
                break;
            case 3:
                printf("A másik helyes válasz lehetőség a(z) %s\n", question->answer4);
                break;
        }
    
    fiftyfifty=false;
}

int getCorrectAnswerIndex(Question* question) {
    if(question->correctAnswer[0] == 'A') return 0;
    else if(question->correctAnswer[0] == 'B') return 1;
    else if(question->correctAnswer[0] == 'C') return 2;
    else if(question->correctAnswer[0] == 'D') return 3;
}