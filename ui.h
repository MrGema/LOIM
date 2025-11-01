#ifndef UI_H
#define UI_H

void drawWelcomeScreen();
void drawDifficultyMenu();
void drawQuestionScreen(const char* player, int qNum, int money, const char* question, const char* answers[4]);
void drawLeaderboard();
void drawGameOverScreen(int prize, int minutes, int seconds);
void drawCategorySelection(char** categories, int categoryCount);

#endif
