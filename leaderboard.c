#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "leaderboard.h"
#include "debugmalloc.h"

LeaderboardEntry leaderboard[MAX_LEADERBOARD_ENTRIES];
int leaderboardCount = 0;


void loadLeaderboard(void) {
    FILE* file = fopen("data/leaderboard.csv", "r");
    if (!file) {
        leaderboardCount = 0;
        return;
    }

    leaderboardCount = 0;

    char line[256];

    fgets(line, sizeof(line), file);

    while (leaderboardCount < MAX_LEADERBOARD_ENTRIES &&
           fgets(line, sizeof(line), file))
    {
        if (sscanf(line, "%49[^;];%d;%d",
                   leaderboard[leaderboardCount].playerName,
                   &leaderboard[leaderboardCount].moneyWon,
                   &leaderboard[leaderboardCount].timeInSeconds) == 3)
        {
            leaderboardCount++;
        }
    }

    fclose(file);
    printf("Loaded %d entries.\n", leaderboardCount);

}



void saveLeaderboard(void) {
    FILE* file = fopen("data/leaderboard.csv", "w+");
    if (!file) {
        return;
    }
    
    fprintf(file, "Név; Pénz; Idő\n");

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

    for (int i = 0; i < leaderboardCount; i++) {
        if (money > leaderboard[i].moneyWon) {
            position = i;
            break;
        }
    }

    if (position == -1 && leaderboardCount < MAX_LEADERBOARD_ENTRIES) {
        position = leaderboardCount;
    }

    if (position == -1) {
        return;
    }

    strncpy(leaderboard[position].playerName, name, 50);
    leaderboard[position].playerName[50] = '\0';
    leaderboard[position].moneyWon = money;
    leaderboard[position].timeInSeconds = time;

    saveLeaderboard();
}