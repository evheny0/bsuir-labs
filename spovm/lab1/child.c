#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>


int compare(const void *a, const void *b)
{
    return (*((char *)a) - *((char *)b));
}

void check_params(char const *argv[])
{
    if (!argv[1]) {
        puts("No input string\n");
        exit(1);
    }
}

void string_to_file(char *str, FILE *file)
{
    fprintf(file, "%s", str);
}



int main(int argc, char const *argv[])
{
    printf("*** PID: %d\n", getpid());
    check_params(argv);

    char *str = calloc(strlen(argv[1]), sizeof(argv[1][0]));
    FILE *file = fopen("output", "w");

    strcpy(str, argv[1]);
    qsort(str, strlen(argv[1]), sizeof(*argv[1]), compare);

    string_to_file(str, file);
    fclose(file);
    return 0;
}
