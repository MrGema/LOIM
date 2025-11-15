#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leaderboard.h"

LeaderboardEntry leaderboard[MAX_LEADERBOARD_ENTRIES];
int leaderboardCount = 0;


void loadLeaderboard(void) {
    FILE* file = fopen("leaderboard.csv", "r");
    if (!file) {
        // Ha a fájl nem létezik, az nem hiba, csak még nincs dicsőséglista.
        leaderboardCount = 0;
        return;
    }

    // A sscanf biztonságosabb, mint a strtok, mert kezeli az üres neveket is.
    while (leaderboardCount < MAX_LEADERBOARD_ENTRIES &&
           fscanf(file, "%50[^;];%d;%d\n",
                  leaderboard[leaderboardCount].playerName,
                  &leaderboard[leaderboardCount].moneyWon,
                  &leaderboard[leaderboardCount].timeInSeconds) == 3) {
        leaderboardCount++;
    }

    fclose(file);
}

void saveLeaderboard(void) {
    FILE* file = fopen("leaderboard.csv", "w");
    if (!file) {
        perror("Hiba a dicsosseglista mentese soran");
        return;
    }

    for (int i = 0; i < leaderboardCount; i++) {
        fprintf(file, "%s;%d;%d\n",
                leaderboard[i].playerName,
                leaderboard[i].moneyWon,
                leaderboard[i].timeInSeconds);
    }

    fclose(file);
}

void updateLeaderboard(const char* name, int money, int time) {
    int position = -1;

    // Megkeressük, hova kell beszúrni az új eredményt (magasabb pontszám elöl)
    for (int i = 0; i < leaderboardCount; i++) {
        if (money > leaderboard[i].moneyWon) {
            position = i;
            break;
        }
    }

    // Ha nem előz meg senkit, de van még hely a listán, a végére kerül
    if (position == -1 && leaderboardCount < MAX_LEADERBOARD_ENTRIES) {
        position = leaderboardCount;
    }

    // Ha nincs helye a listán, nem csinálunk semmit
    if (position == -1) {
        return;
    }

    // Ha a lista tele van, a legutolsó kiesik, a többiek hátrébb csúsznak
    if (leaderboardCount == MAX_LEADERBOARD_ENTRIES) {
        for (int i = leaderboardCount - 1; i > position; i--) {
            leaderboard[i] = leaderboard[i - 1];
        }
    } else { // Ha van még hely, csak hátrébb csúsznak és nő a lista mérete
        for (int i = leaderboardCount; i > position; i--) {
            leaderboard[i] = leaderboard[i - 1];
        }
        leaderboardCount++;
    }

    // Beszúrjuk az új bejegyzést a megüresedett helyre
    strncpy(leaderboard[position].playerName, name, 50);
    leaderboard[position].playerName[50] = '\0'; // Biztonsági null terminátor
    leaderboard[position].moneyWon = money;
    leaderboard[position].timeInSeconds = time;
    
    // A változások után azonnal mentjük a fájlba
    saveLeaderboard();
}