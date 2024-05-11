#include "operation_tree.h"

typedef struct
{
    int height;
    int number_childs;
    int max_childs;
    struct Node **childs;
    char *value;
} Node;

Node *create_node(void)
{
    Node *new_node = (Node *)malloc(sizeof(Node));

    new_node->height        = 1;
    new_node->number_childs = 0;
    new_node->max_childs    = 0;

    new_node->value  = NULL;
    new_node->childs = NULL;
    return new_node;
}

void link_nodes(Node *parent, Node *child)
{
    if (parent->max_childs + 1 >= parent->number_childs)
    {
        Node **new_childs_area = (Node **)realloc(parent->childs, 2 * parent->max_childs * sizeof(parent->childs[0]));
        if (new_childs_area != NULL)
        {
            parent->childs = (struct Node **)new_childs_area;
            parent->max_childs *= 2;
        }
        // TODO - обработка ошибки realloc
    }
    parent->childs[parent->number_childs] = (struct Node *)child;
    (parent->number_childs)++;
}

void operation_tree_free(Node *root)
{
    // Предполагается что дерево корректно => number_childs = количество не нулевых детей
    while ((root->number_childs)--)
    {
        operation_tree_free((Node *)(root->childs[root->number_childs]));
    }
    free(root->childs);
    free(root);
}
