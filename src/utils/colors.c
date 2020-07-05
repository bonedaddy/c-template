#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../include/utils/colors.h"

char *get_ansi_color_scheme(COLORS color) {
  switch (color) {
        case COLORS_RED:
            return ANSI_COLOR_RED;
        case COLORS_SOFT_RED:
            return ANSI_COLOR_SOFT_RED;
        case COLORS_GREEN:
            return ANSI_COLOR_GREEN;
        case COLORS_YELLOW:
            return ANSI_COLOR_YELLOW;
        case COLORS_BLUE:
            return ANSI_COLOR_BLUE;
        case COLORS_MAGENTA:
            return ANSI_COLOR_MAGENTA;
        case COLORS_CYAN:
            return ANSI_COLOR_CYAN;
        case COLORS_RESET:
            return ANSI_COLOR_RESET;
        default:
            return NULL;
  }
}

char *format_colored(COLORS color, char *message) {
  char *pcolor = get_ansi_color_scheme(color);
  char *formatted = malloc(sizeof(message) + sizeof(pcolor));
  strcat(formatted, pcolor);
  strcat(formatted, message);
  return formatted;
}

void print_colored(COLORS color, char *message) {
  printf("%s%s%s\n", get_ansi_color_scheme(color), message, ANSI_COLOR_RESET);
}

int write_colored(FILE *fh, COLORS color, char *message, bool do_flush) {
    char *pcolor = get_ansi_color_scheme(color);
    char *reset = get_ansi_color_scheme(COLORS_RESET);
    // 2 for \n
    char *write_message = malloc(strlen(pcolor) + strlen(reset) + strlen(message) + 2);
    strcat(write_message, pcolor);
    strcat(write_message, message);
    strcat(write_message, reset);
    strcat(write_message, "\n");
    int response = fputs(write_message, fh);
    if (response != 1) {
        printf("failed to call fputs: %i\n", response);
        free(write_message);
        return 1; // return 1 as we return 0 if there was no error
    }
    // FILE* likely buffers internally so this likely doesnt cause any issues
    // if you want to be safe set do_flush to true, but this will increase disk activity
    if (do_flush == false) {
        free(write_message);
        return response;
    }
    // TODO(bonedaddy): should we buffer logs?
    response = fflush(fh);
    if (response != 0) {
        printf("failed to call fflush\n");
    }
    free(write_message);
    return response;
}