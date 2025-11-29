#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "data.h"
#include "debugmalloc.h"
#include "leaderboard.h"

Question questions[MAX_QUESTIONS];
int questionCount = 0;

Question** questionPool = NULL;
int poolCount = 0;

char** categories = NULL;
int categoryCount = 0;

static char* strdup_safe(const char* str) {
    if (!str) return NULL;
    char* copy = malloc(strlen(str) + 1);
    if (copy) strcpy(copy, str);
    return copy;
}

void readData() {
    FILE* stream = fopen("data/loim.csv", "r");
    if (!stream) {
        printf("Error opening file\n");
        return;
    }

    char line[1024];

    fgets(line, sizeof(line), stream);

    while (fgets(line, sizeof(line), stream)) {
        
        if (questionCount >= MAX_QUESTIONS) break;

        //leveszi a sorvégi újsor karaktereket
        line[strcspn(line, "\r\n")] = 0;

        //kell egy masolat, mert a strtok módosítja a stringet
        char* tmp = strdup_safe(line);
        if (!tmp) continue;

        char* token = strtok(tmp, ";");
        int col = 0;
        
        Question* q = &questions[questionCount];

        //feltölti a kerdes mezoit
        while (token && col < 8) {
            switch(col) {
                case 0: q->difficulty = atoi(token); break;
                case 1: q->question = strdup_safe(token); break;
                case 2: q->answer1 = strdup_safe(token); break;
                case 3: q->answer2 = strdup_safe(token); break;
                case 4: q->answer3 = strdup_safe(token); break;
                case 5: q->answer4 = strdup_safe(token); break;
                case 6: q->correctAnswer = strdup_safe(token); break;
                case 7: q->category = strdup_safe(token); break;
            }
            token = strtok(NULL, ";");
            col++;
        }

        free(tmp);
        questionCount++;
    }

    fclose(stream);
}

bool isInArray(const char *category) {
    for (int i = 0; i < categoryCount; i++) {
        if (strcmp(categories[i], category) == 0)
            return true;
    }
    return false;
}

//Ez az első szűrési lépés nehézség alapján.
void buildPoolByDifficulty(int minDiff, int maxDiff) {
    //Reset
    if (questionPool) {
        free(questionPool);
        questionPool = NULL;
    }
    poolCount = 0;

    //megszamoljuk hány kérdés felel meg a kritériumnak
    int requiredCount = 0;
    for (int i = 0; i < questionCount; i++) {
        if (questions[i].difficulty >= minDiff && questions[i].difficulty <= maxDiff) {
            requiredCount++;
        }
    }

    if (requiredCount == 0) {
        printf("Sajnos ezen a nehezsegi szinten nincsenek kerdesek.\n");
        return;
    }

    //lefoglaljuk a memoriát
    questionPool = (Question**)malloc(requiredCount * sizeof(Question*));
    if (!questionPool) {
        printf("Memory allocation failed for pool.\n");
        return;
    }

    //feltöltjük
    for (int i = 0; i < questionCount; i++) {
        if (questions[i].difficulty >= minDiff && questions[i].difficulty <= maxDiff) {
            questionPool[poolCount] = &questions[i];
            poolCount++;
        }
    }
}

//ez a második lépés, ami a nehezsegi szint alapján gyűjti a kategóriákat.
void getCategoriesFromPool() {
    for (int i = 0; i < poolCount; i++) {
        //Ha nince benne a tömbben, hozzáadjuk
        if (!isInArray(questionPool[i]->category)) {
            
            //növeljük a kategória tömb méretét
            char** temp = realloc(categories, (categoryCount + 1) * sizeof(char*));
            
            if (!temp) {
                printf("Memory allocation failed for categories.\n");
                return;
            }
            categories = temp;

            //allokáljuk és másoljuk a kategória nevet
            categories[categoryCount] = strdup(questionPool[i]->category);
            categoryCount++;
        }
    }
}

//ez a harmadik lépés, ami a felhasználó által kiválasztott kategóriák és nehezseg alapján szűri a pool-t.
void filterPoolBySelectedCategories(const int selectedIndexes[], int count) {
    //Ha a felhasználó 0-t adott meg, akkor az összes kategória ki van választva.
    if (count == 0) return;

    //az uj nem lehet nagyobb mint a régi, ezért elég ekkora memóriát foglalni
    Question** tempPool = (Question**)malloc(poolCount * sizeof(Question*));
    if (!tempPool) return;

    int newCount = 0;

    for (int i = 0; i < poolCount; i++) {
        bool shouldKeep = false;
        
        for (int j = 0; j < count; j++) {
            int selectedIndex = selectedIndexes[j];
            
            int arrayIndex = selectedIndex - 1;

            if (arrayIndex >= 0 && arrayIndex < categoryCount) {
                if (strcmp(questionPool[i]->category, categories[arrayIndex]) == 0) {
                    shouldKeep = true;
                    break;
                }
            }
        }

        if (shouldKeep) {
            tempPool[newCount] = questionPool[i];
            newCount++;
        }
    }

    free(questionPool);

    questionPool = tempPool;
    poolCount = newCount;

    if (poolCount == 0) {
        printf("Nincs kerdes a kivalasztott kategoriakban.\n");
    }
}

void freeAllMemory() {
    for (int i = 0; i < questionCount; i++) {
        free(questions[i].question);
        free(questions[i].answer1);
        free(questions[i].answer2);
        free(questions[i].answer3);
        free(questions[i].answer4);
        free(questions[i].correctAnswer);
        free(questions[i].category);
    }
    free(questionPool);
    
    for (int i = 0; i < categoryCount; i++) {
        free(categories[i]); 
    }
    free(categories);
    freeLeaderboard();
}