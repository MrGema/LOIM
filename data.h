#ifndef DATA_H
#define DATA_H

#define MAX_QUESTIONS 5000

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

extern Question** questionPool;
extern int poolCount;

extern char** categories;
extern int categoryCount;


void readData(void);
void buildPoolByDifficulty(int minDiff, int maxDiff);
void getCategoriesFromPool(void);
void filterPoolBySelectedCategories(const int selectedIndexes[], int count);
void freeAllMemory(void);

#endif