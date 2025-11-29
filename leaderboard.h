#ifndef LEADERBOARD_H
#define LEADERBOARD_H

typedef struct LeaderboardEntry {
    char playerName[51];
    int moneyWon;
    int timeInSeconds;
    struct LeaderboardEntry* next; 
} LeaderboardEntry;

extern LeaderboardEntry* leaderboardHead; 

void loadLeaderboard(void);
void saveLeaderboard(void);
void updateLeaderboard(const char* name, int money, int time);
void freeLeaderboard(void); 

#endif