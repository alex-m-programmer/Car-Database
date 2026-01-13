#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "list.h"

void getLine(char* buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForUser() {
    char temp[MAX_STRING_LENGTH];
    printf("\nPress Enter to continue...");
    fgets(temp, sizeof(temp), stdin);
}

void printTableHeader() {
    printf("\n%-15s %-20s %-20s %s\n", "Plate", "Color", "Manufacturer", "Year");
    printf("----------------------------------------------------------------\n");
}

void printTableFooter() {
    printf("----------------------------------------------------------------\n");
}

void printMenu() {
    printf("----------MAIN MENU----------");
    printf("\n1. SHOW ALL");
    printf("\n2. ADD NEW ENTRY");
    printf("\n3. DELETE ENTRY");
    printf("\n4. UPDATE ENTRY");
    printf("\n5. SEARCH");
    printf("\n6. SAVE CHANGES");
    printf("\n0. EXIT");
    printf("\nSelect an option: ");
}

void printRecordMenu() {
	printf("\n----- RECORD MENU -----");
    printf("\n1. LICENSE PLATE");
    printf("\n2. COLOR");
    printf("\n3. MANUFACTURER");
    printf("\n4. YEAR");
    printf("\n0. EXIT");
    printf("\nSelect an option: ");
}

void showAll(Database* db) {
	if (!db->head) {
        printf("No cars in database.\n");
        return;
    }
    
    Node* current = db->head;
    printTableHeader();
    while (current) {
        printf("%-15d %-20s %-20s %d\n", current->data.plate, current->data.color, current->data.manufacturer, current->data.year);
        current = current->next;
    }
    printTableFooter();
}

void addEntry(Database* db) {
    Car car;
    char buffer[MAX_STRING_LENGTH];

    printf("Enter vehicle license plate number (1000-9999): ");
    getLine(buffer, sizeof(buffer));
    if (sscanf(buffer, "%d", &car.plate) != 1) {
    	printf("Invalid input!\n");
    	return;
	}

    if (!isPlateValid(car.plate)) {
    	printf("Invalid license plate! Range 1000-9999.\n");
    	return;
	}
	
	if (findByPlate(db, car.plate)) {
		printf("License plate already exists!\n");
		return;
	}
	
    printStringList(db->basicColors, db->colorCount, "Available colors");
    printf("Choose a color: ");
    getLine(car.color, sizeof(car.color));
    if (!normalizeString(car.color, false, false) || !existsInList(db->basicColors, db->colorCount, car.color)) {
        printf("Invalid color selection!\n");
        return;
    }
	
	printStringList(db->companies, db->companyCount, "Available manufacturers");
    printf("Choose a manufacturer: ");
    getLine(car.manufacturer, sizeof(car.manufacturer));
    if (!normalizeString(car.manufacturer, true, true) || !existsInList(db->companies, db->companyCount, car.manufacturer)) {
        printf("Invalid manufacturer selection!\n");
        return;
    }

    printf("Enter year of manufacture (1960-2025): ");
    getLine(buffer, sizeof(buffer));
    if (sscanf(buffer, "%d", &car.year) != 1) {
    	printf("Invalid input!\n");
    	return;  	
	}

    if (!isYearValid(car.year)) {
    	printf("Invalid year!\n");
		return;	
	}

    insertSorted(db, car);
    printf("Car added successfully!\n");
}

void deleteEntry(Database* db) {
    char buffer[MAX_STRING_LENGTH];
    int plate;
    printf("Enter license plate to delete: ");
    getLine(buffer, sizeof(buffer));
    if (sscanf(buffer, "%d", &plate) != 1) {
        printf("Invalid input!\n");
        return;
	}
	
    if (deleteByPlate(db, plate)) {
        printf("Car with plate %d deleted successfully.\n", plate);
    } 
	else {
        printf("License plate not found.\n");
    }
}

void updateEntry(Database* db) {
    if (!db->head) {
        printf("Database empty!\n");
        return;
    }
    
    char buffer[MAX_STRING_LENGTH];
    int plate;
    showAll(db);
    
    printf("Select license plate to update: ");
    getLine(buffer, sizeof(buffer));
    if (sscanf(buffer, "%d", &plate) != 1) {
    	printf("Invalid input!\n");
    	return;
	}

    Node* current = findByPlate(db, plate);
    if (!current) {
        printf("License plate not found.\n");
        return;
    }
	
	int choice;
	do {
		printRecordMenu();
		getLine(buffer, sizeof(buffer));
		if (sscanf(buffer, "%d", &choice) != 1) {
			choice = -1;
		}
		if (choice == 0) break;
		
		switch (choice) {
			case 1: {
				int newPlate;
                printf("Enter new plate: ");
                getLine(buffer, sizeof(buffer));
                if (sscanf(buffer, "%d", &newPlate) != 1) {
                	printf("Invalid input!\n");
    				break;
				}
				
				if (!isPlateValid(newPlate)) {
					printf("Invalid license plate! Range 1000-9999.\n");
					break;
				}
				
				if (findByPlate(db, newPlate)) {
					printf("License plate already exists!\n");
					break;	
				}
				
				Car temp = current->data;
                deleteByPlate(db, current->data.plate);
                temp.plate = newPlate;
                insertSorted(db, temp);
                current = findByPlate(db, newPlate); 
                printf("License plate updated.\n");
				break;
			}
			case 2: {
				char newColor[MAX_STRING_LENGTH];
				printStringList(db->basicColors, db->colorCount, "Available colors");
                printf("Enter new color: ");
                getLine(buffer, sizeof(buffer));
                strcpy(newColor, buffer);
                if (!normalizeString(newColor, false, false)) {
                	printf("Invalid input!\n");
                	break;
				}
				
				if (!existsInList(db->basicColors, db->colorCount, newColor)) {
					printf("Invalid color.\n");
					break;
				}
				strcpy(current->data.color, newColor);
                printf("Color updated.\n");
				break;
			}
			case 3: {
				char newManufacturer[MAX_STRING_LENGTH];
                printStringList(db->companies, db->companyCount, "Available manufacturers");
                printf("Enter new manufacturer: ");
                getLine(buffer, sizeof(buffer));
                strcpy(newManufacturer, buffer);
                if (!normalizeString(newManufacturer, true, true)) {
                	printf("Invalid input!");
                	break;
				}
				
				if (!existsInList(db->companies, db->companyCount, newManufacturer)) {
					printf("Invalid manufacturer.\n");
					break;
				}
				
				strcpy(current->data.manufacturer, newManufacturer);
                printf("Manufacturer updated.\n");
				break;
			}
			case 4: {
				int newYear;
                printf("Enter new year: ");
                getLine(buffer, sizeof(buffer));
                if (sscanf(buffer, "%d", &newYear) != 1) {
                    printf("Invalid input!\n");
                    break;
                }
                
                if (!isYearValid(newYear)) {
                	printf("Invalid year.\n");
                	break;
				}
				
				current->data.year = newYear;
                printf("Year updated.\n");
				break;
			}
			
			default:
                printf("Invalid choice.\n");
                break;
		}
	} while (choice != 0);
}

void searchEntry(Database* db) {
    if (!db->head) {
        printf("Database empty!\n");
        return;
    }

    char buffer[50];
    int choice;

    do {
        printRecordMenu();
        getLine(buffer, sizeof(buffer));
        if (sscanf(buffer, "%d", &choice) != 1) {
        	choice = -1;
		}
		
        if (choice == 0) break;

        switch (choice) {
            case 1: {
                int plate, direction, found;
                Node* current;

                printf("Enter plate number: ");
                getLine(buffer, sizeof(buffer));
                if (sscanf(buffer, "%d", &plate) != 1) {
                    printf("Invalid input!\n");
                    break;
                }

                printf("1. Higher than %d\n2. Lower than %d\nChoose: ", plate, plate);
                getLine(buffer, sizeof(buffer));
                if (sscanf(buffer, "%d", &direction) != 1 || (direction != 1 && direction != 2)) {
                    printf("Invalid choice!\n");
                    break;
                }

                found = 0;
                current = db->head;
                printTableHeader();
                while (current) {
                    if ((direction == 1 && current->data.plate > plate) || (direction == 2 && current->data.plate < plate)) {
                        printf("%-15d %-20s %-20s %d\n", current->data.plate, current->data.color, current->data.manufacturer, current->data.year);
                        found = 1;
                    }
                    current = current->next;
                }

                if (!found) {
                	printf("No matching cars found.\n");
				}
				printTableFooter();
                break;
            }

            case 2: {
                char color[MAX_STRING_LENGTH];
                int found;
                Node* current;

                printStringList(db->basicColors, db->colorCount, "Available colors");
                printf("Enter color: ");
                getLine(buffer, sizeof(buffer));
                strcpy(color, buffer);
                if (!normalizeString(color, false, false)) {
                	printf("Invalid input!");
                	break;
				}

                found = 0;
                current = db->head;
                printTableHeader();
                while (current) {
                    if (strcmp(current->data.color, color) == 0) {
                        printf("%-15d %-20s %-20s %d\n", current->data.plate, current->data.color, current->data.manufacturer, current->data.year);
                        found = 1;
                    }
                    current = current->next;
                }
                if (!found) {
                	printf("No cars with color %s found.\n", color);
				}
				printTableFooter();
                break;
            }

            case 3: {
                char manufacturer[MAX_STRING_LENGTH];
                int found;
                Node* current;

                printStringList(db->companies, db->companyCount, "Available manufacturers");
                printf("Enter manufacturer: ");
                getLine(buffer, sizeof(buffer));
                strcpy(manufacturer, buffer);
                if (!normalizeString(manufacturer, true, true)) {
                	printf("Invalid input!");
                	break;
				}

                found = 0;
                current = db->head;
                printTableHeader();
                while (current) {
                    if (strcmp(current->data.manufacturer, manufacturer) == 0) {
                        printf("%-15d %-20s %-20s %d\n", current->data.plate, current->data.color, current->data.manufacturer, current->data.year);
                        found = 1;
                    }
                    current = current->next;
                }
                if (!found) {
                	printf("No cars from %s found.\n", manufacturer);	
				}
				printTableFooter();
                break;
            }

            case 4: {
                int year, found;
                Node* current;

                printf("Enter year: ");
                getLine(buffer, sizeof(buffer));
                if (sscanf(buffer, "%d", &year) != 1) {
                    printf("Invalid input!\n");
                    break;
                }

                found = 0;
                current = db->head;
                printTableHeader();
                while (current) {
                    if (current->data.year == year) {
                        printf("%-15d %-20s %-20s %d\n", current->data.plate, current->data.color, current->data.manufacturer, current->data.year);
                        found = 1;
                    }
                    current = current->next;
                }
                if (!found) {
                	printf("No cars from year %d found.\n", year);
				}
				printTableFooter();
                break;
            }

            default:
                printf("Invalid choice!\n");
                break;
        }

    } while (choice != 0);
}
