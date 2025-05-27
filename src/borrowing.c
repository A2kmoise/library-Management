#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/library.h"

int add_borrowing(Borrowing borrowing) {
    FILE *file = fopen(BORROWINGS_FILE, "a");
    if (!file){
        printf("Failed to open file: %s\n", BORROWINGS_FILE); 
        return 1;
    }
            fprintf(file, "%d|%d|%d|%s|%s|%s\n",
            borrowing.borrowing_id, borrowing.book_id, borrowing.member_id, borrowing.borrow_date,
            borrowing.due_date, borrowing.return_date);

    fclose(file);
    return 0;
}

int read_borrowings(Borrowing **borrowings, int *count) {
    FILE *file = fopen(BORROWINGS_FILE, "r");
    if (!file) return 1;

    char line[512];
    int capacity = 10;
    *borrowings = malloc(capacity * sizeof(Borrowing));
    if (!*borrowings) {
        fclose(file);
        return 1;
    }

    *count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (*count >= capacity) {
            capacity *= 2;
            Borrowing *temp = realloc(*borrowings, capacity * sizeof(Borrowing));
            if (!temp) {
                free(*borrowings);
                fclose(file);
                return 1;
            }
            *borrowings = temp;
        }

        Borrowing b;
        sscanf(line, "%d|%d|%d|%[^|]|%[^|]|%[^\n]",
               &b.borrowing_id, &b.book_id, &b.member_id,
               b.borrow_date, b.due_date, b.return_date);

        (*borrowings)[*count] = b;
        (*count)++;
    }

    fclose(file);
    return 0;
}
