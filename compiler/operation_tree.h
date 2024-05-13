#ifndef OPERATION_TREE_H
#define OPERATION_TREE_H
#include "../lib/macro.h"

#include <stdlib.h>
typedef struct
{
    int height;
    int number_childs;
    int max_childs;
    struct Node **childs;
    char *value;
    char spec;
} Node;

Node *node_create(void);
void node_link_nodes(Node *parent, Node *child);
void operation_tree_free(Node *root);
void operation_tree_print(Node *root, const char *indent);
void operation_tree_to_asm(Node *root);
#endif