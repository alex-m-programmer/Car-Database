#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "database.h"
#include "list.h"

static void normalize(char* s) {
	if (!s) return;
	int i;
    for (i = 0; s[i]; i++) {
    	s[i] = toupper((unsigned char)s[i]);
	}  
}

void loadDatabase(Database* db) {
    db->head = NULL;
    db->colorCount = 0;
    db->companyCount = 0;

    FILE* file = fopen(FILENAME, "r");
    if (file) {
        Car car;
        while (fscanf(file, "%d %49s %49s %d", &car.plate, car.color, car.manufacturer, &car.year) == 4) {
            normalize(car.color);
            normalize(car.manufacturer);
            insertSorted(db, car);
        }
        fclose(file);
    }

    file = fopen("BasicColors.txt", "r");
    if (file) {
        while (db->colorCount < MAX_COLORS && fgets(db->basicColors[db->colorCount], MAX_STRING_LENGTH, file)) {
            db->basicColors[db->colorCount][strcspn(db->basicColors[db->colorCount], "\n")] = 0;
            normalize(db->basicColors[db->colorCount++]);
        }
        fclose(file);
    }

    file = fopen("Companies.txt", "r");
    if (file) {
        while (db->companyCount < MAX_COMPANIES && fgets(db->companies[db->companyCount], MAX_STRING_LENGTH, file)) {
            db->companies[db->companyCount][strcspn(db->companies[db->companyCount], "\n")] = 0;
            normalize(db->companies[db->companyCount++]);
        }
        fclose(file);
    }
}

void saveDatabase(Database* db) {
	char buffer[MAX_STRING_LENGTH];
    char confirm;
    
    printf("Save changes? (y/n): ");
    getLine(buffer, sizeof(buffer));
    if (sscanf(buffer, " %c", &confirm) != 1 || (confirm != 'y' && confirm != 'Y')) {
        printf("Save cancelled.\n");
        return;
    }

    FILE* file = fopen(FILENAME, "w");
    if (!file) return;

    Node* current = db->head;
    while (current) {
        fprintf(file, "%d %s %s %d\n", current->data.plate, current->data.color, current->data.manufacturer, current->data.year);
        current = current->next;
    }
    printf("Database saved.\n");
    fclose(file);
}

void freeDatabase(Database* db) {
    Node* current = db->head;
    while (current) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    db->head = NULL;
}

bool isPlateValid(int plate) {
    return plate >= 1000 && plate <= 9999;
}

bool isYearValid(int year) {
    return year >= 1960 && year <= 2025;
}

void printStringList(char list[][MAX_STRING_LENGTH], int count, const char* title) {
    int i;
    printf("\n%s:\n", title);
    printf("----------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        printf("%s\n", list[i]);
    }
    printf("----------------------------------------------------\n");
}

bool existsInList(char list[][MAX_STRING_LENGTH], int count, const char *value) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(list[i], value) == 0) return true;
    }
    return false;
}

bool normalizeString(char *s, bool allowDigits, bool allowHyphen) {
    int i;
    if (!s || s[0] == '\0') return false;

    for (i = 0; s[i]; i++) {
        unsigned char c = (unsigned char)s[i];
        if (isalpha(c)) {
            s[i] = toupper(c);
        }
        else if (isspace(c)) {
            continue; 
        }
        else if (allowDigits && isdigit(c)) {}        
        else if (allowHyphen && c == '-') {}        
        else {
            return false;
        }
    }
    return true;
}
