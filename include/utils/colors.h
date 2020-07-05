/*! @file colors.h
  * @brief macros and utilities for printing color to stdout
  * from https://www.quora.com/How-do-I-print-a-colored-output-in-C
  * Pass a `COLORS_x` enum value into `print_colored` and the color will be printed on stdout
  * Note that if you want to disable printing of that color you'll have to send the `COLORS_RESET` enum value through
*/

#pragma once
// to make non-bol change 0->1 (0;31m red) vs (1;31m bold red)
// 
// to use 256bit the following syntax
#define ANSI_COLOR_RED "\x1b[1;31m"
#define ANSI_COLOR_SOFT_RED "\x1b[1;38;5;210m"
#define ANSI_COLOR_GREEN "\x1b[1;32m"
#define ANSI_COLOR_YELLOW "\x1b[1;33m"
#define ANSI_COLOR_BLUE "\x1b[1;34m"
#define ANSI_COLOR_MAGENTA "\x1b[1;35m"
#define ANSI_COLOR_CYAN "\x1b[1;36m"
#define ANSI_COLOR_RESET "\x1b[1;0m"

typedef enum {
  COLORS_RED, COLORS_SOFT_RED, COLORS_GREEN, COLORS_YELLOW, COLORS_BLUE, COLORS_MAGENTA, COLORS_CYAN, COLORS_RESET
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