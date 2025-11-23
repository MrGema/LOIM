#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ui.h"
#include "data.h"
#include "leaderboard.h"

void drawWelcomeScreen() {
    system("cls");
    printf("==========================================\n");
    printf("\tLEGYEN ÖN IS MILLIOMOS!\n");
    printf("\tVÁLASSZON OPCIÓT\n");
    printf("1. Uj jatek\n2. Ranglista megtekintese\n3. Kilepes\n");
    printf("==========================================\n\n");
}

//ide maybe input bekeres style

void drawDifficultyMenu() {
    system("cls");
    printf("==========================================\n");
    printf("\tVÁLASSZON NEHÉZSEGI FOKOZATOT\n");
    printf("==========================================\n");
    printf("1. Konnyu\n2. Kozepes\n3. Nehez\n4. Vissza\n");
    printf("------------------------------------------\n");
}

void drawQuestionScreen(const char* player, int qNum, int money, const Question* question, bool fiftyFifty, bool phone, bool audience) {
    system("cls");
    printf("Név: %s | Kérdés #%d | Nyeremény: %d\n", player, qNum, money);
    printf("Segítségek: 50:50 (%s) | Telefonos (%s) | Közönség (%s)\n",
           fiftyFifty ? "elérhető" : "felhasználva",
           phone ? "elérhető" : "felhasználva",
           audience ? "elérhető" : "felhasználva");
    printf("----------------------------------------------------------------------\n");
    printf("%s\n\n", question->question);
    printf("A) %s\nB) %s\nC) %s\nD) %s\n", question->answer1, question->answer2, question->answer3, question->answer4);
    printf("----------------------------------------------------------------------\n");
    printf("Válaszod: ");
}
void drawGameOverScreen(int prize, int minutes, int seconds) {
    system("cls");
    printf("==========================================\n");
    printf("\tJáték vége\n");
    printf("==========================================\n");
    printf("Nyeremény: %d\nJátékban töltott idő: %02d:%02d\n", prize, minutes, seconds);
    printf("------------------------------------------\n");
}

void drawCategorySelection(char** catergories, int categoryCount) {
    system("cls");
    printf("==========================================\n");
    printf("\tVÁLASSZON KATEGÓRIÁKAT\n");
    printf("==========================================\n");
    printf("0. Összes \n");
    for(int i=0; i<categoryCount; i++){
            printf("%d. %s\n", i+1, catergories[i]);
    }
    printf("------------------------------------------\n");
    printf("írja be szóközzel elválasztva a kategóriák számát amelyikben játszani szeretne\n");
}

void drawLeaderboard(const LeaderboardEntry entries[], int entryCount) {
    system("cls");
    printf("================================================================================\n");
    printf("=                          D I C S O S E G L I S T A                           =\n");
    printf("================================================================================\n\n");

    printf("%-5s | %-50s | %12s | %10s\n", "Hely.", "Nev", "Nyeremeny", "Jatekido");
    printf("------+----------------------------------------------------+--------------+-----------\n");

    if (entryCount == 0) {
        printf("A dicsőséglista egyelőre üres...\n");
    } else {
        for (int i = 0; i < entryCount; i++) {
            printf("#%-4d | %-49s | %10d Ft | %7d s\n",
                   i + 1,
                   entries[i].playerName,
                   entries[i].moneyWon,
                   entries[i].timeInSeconds);
        }
    }

    printf("\n\nNyomj egy entert a fomenube valo visszatereshez...");
}