#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "data.h"

Question questions[MAX_QUESTIONS];
int questionCount = 0;
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

    while (fgets(line, sizeof(line), stream)) {
        if (strlen(line) <= 1) continue;
        line[strcspn(line, "\r\n")] = '\0';

        tmp = strdup_safe(line);
        if (!tmp) continue;

        char* token = strtok(tmp, ";");
        int col = 0;

        while (token && col < 8) {
            trimToken(token);

            if (col == 0 && (unsigned char)token[0] == 0xEF &&
                (unsigned char)token[1] == 0xBB &&
                (unsigned char)token[2] == 0xBF) {
                memmove(token, token + 3, strlen(token) - 2);
            }

            //printf("[%d] %s\n", col, token);

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
    getCategories();
}

bool isInArray(const char *category) {
    for (int i = 0; i < categoryCount; i++) {
        if (strcmp(categories[i], category) == 0)
            return true;
    }
    return false;
}

void getCategories() {
    for (int i = 0; i < questionCount; i++) {
        if (questions[i].category == NULL || strlen(questions[i].category) == 0)
            continue;

        if (!isInArray(questions[i].category) && categoryCount < MAX_CATEGORIES) {
            categories[categoryCount] = strdup(questions[i].category);
            categoryCount++;
        }
    }
}