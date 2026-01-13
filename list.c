#include <stdlib.h>
#include "list.h"

Node* createNode(Car car) {
    Node* n = malloc(sizeof(Node));
    if (!n) return NULL;
    n->data = car;
    n->next = NULL;
    return n;
}

void insertSorted(Database* db, Car car) {
    Node* n = createNode(car);
    if (!n) return;

    if (!db->head || car.plate < db->head->data.plate) {
        n->next = db->head;
        db->head = n;
        return;
    }

    Node* current = db->head;
    while (current->next && current->next->data.plate < car.plate) {
    	current = current->next;
	}
        
    n->next = current->next;
    current->next = n;
}

bool deleteByPlate(Database* db, int plate) {
    Node* current = db->head;
	Node* prev = NULL;

    while (current && current->data.plate != plate) {
        prev = current;
        current = current->next;
    }

    if (!current) return false;

    if (!prev) db->head = current->next;
    else prev->next = current->next;

    free(current);
    return true;
}

Node* findByPlate(Database* db, int plate) {
    Node* current = db->head;
    while (current) {
        if (current->data.plate == plate) return current;
        current = current->next;
    }
    return NULL;
}

int countCars(Database* db) {
    int count = 0;
    Node* current = db->head;
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}
