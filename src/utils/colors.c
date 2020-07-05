#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
