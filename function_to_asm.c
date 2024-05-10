#include <math.h>
#include <stdio.h>
#include <string.h>

#define EPS      1e-3
#define BUF_SIZE 2000
#define DEBUG

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

    char s[BUF_SIZE];
    while (fgets(s, BUF_SIZE, file) != NULL) {}

    fclose(file);

    return 0;
}