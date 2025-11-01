#ifndef DATA_H
#define DATA_H

#include <stdbool.h> 

typedef struct {
    int difficulty;
    char* question;
    char* answer1;
    char* answer2;
    char* answer3;
    char* answer4;
    char* correctAnswer;
    char* category;
    int id;
} Question;

#define MAX_QUESTIONS 5000
#define MAX_CATEGORIES 100
extern Question questions[MAX_QUESTIONS];
extern char* categories[MAX_CATEGORIES];
extern int questionCount;
extern int categoryCount;

void readData(void);
void freeQuestions(void);
void getCategories(void);

#endif