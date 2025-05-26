#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/library.h"

int add_borrowing(Borrowing borrowing) {
    FILE *file = fopen(BORROWINGS_FILE, "a");
    if (!file) return 1;
    fwrite(&borrowing, sizeof(Borrowing), 1, file);
    fclose(file);
    return 0;
}

int read_borrowings(Borrowing **borrowings, int *count) {
    FILE *file = fopen(BORROWINGS_FILE, "r");
    if (!file) return 1;

    *count = 0;
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    *count = size / sizeof(Borrowing);
    rewind(file);

    *borrowings = malloc(size);
    if (!*borrowings) {
        fclose(file);
        return 1;
    }

    fread(*borrowings, sizeof(Borrowing), *count, file);
    fclose(file);
    return 0;
}