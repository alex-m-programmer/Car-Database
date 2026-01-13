#ifndef DATABASE_H
#define DATABASE_H

#include <stdbool.h>

#define MAX_COLORS 13
#define MAX_COMPANIES 67
#define MAX_STRING_LENGTH 50
#define FILENAME "Cars.txt"

typedef struct {
    int plate;
    char color[MAX_STRING_LENGTH];
    char manufacturer[MAX_STRING_LENGTH];
    int year;
} Car;

typedef struct Node {
    Car data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    char basicColors[MAX_COLORS][MAX_STRING_LENGTH];
    char companies[MAX_COMPANIES][MAX_STRING_LENGTH];
    int colorCount;
    int companyCount;
} Database;


void loadDatabase(Database* db);
void saveDatabase(Database* db);
void freeDatabase(Database* db);
bool isPlateValid(int plate);
bool isYearValid(int year);
void printStringList(char list[][MAX_STRING_LENGTH], int count, const char* title);
bool existsInList(char list[][MAX_STRING_LENGTH], int count, const char* value);
bool normalizeString(char* s, bool allowDigits, bool allowHyphen);
void printTableHeader();
void printTableFooter();

#endif
