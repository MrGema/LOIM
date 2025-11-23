#ifndef UI_H
#define UI_H

#include "data.h"

void drawWelcomeScreen();
void drawDifficultyMenu();
void drawQuestionScreen(const char* player, int qNum, int money, const Question* question, bool fiftyFifty, bool phone, bool audience);
void drawLeaderboard();
void drawGameOverScreen(int prize, int minutes, int seconds);
void drawCategorySelection(char** categories, int categoryCount);

#endif
