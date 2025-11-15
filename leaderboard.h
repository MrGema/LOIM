#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#define MAX_LEADERBOARD_ENTRIES 50

typedef struct {
    char playerName[51];
    int moneyWon;
    int timeInSeconds;
} LeaderboardEntry;

extern LeaderboardEntry leaderboard[MAX_LEADERBOARD_ENTRIES];
extern int leaderboardCount;

void loadLeaderboard(void);
void saveLeaderboard(void);
void updateLeaderboard(const char* name, int money, int time);

#endif