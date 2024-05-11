#include "operation_tree.c"

#include <math.h>
#include <stdio.h>
#include <string.h>

#define EPS      1e-3
#define BUF_SIZE 2000
#define DEBUG
#ifndef M_E
    #define M_E 2.7182818284590452354
#endif
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

typedef struct
{
    const char *name;
    double value;
} Constant;

Constant constants[] = {
    { "e",  M_E},
    {"pi", M_PI}
};
const int num_constants = sizeof(constants) / sizeof(constants[0]);

typedef struct
{
    char *name;
    char *translation;
} Uoperation;

Uoperation uoperations[] = {
    {"sin",                      "fsin"},
    {"cos",                      "fcos"},
    {"tan",           "fptan\nfstp st0"},
    {"ctg", "fptan\nfdiv st1\nfstp st1"}
};
const int num_uoperations = sizeof(uoperations) / sizeof(uoperations[0]);

typedef struct
{
    char *name;
    char *translation;
} Boperation;
Boperation boperations[] = {
    { "/",  "fdiv"},
    {"\\",  "fdiv"},
    { "+", "faddp"},
    { "*", "fmulp"},
    { "-", "fsubp"}
};
const int num_boperations = sizeof(boperations) / sizeof(boperations[0]);

int string_is_empty(char *s)
{
    while (*s != 0)
    {
        if (*s != ' ' && *s != '\n')
        {
            return 0;
        }
        ++s;
    }
    return 1;
}

char *fsgets(char *s, int size, char *input)
{
    if (!*input)
    {
        return NULL;
    }
    while (size && *input != '\n' && *input)
    {
        *s = *input;
        ++s;
        ++input;
        --size;
    }
    *s = 0;

    if (*input)
        return input + 1;
    return input;
}

void make_asm(char *s)
{
    Node *root = create_node();
    char sub_s[BUF_SIZE];
    while (*s)
    {
        sscanf(s, "%s", sub_s);
        s += strlen(sub_s);
        while (*s == ' ' || *s == '\n')
        {
            ++s;
        }

        int not_found = 1;
        if (!strcmp(sub_s, "x"))
        {
            printf("Это x\n");
            not_found = 0;
        }
        else
        {
            for (int i = 0; i < num_uoperations; ++i)
            {
                if (!strcmp(sub_s, uoperations[i].name))
                {
                    char *next = uoperations[i].translation;
                    while ((next = fsgets(sub_s, BUF_SIZE, next)) != NULL)
                    {
                        printf("%s\n", sub_s);
                    }
                    not_found = 0;
                    break;
                }
            }
            if (not_found)
            {
                for (int i = 0; i < num_boperations; ++i)
                {
                    if (!strcmp(sub_s, boperations[i].name))
                    {
                        char *next = boperations[i].translation;
                        while ((next = fsgets(sub_s, BUF_SIZE, next)) != NULL)
                        {
                            printf("%s\n", sub_s);
                        }
                        not_found = 0;
                        break;
                    }
                }

                if (not_found)
                {
                    for (int i = 0; i < num_constants; ++i)
                    {
                        if (!strcmp(sub_s, constants[i].name))
                        {
                            printf("%lf\n", constants[i].value);
                            not_found = 0;
                            break;
                        }
                    }
                    if (not_found)
                    {
                        double value;
                        sscanf(sub_s, "%lf", &value);
                        printf("Это число! %lf\n", value);
                        not_found = 0;
                    }
                }
            }
        }
    }
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
                printf("Ошибка: отсутствует путь к входному файлу после флага -i/--input\n");
                return 1;
            }
        }
    }
    if (input_file == NULL)
    {
        printf("Ошибка: отсутствует путь к входному файлу\n");
        return 1;
    }
#else
    const char *input_file = ".debug/spec.txt";
#endif

    FILE *file = fopen(input_file, "r");

    if (file == NULL)
    {
        printf("Не удалось открыть файл %s\n", input_file);
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