#include "constants.h"
#include "cvector.c"
#include "macro.h"
#include "operation_tree.c"
#include "tools.c"

#include <math.h>
#include <stdio.h>
#include <string.h>

#define EPS                             1e-3
#define BUF_SIZE                        2000
#define MAX_LEN_INDEX_CONST_DECLARATION 15

void make_asm(char *s)
{
    /*
        TODO сейчас всё работает на предположении, что strlen(s) < BUF_SIZE,
        но можно было бы реализовать для строки переменной длины.
    */
    cvector section_data_vec;
    double *section_data = (double *)cvector_init(&section_data_vec, sizeof(double *));

    cvector nodes_vec;
    Node **nodes = (Node **)cvector_init(&nodes_vec, sizeof(Node *));

    cvector substr_vec;
    cvector_init(&substr_vec, sizeof(char));
    char *substr = (char *)cvector_resize(&substr_vec, BUF_SIZE);

    char int2str_buffer[MAX_LEN_INDEX_CONST_DECLARATION];
    int num_section_data_items = 0;
    while (*s)
    {
        sscanf(s, "%s", substr);
        s += strlen(substr);
        while (*s == ' ' || *s == '\n')
        {
            ++s;
        }

        int not_found = 1;

        do
        {
            if (!strcmp(substr, "x"))
            {
                Node *new_node = node_create();

                new_node->value = (char *)malloc(sizeof("fld qword [ebp + 8]"));
                strcpy(new_node->value, "fld qword [ebp + 8]");

                new_node->spec |= IS_COMMUTATIVE;
                nodes     = (Node **)cvector_push_back(&nodes_vec, &new_node);
                not_found = 0;
                break;
            }

            for (int i = 0; i < num_uoperations; ++i)
            {
                if (!strcmp(substr, uoperations[i].name))
                {
                    if (!nodes_vec.size)
                    {
                        ERROR_MSG("Empty stack. No operands for unary operation!\n");
                        exit(1);
                    }

                    Node *new_node = node_create();
                    node_link_nodes(new_node, nodes[nodes_vec.size - 1]);

                    int len         = strlen(uoperations[i].translation) + 1;
                    new_node->value = (char *)malloc(len * sizeof(char));
                    strcpy(new_node->value, uoperations[i].translation);

                    nodes[nodes_vec.size - 1] = new_node;

                    // char *next = uoperations[i].translation;
                    // while ((next = fsgets(substr, substr_vec.size, next)) != NULL)
                    // {
                    //     printf("%s\n", substr);
                    // }
                    not_found = 0;
                    break;
                }
            }
            break_if(!not_found);

            for (int i = 0; i < num_boperations; ++i)
            {
                if (!strcmp(substr, boperations[i].name))
                {
                    if (nodes_vec.size < 2)
                    {
                        ERROR_MSG("Empty stack. No operands for binary operation!\n");
                        exit(1);
                    }
                    Node *new_node = node_create();
                    node_link_nodes(new_node, nodes[nodes_vec.size - 2]);
                    node_link_nodes(new_node, nodes[nodes_vec.size - 1]);
                    nodes = cvector_pop(&nodes_vec);

                    int len         = strlen(boperations[i].translation) + 1;
                    new_node->value = (char *)malloc(len * sizeof(char));
                    strcpy(new_node->value, boperations[i].translation);

                    new_node->spec            = boperations[i].is_commutative;
                    nodes[nodes_vec.size - 1] = new_node;
                    // char *next = boperations[i].translation;
                    // while ((next = fsgets(substr, substr_vec.size, next)) != NULL)
                    // {
                    //     printf("%s\n", substr);
                    // }
                    not_found = 0;
                    break;
                }
            }
            break_if(!not_found);

            for (int i = 0; i < num_constants; ++i)
            {
                if (!strcmp(substr, constants[i].name))
                {
                    double value = constants[i].value;
                    section_data = (double *)cvector_push_back(&section_data_vec, &value);

                    sprintf(int2str_buffer, "%i", num_section_data_items);
                    int int2str_len = strlen(int2str_buffer);

                    Node *new_node = node_create();

                    new_node->value = (char *)malloc(sizeof("fld qword[constant]") + int2str_len);
                    sprintf(new_node->value, "fld qword[constant%s]", int2str_buffer);

                    nodes = (Node **)cvector_push_back(&nodes_vec, &new_node);

                    ++num_section_data_items;
                    not_found = 0;
                    break;
                }
            }
            break_if(!not_found);

            double value;
            sscanf(substr, "%lf", &value);
            section_data = (double *)cvector_push_back(&section_data_vec, &value);

            sprintf(int2str_buffer, "%i", num_section_data_items);
            int int2str_len = strlen(int2str_buffer);

            Node *new_node = node_create();

            new_node->value = (char *)malloc(sizeof("fld qword[constant]") + int2str_len);
            sprintf(new_node->value, "fld qword[constant%s]", int2str_buffer);

            nodes = (Node **)cvector_push_back(&nodes_vec, &new_node);
            ++num_section_data_items;

        } while (not_found = 0);
    }
    operation_tree_to_asm(nodes[0]);
    operation_tree_print(nodes[0]);
    printf("\n");
    for (int i = 0; i < nodes_vec.size; ++i)
    {
        operation_tree_free(nodes[i]);
    }
    substr       = (char *)cvector_free(&nodes_vec);
    section_data = (double *)cvector_free(&section_data_vec);
    nodes        = (Node **)cvector_free(&substr_vec);
}

int main(int argc, char *argv[])
{
#ifndef DEBUG
    char *input_file = NULL;

    // Проверка аргументов командной строки
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0)
        {
            // Следующий аргумент после флага должен быть путем к файлу
            if (i + 1 < argc)
            {
                input_file = argv[i + 1];
            }
            else
            {
                ERROR_MSG("Ошибка: отсутствует путь к входному файлу после флага -i/--input\n");
                return 1;
            }
        }
    }
    if (input_file == NULL)
    {
        ERROR_MSG("Ошибка: отсутствует путь к входному файлу\n");
        return 1;
    }
#else
    const char *input_file = ".debug/spec.txt";
#endif

    FILE *file = fopen(input_file, "r");

    if (file == NULL)
    {
        ERROR_MSG("Не удалось открыть файл %s\n", input_file);
        return 1;
    }

    double l, r;
    fscanf(file, "%lf%lf", &l, &r);

    // Основной цикл
    char s[BUF_SIZE];
    while (fgets(s, BUF_SIZE, file) != NULL)
    {
        if (string_is_empty(s))
        {
            continue;
        }
        make_asm(s);
    }

    fclose(file);

    return 0;
}