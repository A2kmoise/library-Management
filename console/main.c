#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

// Function to execute SQL statements
int execute_sql(MYSQL *conn, const char *sql, const char *success_msg) {
    if (mysql_query(conn, sql)) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        return 0;
    }
    printf("%s\n", success_msg);
    return 1;
}

// Function to create database tables
void create_tables(MYSQL *conn) {
    const char *sql_books = 
        "CREATE TABLE IF NOT EXISTS Books ("
        "book_id INT AUTO_INCREMENT PRIMARY KEY, "
        "title VARCHAR(255), "
        "author_id INT, "
        "publisher_id INT, "
        "isbn VARCHAR(50), "
        "genre VARCHAR(50), "
        "year_published INT, "
        "copies_available INT, "
        "shelf_location VARCHAR(50), "
        "FOREIGN KEY (author_id) REFERENCES Authors(author_id), "
        "FOREIGN KEY (publisher_id) REFERENCES Publishers(publisher_id));";

    const char *sql_authors = 
        "CREATE TABLE IF NOT EXISTS Authors ("
        "author_id INT AUTO_INCREMENT PRIMARY KEY, "
        "name VARCHAR(255), "
        "bio TEXT);";

    const char *sql_publishers = 
        "CREATE TABLE IF NOT EXISTS Publishers ("
        "publisher_id INT AUTO_INCREMENT PRIMARY KEY, "
        "name VARCHAR(255), "
        "address VARCHAR(255), "
        "contact_info VARCHAR(100));";

    const char *sql_members = 
        "CREATE TABLE IF NOT EXISTS Members ("
        "member_id INT AUTO_INCREMENT PRIMARY KEY, "
        "name VARCHAR(255), "
        "address VARCHAR(255), "
        "phone VARCHAR(15), "
        "email VARCHAR(100), "
        "date_joined DATE, "
        "membership_status VARCHAR(50));";

    const char *sql_staff = 
        "CREATE TABLE IF NOT EXISTS Staff ("
        "staff_id INT AUTO_INCREMENT PRIMARY KEY, "
        "name VARCHAR(255), "
        "role VARCHAR(100), "
        "email VARCHAR(100), "
        "phone VARCHAR(15));";

    const char *sql_borrowings = 
        "CREATE TABLE IF NOT EXISTS Borrowings ("
        "borrowing_id INT AUTO_INCREMENT PRIMARY KEY, "
        "book_id INT, "
        "member_id INT, "
        "borrow_date DATE, "
        "due_date DATE, "
        "return_date DATE, "
        "staff_id INT, "
        "FOREIGN KEY (book_id) REFERENCES Books(book_id), "
        "FOREIGN KEY (member_id) REFERENCES Members(member_id), "
        "FOREIGN KEY (staff_id) REFERENCES Staff(staff_id));";

    const char *sql_fines = 
        "CREATE TABLE IF NOT EXISTS Fines ("
        "fine_id INT AUTO_INCREMENT PRIMARY KEY, "
        "borrowing_id INT, "
        "amount DECIMAL(10, 2), "
        "paid BOOLEAN, "
        "date_paid DATE, "
        "FOREIGN KEY (borrowing_id) REFERENCES Borrowings(borrowing_id));";

    execute_sql(conn, sql_authors, "Authors table created.");
    execute_sql(conn, sql_publishers, "Publishers table created.");
    execute_sql(conn, sql_books, "Books table created.");
    execute_sql(conn, sql_members, "Members table created.");
    execute_sql(conn, sql_staff, "Staff table created.");
    execute_sql(conn, sql_borrowings, "Borrowings table created.");
    execute_sql(conn, sql_fines, "Fines table created.");
}

// Function to insert a book
void insert_book(MYSQL *conn) {
    char title[100], isbn[20], genre[50], shelf_location[20];
    int author_id, publisher_id, year_published, copies_available;
    printf("Enter book title: ");
    scanf(" %[^\n]", title);
    printf("Enter author_id: ");
    scanf("%d", &author_id);
    printf("Enter publisher_id: ");
    scanf("%d", &publisher_id);
    printf("Enter ISBN: ");
    scanf("%s", isbn);
    printf("Enter genre: ");
    scanf(" %[^\n]", genre);
    printf("Enter year published: ");
    scanf("%d", &year_published);
    printf("Enter copies available: ");
    scanf("%d", &copies_available);
    printf("Enter shelf location: ");
    scanf("%s", shelf_location);

    char sql[512];
    snprintf(sql, sizeof(sql),
             "INSERT INTO Books (title, author_id, publisher_id, isbn, genre, year_published, copies_available, shelf_location) "
             "VALUES ('%s', %d, %d, '%s', '%s', %d, %d, '%s');",
             title, author_id, publisher_id, isbn, genre, year_published, copies_available, shelf_location);

    execute_sql(conn, sql, "Book inserted successfully.");
}

// Function to insert an author
void insert_author(MYSQL *conn) {
    char name[100], bio[500];
    printf("Enter author name: ");
    scanf(" %[^\n]", name);
    printf("Enter bio: ");
    scanf(" %[^\n]", bio);

    char sql[512];
    snprintf(sql, sizeof(sql),
             "INSERT INTO Authors (name, bio) VALUES ('%s', '%s');",
             name, bio);

    execute_sql(conn, sql, "Author inserted successfully.");
}

// Function to display all books
void display_books(MYSQL *conn) {
    const char *sql = "SELECT * FROM Books;";
    MYSQL_RES *res;
    MYSQL_ROW row;

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "Error storing result: %s\n", mysql_error(conn));
        return;
    }

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            printf("%s = %s\n", mysql_fetch_lengths(res) ? mysql_fetch_lengths(res)[i] : "NULL", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }
    mysql_free_result(res);
}

// Main menu
void menu(MYSQL *conn) {
    int choice;
    do {
        printf("\nLibrary Management System\n");
        printf("1. Insert Book\n");
        printf("2. Insert Author\n");
        printf("3. Display Books\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                insert_book(conn);
                break;
            case 2:
                insert_author(conn);
                break;
            case 3:
                display_books(conn);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 4);
}

int main() {
    MYSQL *conn;

    // Initialize MySQL
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return EXIT_FAILURE;
    }

    // Connect to the database
    if (mysql_real_connect(conn, "localhost", "root", "password", "library_db", 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    create_tables(conn);
    menu(conn);

    mysql_close(conn);
    return 0;
}
