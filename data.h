#ifndef DATA_H
#define DATA_H

#include <stdbool.h>

#define MAX_QUESTIONS 5000
#define MAX_CATEGORIES 100

typedef struct {
    int difficulty;
    char* question;
    char* answer1;
    char* answer2;
    char* answer3;
    char* answer4;
    char* correctAnswer;
    char* category;
} Question;

extern Question questions[MAX_QUESTIONS];
extern int questionCount;

extern Question* questionPool[MAX_QUESTIONS];
extern int poolCount;

extern char* categories[MAX_CATEGORIES];
extern int categoryCount;


void readData(void);
void buildPoolByDifficulty(int minDiff, int maxDiff);
void getCategoriesFromPool(Question* pool[], int poolCount);
void filterPoolBySelectedCategories(const int selectedIndexes[], int count);


#endif