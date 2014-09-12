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
        puts("No input string");
        exit(1);
    }
}

void string_to_file(char *str, FILE *file)
{
    fprintf(file, "%s", str);
}

void print_sorted_string(char *str)
{
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);

    attron(COLOR_PAIR(1));
    printw("Sorted string: %s\n", str);
    attron(COLOR_PAIR(2));
    printw("Also available in output file\n");
    refresh();

    getchar();
    endwin();
}



int main(int argc, char const *argv[])
{
    printf(" *** PID: %d\n", getpid());
    check_params(argv);

    char *str = calloc(strlen(argv[1]), sizeof(argv[1][0]));
    FILE *file = fopen("output", "w");

    strcpy(str, argv[1]);
    qsort(str, strlen(argv[1]), sizeof(*argv[1]), compare);

    string_to_file(str, file);
    print_sorted_string(str);
    fclose(file);
    free(str);
    return 0;
}
