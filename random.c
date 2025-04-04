#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LENGTH 256

void load_file(const char *filename, char text[MAX_LINES][MAX_LENGTH], int *num_lines) {
    FILE *file = fopen(filename, "r");
    if (!file) return;

    *num_lines = 0;
    while (fgets(text[*num_lines], MAX_LENGTH, file) && *num_lines < MAX_LINES - 1) {
        text[*num_lines][strcspn(text[*num_lines], "\n")] = '\0'; // Eliminar salto de línea
        (*num_lines)++;
    }
    fclose(file);
}

void save_file(const char *filename, char text[MAX_LINES][MAX_LENGTH], int num_lines) {
    FILE *file = fopen(filename, "w");
    if (!file) return;

    for (int i = 0; i < num_lines; i++) {
        fprintf(file, "%s\n", text[i]);
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    char text[MAX_LINES][MAX_LENGTH] = {0};
    int num_lines = 0;
    load_file(argv[1], text, &num_lines);

    initscr();
    noecho();
    keypad(stdscr, TRUE);

    int row = 0;
    int col = 0;
    int ch;

    while ((ch = getch()) != 27) { // Presionar ESC para salir
        switch (ch) {
            case KEY_UP:
                if (row > 0) row--;
                break;
            case KEY_DOWN:
                if (row < num_lines - 1) row++;
                break;
            case KEY_BACKSPACE:
                if (col > 0) {
                    text[row][--col] = '\0';
                }
                break;
            default:
                if (col < MAX_LENGTH - 1) {
                    text[row][col++] = ch;
                    text[row][col] = '\0';
                }
        }
        clear();
        for (int i = 0; i < num_lines; i++) {
            mvprintw(i, 0, "%s", text[i]);
        }
        move(row, col);
        refresh();
    }

    save_file(argv[1], text, num_lines);
    endwin();
    return 0;
}
