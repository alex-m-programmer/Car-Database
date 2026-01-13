Car Database Management System

A robust CLI-based database application written in C for managing vehicle records.
Built from scratch to demonstrate data structures, modular design, and safe file handling.

Key Features

• Sorted Data Storage
Automatically maintains records sorted by license plate number using a custom singly linked list.

• Strict Data Validation
– License plates: 1000–9999
– Manufacturing years: 1960–2025
– Colors and manufacturers validated against external reference files.

• Persistent Storage
Records are saved to and loaded from Cars.txt, with string normalization to ensure data integrity.

• Advanced Search & Filtering
Search vehicles by:
– Year
– Color
– Manufacturer
– License plate range

• Memory-Safe by Design
Manual memory management with proper allocation and deallocation to prevent leaks.

Technical Specifications

• Language: C (C99 / C11 compatible)
• Data Structure: Singly Linked List
– Search: O(n)
– Insertion: O(1) (after traversal)
• Input Handling:
– Uses fgets + sscanf
– Avoids unsafe scanf buffer overflow vulnerabilities
• File I/O: Standard C file handling (FILE*, fopen, fgets, fprintf)

Project Structure

• main.c
– Program entry point and main application loop

• list.c / list.h
– Linked list operations (Insert, Delete, Find, Count)

• database.c / database.h
– File I/O, validation logic, string normalization

• ui.c / ui.h
– CLI menus and formatted table output

How to Run
Compile
gcc main.c database.c list.c ui.c -o car_db

Required Data Files

Make sure the following files exist in the project root:

• BasicColors.txt
• Companies.txt
• Cars.txt

Execute

./car_db
