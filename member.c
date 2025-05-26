#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/library.h"

int add_member(Member member) {
    FILE *file = fopen(MEMBERS_FILE, "a");
    if (!file) return 1;
    fwrite(&member, sizeof(Member), 1, file);
    fclose(file);
    return 0;
}

int read_members(Member **members, int *count) {
    FILE *file = fopen(MEMBERS_FILE, "r");
    if (!file) return 1;

    *count = 0;
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    *count = size / sizeof(Member);
    rewind(file);

    *members = malloc(size);
    if (!*members) {
        fclose(file);
        return 1;
    }

    fread(*members, sizeof(Member), *count, file);
    fclose(file);
    return 0;
}