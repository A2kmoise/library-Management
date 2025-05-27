#ifndef BOOK_H
#define BOOK_H

typedef struct {
    int book_id;
    char title[255];
    char isbn[20];
    char genre[50];
    int year_published;
    int copies_available;
    char shelf_location[50];
} Book;

int add_book(Book book);

int read_books(Book **books, int *count);

int update_book_copies(int book_id, int change);

#endif