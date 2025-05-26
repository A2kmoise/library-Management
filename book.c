#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/library.h"

int add_book(Book book) {
    FILE *file = fopen(BOOKS_FILE, "a");
    if (!file) {
        printf("Failed to open file: %s\n", BOOKS_FILE); 
        return 1;
    }
        fprintf(file, "%d|%s|%s|%s|%d|%d|%s\n",
            book.book_id, book.title, book.isbn, book.genre,
            book.year_published, book.copies_available, book.shelf_location);

    fclose(file);
    return 0;
}


int read_books(Book **books, int *count) {
    FILE *file = fopen(BOOKS_FILE, "r");
    if (!file) return -1;

    *count = 0;
    Book temp;
    Book *list = malloc(sizeof(Book) * 100);
    while (fscanf(file, "%d|%255[^|]|%19[^|]|%29[^|]|%d|%d|%49[^\n]\n",
                  &temp.book_id, temp.title, temp.isbn, temp.genre,
                  &temp.year_published, &temp.copies_available, temp.shelf_location) == 7) {
        list[*count] = temp;
        (*count)++;
    }

    fclose(file);
    *books = list;
    return 0;
}


int update_book_copies(int book_id, int change) {
    Book *books;
    int count;
    if (read_books(&books, &count)) return 1;

    FILE *file = fopen(BOOKS_FILE, "w");
    if (!file) {
        free(books);
        return 1;
    }

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (books[i].book_id == book_id) {
            books[i].copies_available += change;
            found = 1;
        }
        fwrite(&books[i], sizeof(Book), 1, file);
    }

    fclose(file);
    free(books);
    return found ? 0 : 1;
}