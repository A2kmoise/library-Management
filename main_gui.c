#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/library.h"

GtkWidget *window;
GtkWidget *tree_view;
GtkListStore *store;

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

void load_books() {
    gtk_list_store_clear(store);
    Book *books;
    int count;
    if (read_books(&books, &count) == 0) {
        for (int i = 0; i < count; i++) {
            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               0, books[i].book_id,
                               1, books[i].title,
                               2, books[i].copies_available,
                               -1);
        }
        free(books);
    }
}

void borrow_book_cb(GtkButton *button, gpointer data) {
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    GtkTreeIter iter;
    GtkTreeModel *model;
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gint book_id;
        gtk_tree_model_get(model, &iter, 0, &book_id, -1);

        Borrowing borrowing;
        borrowing.borrowing_id = get_next_id(BORROWINGS_FILE, sizeof(Borrowing));
        borrowing.book_id = book_id;
        borrowing.member_id = 1; 
        get_current_date(borrowing.borrow_date);
        get_due_date(borrowing.due_date);
        strcpy(borrowing.return_date, "");
        if (add_borrowing(borrowing) == 0 && update_book_copies(book_id, -1) == 0) {
            load_books();
            g_print("Book borrowed successfully.\n");
        } else {
            g_print("Failed to borrow book.\n");
        }
    }
}

void setup_gui() {
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Library Management");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    store = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);
    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree_view), -1, "Book ID", renderer, "text", 0, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree_view), -1, "Title", renderer, "text", 1, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree_view), -1, "Copies Available", renderer, "text", 2, NULL);
    gtk_box_pack_start(GTK_BOX(vbox), tree_view, TRUE, TRUE, 0);

    GtkWidget *borrow_button = gtk_button_new_with_label("Borrow Selected Book");
    g_signal_connect(borrow_button, "clicked", G_CALLBACK(borrow_book_cb), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), borrow_button, FALSE, FALSE, 0);

    load_books();
    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    setup_gui();
    gtk_main();
    return 0;
}