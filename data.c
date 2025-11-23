#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "data.h"
#include "debugmalloc.h"
#define MAX_QUESTIONS 5000
#define MAX_CATEGORIES 100

Question questions[MAX_QUESTIONS];
int questionCount = 0;

Question* questionPool[MAX_QUESTIONS];
int poolCount = 0;

char* categories[MAX_CATEGORIES];
int categoryCount = 0;

static char* strdup_safe(const char* str) {
    if (!str) return NULL;
    char* new_str = malloc(strlen(str) + 1);
    if (new_str) {
        strcpy(new_str, str);
    }
    return new_str;
}

void trimToken(char *str) {
    char *start = str;
    while (*start == ' ' || *start == '"' || *start == '\n' || *start == '\r')
        start++;
    char *end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '"' || *end == '\n' || *end == '\r'))
        *end-- = '\0';
    memmove(str, start, strlen(start) + 1);
}

void readData() {
    FILE* stream = fopen("data/loim.csv", "r");
    if (!stream) {
        printf("Error opening file\n");
        return;
    }

    char line[1024];
    char* tmp;
    fgets(line, sizeof(line), stream);

    while (fgets(line, sizeof(line), stream) && questionCount < MAX_QUESTIONS) {
        if (strlen(line) <= 1)
            continue;
        line[strcspn(line, "\r\n")] = '\0';

        tmp = strdup_safe(line);
        if (!tmp)
            continue;

        char* token = strtok(tmp, ";");
        int col = 0;

        while (token && col < 8) {
            trimToken(token);

            if (col == 0 && (unsigned char)token[0] == 0xEF &&
                (unsigned char)token[1] == 0xBB &&
                (unsigned char)token[2] == 0xBF) {
                memmove(token, token + 3, strlen(token) - 2);
            }

            switch(col) {
                case 0:
                    questions[questionCount].difficulty = atoi(token);
                    break;
                case 1:
                    questions[questionCount].question = strdup_safe(token);
                    break;
                case 2:
                    questions[questionCount].answer1 = strdup_safe(token);
                    break;
                case 3:
                    questions[questionCount].answer2 = strdup_safe(token);
                    break;
                case 4:
                    questions[questionCount].answer3 = strdup_safe(token);
                    break;
                case 5:
                    questions[questionCount].answer4 = strdup_safe(token);
                    break;
                case 6:
                    questions[questionCount].correctAnswer = strdup_safe(token);
                    break;
                case 7:
                    questions[questionCount].category = strdup_safe(token);
                    break;
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

Question* questionPool[MAX_QUESTIONS];

//Ez az első szűrési lépés nehézség alapján.
void buildPoolByDifficulty(int minDiff, int maxDiff) {
    poolCount = 0;
    for (int i = 0; i < questionCount; i++) {
        if (questions[i].difficulty >= minDiff && questions[i].difficulty <= maxDiff) {
            questionPool[poolCount] = &questions[i];
            poolCount++;
        }
    }

    if (poolCount == 0) {
        printf("Sajnos ezen a nehezsegi szinten nincsenek kerdesek.\n");
        return;
    }
}

//ez a második lépés, ami a nehezsegi szint alapján gyűjti a kategóriákat.
void getCategoriesFromPool(Question* pool[], int poolCount) {
    categoryCount = 0;
    for (int i = 0; i < poolCount; i++) {
        if (pool[i]->category == NULL || strlen(pool[i]->category) == 0)
            continue;

        if (!isInArray(pool[i]->category) && categoryCount < MAX_CATEGORIES) {
            categories[categoryCount] = strdup(pool[i]->category);
            categoryCount++;
        }
    }
}

//ez a harmadik lépés, ami a felhasználó által kiválasztott kategóriák és nehezseg alapján szűri a pool-t.
void filterPoolBySelectedCategories(const int selectedIndexes[], int count) {
    // Ha a felhasználó a '0'-t (Mind) választotta, nincs mit szűrni.
    if (count == 0) {
        return;
    }

    int currentPoolCount = 0;
    for (int i = 0; i < poolCount; i++) {
        bool shouldKeep = false;
        for (int j = 0; j < count; j++) {
            int selectedIndex = selectedIndexes[j];
            if (selectedIndex > 0 && selectedIndex <= categoryCount) {
                const char* selectedCategoryName = categories[selectedIndex - 1];
                if (questionPool[i]->category && strcmp(questionPool[i]->category, selectedCategoryName) == 0) {
                    shouldKeep = true;
                    break;
                }
            }
        }
        if (shouldKeep) {
            questionPool[currentPoolCount] = questionPool[i];
            currentPoolCount++;
        }
    }

    poolCount = currentPoolCount;
}