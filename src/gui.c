#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "library.c"

GtkWidget *window;
GtkWidget *entry_title, *entry_isbn;
GtkWidget *entry_member_name, *entry_member_email;
GtkWidget *label_result;

void on_button_add_book_clicked(GtkWidget *widget, gpointer data) {
    Book book;
    book.book_id = get_next_id(BOOKS_FILE, sizeof(Book));
    strcpy(book.title, gtk_entry_get_text(GTK_ENTRY(entry_title)));
    strcpy(book.isbn, gtk_entry_get_text(GTK_ENTRY(entry_isbn)));

    if (add_book(book) == 0) {
        gtk_label_set_text(GTK_LABEL(label_result), "Book added successfully!");
    } else {
        gtk_label_set_text(GTK_LABEL(label_result), "Failed to add book.");
    }
}

void on_button_add_member_clicked(GtkWidget *widget, gpointer data) {
    Member member;
    member.member_id = get_next_id(MEMBERS_FILE, sizeof(Member));
    strcpy(member.name, gtk_entry_get_text(GTK_ENTRY(entry_member_name)));
    strcpy(member.email, gtk_entry_get_text(GTK_ENTRY(entry_member_email)));
    get_current_date(member.date_joined);

    if (add_member(member) == 0) {
        gtk_label_set_text(GTK_LABEL(label_result), "Member added successfully!");
    } else {
        gtk_label_set_text(GTK_LABEL(label_result), "Failed to add member.");
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv());

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Library Management System");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

   
    GtkWidget *book_label = gtk_label_new("Add Book:");
    gtk_box_pack_start(GTK_BOX(vbox), book_label, FALSE, FALSE, 0);

    entry_title = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_title), "Book Title");
    gtk_box_pack_start(GTK_BOX(vbox), entry_title, FALSE, FALSE, 0);

    entry_isbn = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_isbn), "Book ISBN");
    gtk_box_pack_start(GTK_BOX(vbox), entry_isbn, FALSE, FALSE, 0);

    GtkWidget *button_add_book = gtk_button_new_with_label("Add Book");
    g_signal_connect(button_add_book, "clicked", G_CALLBACK(on_button_add_book_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button_add_book, FALSE, FALSE, 0);

 
    GtkWidget *member_label = gtk_label_new("Add Member:");
    gtk_box_pack_start(GTK_BOX(vbox), member_label, FALSE, FALSE, 0);

    entry_member_name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_member_name), "Member Name");
    gtk_box_pack_start(GTK_BOX(vbox), entry_member_name, FALSE, FALSE, 0);

    entry_member_email = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_member_email), "Member Email");
    gtk_box_pack_start(GTK_BOX(vbox), entry_member_email, FALSE, FALSE, 0);

    GtkWidget *button_add_member = gtk_button_new_with_label("Add Member");
    g_signal_connect(button_add_member, "clicked", G_CALLBACK(on_button_add_member_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button_add_member, FALSE, FALSE, 0);

   
    label_result = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), label_result, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
