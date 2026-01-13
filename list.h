#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include "database.h"

Node* createNode(Car car);
void insertSorted(Database* db, Car car);
bool deleteByPlate(Database* db, int plate);
Node* findByPlate(Database* db, int plate);
int countCars(Database* db);

#endif
