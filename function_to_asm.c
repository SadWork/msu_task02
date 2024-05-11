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
const int num_constants   = sizeof(constants) / sizeof(constants[0]);

void make_asm(char *s) {}

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
    while (size && *input != '\n' && *input)
    {
        *s = *input;
        ++s;
        ++input;
        --size;
    }
    *s = 0;

    if (*input != 0)
        return input + 1;
    return NULL;
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

        // Пример вывод строки uoperation
        char *next = uoperations[num_uoperations - 1].translation;
        while ((next = fsgets(s, BUF_SIZE, next)) != NULL)
        {
            printf("%s\n", s);
        }
        make_asm(s);
    }

    fclose(file);

    return 0;
}