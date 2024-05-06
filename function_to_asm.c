#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *output_file = NULL;

    // Проверка аргументов командной строки
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0)
        {
            // Следующий аргумент после флага должен быть путем к файлу
            if (i + 1 < argc)
            {
                output_file = argv[i + 1];
            }
            else
            {
                printf("Ошибка: отсутствует путь к входному файлу после флага -i/--input\n");
                return 1;
            }
        }
    }
    if (output_file == NULL)
    {
        printf("Ошибка: отсутствует путь к входному файлу\n");
        return 1;
    }

    FILE *file = fopen(output_file, "r");

    if (file == NULL)
    {
        printf("Не удалось открыть файл %s\n", output_file);
        return 1;
    }

    fclose(file);

    return 0;
}