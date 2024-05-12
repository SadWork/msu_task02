#include "operation_tree.h"

typedef struct
{
    int height;
    int number_childs;
    int max_childs;
    struct Node **childs;
    char *value;
    char spec;
} Node;

Node *node_create(void)
{
    Node *new_node = (Node *)malloc(sizeof(Node));

    new_node->height        = 1;
    new_node->number_childs = 0;
    new_node->max_childs    = 0;
    new_node->spec          = 0;

    new_node->value  = NULL;
    new_node->childs = NULL;
    return new_node;
}

void node_link_nodes(Node *parent, Node *child)
{
    if (parent->number_childs == parent->max_childs)
    {
        if (!parent->max_childs)
        {
            parent->max_childs++;
        }
        Node **new_childs_area = (Node **)realloc(parent->childs, 2 * parent->max_childs * sizeof(parent->childs[0]));
        if (new_childs_area != NULL)
        {
            parent->childs = (struct Node **)new_childs_area;
            parent->max_childs *= 2;
        }
        // TODO - обработка ошибки realloc
    }
    parent->childs[parent->number_childs] = (struct Node *)child;
    parent->number_childs++;
}

void operation_tree_free(Node *root)
{
    // Предполагается что дерево корректно => number_childs = количество не нулевых детей
    while ((root->number_childs)--)
    {
        operation_tree_free((Node *)(root->childs[root->number_childs]));
    }
    free(root->childs);
    free(root->value);
    free(root);
}

void operation_tree_print(Node *root)
{
    if (CHECK_OVERFLOW(root->spec))
    {
        operation_tree_print((Node *)(root->childs[1]));
        printf("fstp qword[tmp]\n");
        operation_tree_print((Node *)(root->childs[0]));
        printf("fld qword[tmp]\n%s\n", root->value);
        return;
    }

    if (SKEWED_RIGHT(root->spec))
    {
        for (int i = root->number_childs - 1; i >= 0; --i)
        {
            operation_tree_print((Node *)(root->childs[i]));
        }
        if (!CHECK_COMMUTATIVE(root->spec))
            printf("fxch st0, st1\n");
    }
    else
    {
        for (int i = 0; i < root->number_childs; ++i)
        {
            operation_tree_print((Node *)(root->childs[i]));
        }
    }

    printf("%s\n", root->value);
}

void operation_tree_to_asm(Node *root)
{
    // Учитывает высоты поддеревьев.
    // TODO сейчас работает только в предположении num_childs <= 2

    if (root->number_childs == 0)
    {
        root->height = 1;
        return;
    }

    int max_height, min_height, height;
    max_height = INT_MIN;
    min_height = INT_MAX;

    for (int i = 0; i < root->number_childs; ++i)
    {
        operation_tree_to_asm((Node *)(root->childs[i]));
        height = ((Node *)(root->childs[i]))->height;

        max_height = max_height < height ? height : max_height;
        min_height = min_height > height ? height : min_height;
    }

    height = max_height + (min_height == max_height);
    if (height > X87_REGISTERS_COUNT)
    {
        height = X87_REGISTERS_COUNT;
        SET_OVERFLOW(root->spec);
    }
    if (root->number_childs == 2)
    {
        int height0 = ((Node *)(root->childs[0]))->height;
        int height1 = ((Node *)(root->childs[1]))->height;
        if (height0 < height1)
        {
            SET_SKEWED_RIGHT(root->spec);
        }
    }
    root->height = height;
}