#include <stdio.h>
#include <stdlib.h>
#include "../../include/utils/colors.h"

char *get_ansi_color_scheme(COLORS color) {
  if (color == COLORS_RED) {
    return ANSI_COLOR_RED;
  } else if (color == COLORS_GREEN) {
    return ANSI_COLOR_GREEN;
  } else if (color == COLORS_YELLOW) {
    return ANSI_COLOR_YELLOW;
  }  else if (color == COLORS_BLUE) {
    return ANSI_COLOR_BLUE;
  } else if (color == COLORS_MAGENTA) {
    return ANSI_COLOR_MAGENTA;
  } else if (color == COLORS_CYAN) {
    return ANSI_COLOR_CYAN;
  } else if (color == COLORS_RESET) {
    return ANSI_COLOR_RESET;
  }
  return NULL;
}

void print_colored(COLORS color, char *message) {
  printf("%s%s", get_ansi_color_scheme(color), message);
}
