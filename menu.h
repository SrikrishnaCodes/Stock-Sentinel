#ifndef MENU_H
#define MENU_H

// defince color codes for terminal output (ANSI escape codes)
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define RESET   "\033[0m"

int get_choice(int);
int get_int(void);

#endif