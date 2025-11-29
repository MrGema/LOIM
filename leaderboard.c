#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "leaderboard.h"
#include "debugmalloc.h"

LeaderboardEntry* leaderboardHead = NULL;

void freeLeaderboard(void) {
    LeaderboardEntry* current = leaderboardHead;
    LeaderboardEntry* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    leaderboardHead = NULL;
}

void loadLeaderboard(void) {
    freeLeaderboard(); 

    FILE* file = fopen("data/leaderboard.csv", "r");
    if (!file) {
        printf("Nem található a leaderboard fájl. Üres listával indulunk.\n");
        return;
    }

    int loadedCount = 0;
    char line[256];

    fgets(line, sizeof(line), file); 

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        LeaderboardEntry* newEntry = (LeaderboardEntry*)calloc(1, sizeof(LeaderboardEntry));
        if (newEntry == NULL) {
            freeLeaderboard(); 
            fclose(file);
            return;
        }

        if (sscanf(line, "%50[^;];%d;%d",
                   newEntry->playerName,
                   &newEntry->moneyWon,
                   &newEntry->timeInSeconds) == 3)
        {
            newEntry->next = NULL; 

            if (leaderboardHead == NULL) {
                leaderboardHead = newEntry;
            } else {
                LeaderboardEntry* current = leaderboardHead;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = newEntry; 
            }
            loadedCount++;
        } else {
            free(newEntry);
        }
    }

    fclose(file);
}

void saveLeaderboard(void) {
    FILE* file = fopen("data/leaderboard.csv", "w+");
    if (!file) {
        fprintf(stderr, "Error: Could not open leaderboard file for saving.\n");
        return;
    }
    
    fprintf(file, "Nev; Penz; Ido\n");

    LeaderboardEntry* current = leaderboardHead;
    while (current != NULL) {
        fprintf(file, "%s;%d;%d\n",
                current->playerName,
                current->moneyWon,
                current->timeInSeconds);
        current = current->next;
    }

    fclose(file);
}

void updateLeaderboard(const char* name, int money, int time) {
    //mivel fgets-sel olvassuk be, lehet benne újsor karakter
    char cleanName[51];
    strncpy(cleanName, name, 50);
    cleanName[50] = '\0';
    cleanName[strcspn(cleanName, "\n")] = 0;

    // Változók a végleges értékeknek
    int finalMoney = money;
    int finalTime = time;

    //Megnézzük, létezik-e már a játékos
    LeaderboardEntry* current = leaderboardHead;
    LeaderboardEntry* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->playerName, cleanName) == 0) {
            finalMoney += current->moneyWon;
            finalTime += current->timeInSeconds;

            //kivesszuk a régi elemet a listából, mivel uj helyre fog kerulni
            if (prev == NULL) {
                leaderboardHead = current->next;
            } else {
                prev->next = current->next;
            }
            
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }

    //uj rekord letrehozasa
    LeaderboardEntry* newEntry = (LeaderboardEntry*)calloc(1, sizeof(LeaderboardEntry));
    if (newEntry == NULL) return;

    strcpy(newEntry->playerName, cleanName);
    newEntry->moneyWon = finalMoney;
    newEntry->timeInSeconds = finalTime;
    newEntry->next = NULL;

    //beszurás a megfelelő helyre
    if (leaderboardHead == NULL || newEntry->moneyWon > leaderboardHead->moneyWon) {
        newEntry->next = leaderboardHead;
        leaderboardHead = newEntry;
    } 
    else {
        current = leaderboardHead;
        while (current->next != NULL && newEntry->moneyWon <= current->next->moneyWon) {
            current = current->next;
        }
        newEntry->next = current->next;
        current->next = newEntry;
    }

    saveLeaderboard();
}