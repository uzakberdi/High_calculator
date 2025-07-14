#ifndef DISPLAY_H
#define DISPLAY_H

#define DISPLAY_WIDTH 50
#define PLOT_WIDTH 60
#define PLOT_HEIGHT 20
#define PLOT_POINT '*'
#define PLOT_LINE '-'
#define PLOT_VERTICAL '|'
#define PLOT_AXIS '+'
#define PLOT_GRID '.'

void clear_screen();
void pause();
void show_title();
void show_result(const char *operation, double result);
void print_menu();

#endif
