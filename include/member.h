#ifndef MEMBER_H
#define MEMBER_H

typedef struct {
    int member_id;
    char name[255];
    char email[255];
    char date_joined[11]; 
} Member;

int add_member(Member member);
int read_members(Member **members, int *count);

#endif