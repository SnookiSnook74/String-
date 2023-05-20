#ifndef S21_SSCANF_H
#define S21_SSCANF_H

#include "s21_string.h"

struct Options {
  int width;
  int num_of_chars_readed;
  bool is_h;
  bool is_l;
  bool is_L;
  bool is_ignore_data;
};

bool _is_decimal(char c);
bool _is_hexal(char c);
bool _is_octal(char c);
int s21_sscanf(char *str, char *format, ...);
int _get_hex_int(char **str, struct Options options);
int _get_oct_int(char **str, struct Options options);
void _process_n(char *start, char *end, va_list *args);
void _set_options(struct Options *options, char **format);
int _get_int(int *num, char **str, struct Options options);
long int _get_oct_long_int(char **str, struct Options options);
long int _get_hex_long_int(char **str, struct Options options);
short int _get_hex_short_int(char **str, struct Options options);
short int _get_oct_short_int(char **str, struct Options options);
int _process_f(char **str, struct Options options, va_list *args);
int _process_s(char **str, struct Options options, va_list *args);
int _process_u(char **str, struct Options options, va_list *args);
int _process_x(char **str, struct Options options, va_list *args);
int _process_p(char **str, struct Options options, va_list *args);
int _process_i(char **str, struct Options options, va_list *args);
int _process_c(char **str, struct Options options, va_list *args);
int _process_d(char **str, struct Options options, va_list *args);
int _process_o(char **str, struct Options options, va_list *args);
unsigned int _get_unsigned_int(char **str, struct Options options);
void _parse_double(char *str, struct Options options, va_list *args);
int _get_long_int(long int *num, char **str, struct Options options);
unsigned int _get_unsigned_oct_int(char **str, struct Options options);
unsigned int _get_unsigned_hex_int(char **str, struct Options options);
int _get_short_int(short int *num, char **str, struct Options options);
unsigned long int _get_unsigned_long_int(char **str, struct Options options);
unsigned short int _get_unsigned_short_int(char **str, struct Options options);
unsigned long int _get_unsigned_hex_long_int(char **str,
                                             struct Options options);
unsigned long int _get_unsigned_oct_long_int(char **str,
                                             struct Options options);
unsigned short int _get_unsigned_oct_short_int(char **str,
                                               struct Options options);
unsigned short int _get_unsigned_hex_short_int(char **str,
                                               struct Options options);
int _parse_octal(char **str, struct Options options, va_list *args,
                 bool is_positive);
int _parse_heximal(char **str, struct Options options, va_list *args,
                   bool is_positive);
int _parse_decimal(char **str, struct Options options, va_list *args,
                   bool is_positive);
int _parse_unsigned_octal(char **str, struct Options options, va_list *args,
                          bool is_positive);
int _parse_unsigned_heximal(char **str, struct Options options, va_list *args,
                            bool is_positive);
unsigned int _parse_unsigned_decimal(char **str, struct Options options,
                                     va_list *args, bool is_positive);
#endif