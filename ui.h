#ifndef UI_H
#define UI_H

#include "database.h"

void clearScreen();
void waitForUser();
void printTableHeader();
void printTableFooter();
void printMenu();
void printRecordMenu();
void showAll(Database* db);
void addEntry(Database* db);
void deleteEntry(Database* db);
void updateEntry(Database* db);
void searchEntry(Database* db);

#endif
