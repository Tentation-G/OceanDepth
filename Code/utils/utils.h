#ifndef UTILS_H
#define UTILS_H

int index_in_axis(char c, const char *axis);
char to_upper_ascii(char c);
int clamp(int valeur, int max);
int in_screen_lim(int y, int x);
int convert_to_percent(int val, int val_max);
char* convert_to_visual_bar(int stat, int stat_max);

void clear_stdin(void);
#endif // UTILS_H
