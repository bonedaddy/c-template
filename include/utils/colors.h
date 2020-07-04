/*! @file colors.h
  * @brief macros and utilities for printing color to stdout
  * from https://www.quora.com/How-do-I-print-a-colored-output-in-C
*/
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef enum {
  COLORS_RED, COLORS_GREEN, COLORS_YELLOW, COLORS_BLUE, COLORS_MAGENTA, COLORS_CYAN, COLORS_RESET
} COLORS;

/*! @brief returns a `char *` with the message formatted with ansi colors
*/
char *format_colored(COLORS color, char *message);
/*! @brief returns an ansi color string to be used with printf
*/
char *get_ansi_color_scheme(COLORS color);
/*! @brief prints message to stdout with the given color
*/
void print_colored(COLORS color, char *message);