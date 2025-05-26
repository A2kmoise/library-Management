#ifndef BORROWING_H
#define BORROWING_H

typedef struct {
    int borrowing_id;
    int book_id;
    int member_id;
    char borrow_date[11]; 
    char due_date[11];   
    char return_date[11];
} Borrowing;

int add_borrowing(Borrowing borrowing);
int read_borrowings(Borrowing **borrowings, int *count);

#endif