#include "constants.h"
#include "cvector.c"
#include "macro.h"
#include "operation_tree.c"
#include "tools.c"

#include <math.h>
#include <stdio.h>
#include <string.h>

#define EPS      1e-3
#define BUF_SIZE 2000

void make_asm(char *s)
{
    /*
        TODO сейчас всё работает на предположении, что strlen(s) < BUF_SIZE,
        но можно было бы реализовать для строки переменной длины.

        TODO в функции не хватает логики построения дерева операций
    */
    Node *root = create_node();

    cvector nodes_vec;
    Node **nodes = (Node **)cvector_init(&nodes_vec, sizeof(Node *));

    cvector substr_vec;
    cvector_init(&substr_vec, sizeof(char));
    char *substr = (char *)cvector_resize(&substr_vec, BUF_SIZE);

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
                Node *new_node = create_node();

                new_node->value = (char *)malloc(sizeof("fld qword [ebp + 8]"));
                strcpy(new_node->value, "fld qword [ebp + 8]");

                nodes = (Node **)cvector_push_back(&nodes_vec, &new_node);
                printf("Это x\n");
                not_found = 0;
                break;
            }

            for (int i = 0; i < num_uoperations; ++i)
            {
                if (!strcmp(substr, uoperations[i].name))
                {
                    char *next = uoperations[i].translation;
                    while ((next = fsgets(substr, substr_vec.size, next)) != NULL)
                    {
                        printf("%s\n", substr);
                    }
                    not_found = 0;
                    break;
                }
            }
            break_if(!not_found);

            for (int i = 0; i < num_boperations; ++i)
            {
                if (!strcmp(substr, boperations[i].name))
                {
                    char *next = boperations[i].translation;
                    while ((next = fsgets(substr, substr_vec.size, next)) != NULL)
                    {
                        printf("%s\n", substr);
                    }
                    not_found = 0;
                    break;
                }
            }
            break_if(!not_found);

            for (int i = 0; i < num_constants; ++i)
            {
                if (!strcmp(substr, constants[i].name))
                {
                    printf("%lf\n", constants[i].value);
                    not_found = 0;
                    break;
                }
            }
            break_if(!not_found);

            double value;
            sscanf(substr, "%lf", &value);
            printf("Это число! %lf\n", value);
        } while (not_found = 0);
    }

    cvector is_cleaned_vec;
    int *is_cleaned = cvector_init(&is_cleaned_vec, sizeof(int));
    is_cleaned      = cvector_resize(&is_cleaned_vec, nodes_vec.size);
    memset(is_cleaned, 0, is_cleaned_vec.size * sizeof(int));

    for (int i = 0; i < nodes_vec.size; ++i)
    {
        if (!is_cleaned[i])
        {
            operation_tree_free(nodes[i]);
        }
    }
    substr     = (char *)cvector_free(&nodes_vec);
    nodes      = (Node **)cvector_free(&substr_vec);
    is_cleaned = (int *)cvector_free(&is_cleaned_vec);
    operation_tree_free(root);
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