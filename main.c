#include <stdio.h>
#include "database.h"
#include "ui.h"

int main() {
    Database db;
    loadDatabase(&db);

    int choice;
    char buffer[10];

    do {
        clearScreen();
        printMenu();
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &choice);

        switch (choice) {
            case 1: showAll(&db); break;
            case 2: addEntry(&db); break;
            case 3: deleteEntry(&db); break;
            case 4: updateEntry(&db); break;
            case 5: searchEntry(&db); break;
            case 6: saveDatabase(&db); break;
        }

        if (choice) waitForUser();
    } while (choice != 0);

    freeDatabase(&db);
    return 0;
}
