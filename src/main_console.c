#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/library.h"

int get_next_id(const char *filename, size_t struct_size) {
    FILE *file = fopen(filename, "r");
    if (!file) return 1;

    int count = 0;
    char line[1024];  

    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) > 1) count++; 
    }

    fclose(file);
    return count + 1;
}


void get_current_date(char *date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(date, 11, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

void get_due_date(char *date) {
    time_t t = time(NULL) + 14 * 24 * 60 * 60; 
    struct tm tm = *localtime(&t);
    snprintf(date, 11, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

int main() {
    int main_choice, book_choice;

    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Book\n");
        printf("2. Member\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &main_choice);
        getchar();

        if (main_choice == 1) {
            while (1) {
                printf("\n Books \n");
                printf("1. Add Book\n");
                printf("2. View Borrowings\n");
                printf("3. Borrow Book\n");
                printf("4. Back\n");
                printf("Enter choice: ");
                scanf("%d", &book_choice);
                getchar();

                if (book_choice == 1) {
                    Book book;
                    book.book_id = get_next_id(BOOKS_FILE, sizeof(Book));
                    printf("Enter title: ");
                    fgets(book.title, 255, stdin); book.title[strcspn(book.title, "\n")] = 0;
                    printf("Enter ISBN: ");
                    scanf("%s", book.isbn);
                    printf("Enter genre: ");
                    scanf("%s", book.genre);
                    printf("Enter year published: ");
                    scanf("%d", &book.year_published);
                    printf("Enter copies available: ");
                    scanf("%d", &book.copies_available);
                    printf("Enter shelf location: ");
                    scanf("%s", book.shelf_location);

                    getchar();
                    if (add_book(book) == 0)
                        printf("Book added successfully.\n");
                    else
                        printf("Failed to add book.\n");

                } else if (book_choice == 2) {
                    Borrowing *borrowings;
                    int count;
                    if (read_borrowings(&borrowings, &count) == 0) {
                        Book *books;
                        int book_count;
                        Member *members;
                        int member_count;
                        read_books(&books, &book_count);
                        read_members(&members, &member_count);

                        printf("\nBorrowings:\n");
                        printf("ID | Title | Member | Borrow Date | Due Date | Return Date\n");
                        for (int i = 0; i < count; i++) {
                            char *title = "Unknown";
                            char *name = "Unknown";
                            for (int j = 0; j < book_count; j++)
                                if (books[j].book_id == borrowings[i].book_id)
                                    title = books[j].title;
                            for (int j = 0; j < member_count; j++)
                                if (members[j].member_id == borrowings[i].member_id)
                                    name = members[j].name;
                            printf("%d | %s | %s | %s | %s | %s\n",
                                borrowings[i].borrowing_id, title, name,
                                borrowings[i].borrow_date, borrowings[i].due_date,
                                borrowings[i].return_date[0] ? borrowings[i].return_date : "Not Returned");
                        }
                        free(borrowings);
                        free(books);
                        free(members);
                    }

                } else if (book_choice == 3) {
                    Borrowing borrowing;
                    borrowing.borrowing_id = get_next_id(BORROWINGS_FILE, sizeof(Borrowing));
                    printf("Enter book_id: ");
                    scanf("%d", &borrowing.book_id);
                    printf("Enter member_id: ");
                    scanf("%d", &borrowing.member_id);
                    get_current_date(borrowing.borrow_date);
                    get_due_date(borrowing.due_date);
                    strcpy(borrowing.return_date, "");
                    if (add_borrowing(borrowing) == 0 && update_book_copies(borrowing.book_id, -1) == 0)
                        printf("Book borrowed successfully.\n");
                    else
                        printf("Failed to borrow book.\n");

                } else if (book_choice == 4) {
                    break; 
                } else {
                    printf("Invalid choice.\n");
                }
            }

        } else if (main_choice == 2) {
            Member member;
            member.member_id = get_next_id(MEMBERS_FILE, sizeof(Member));
            printf("Enter name: ");
            fgets(member.name, 255, stdin); member.name[strcspn(member.name, "\n")] = 0;
            printf("Enter email: ");
            scanf("%s", member.email);
            get_current_date(member.date_joined);
            if (add_member(member) == 0)
                printf("Member added successfully.\n");
            else
                printf("Failed to add member.\n");

        } else if (main_choice == 3) {
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
