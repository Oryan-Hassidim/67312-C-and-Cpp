#ifndef _EMPLOYEE_LINKED_LIST_EXAMPLE_H_

#define _EMPLOYEE_LINKED_LIST_EXAMPLE_H_
#define MAX_LINE 1024
#define SUCCESS 0
#define INVALID_ARGUMENT 1
#define ALLOCATION_ERROR 2
#define FILE_OPEN_ERROR 3
#define ID_PARSE_ERROR 4
#define FIRST_NAME_PARSE_ERROR 5
#define LAST_NAME_PARSE_ERROR 6
#define EMAIL_PARSE_ERROR 7
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Employee {
    char *first_name;
    char *last_name;
    char *email;
    int id;
} Employee;

typedef struct Node {
    struct Node *next;
    struct Employee data;
} Node;

typedef struct LinkedList {
    struct Node *head;
} LinkedList;

LinkedList *new_linked_list (void);
int push (LinkedList *list, const Employee *e);
const Employee *find (LinkedList *list, const Employee *e);
int fill_linked_list (LinkedList *list, const char *filename);
void free_linked_list (LinkedList *list);

#endif //_EMPLOYEE_LINKED_LIST_EXAMPLE_H_
