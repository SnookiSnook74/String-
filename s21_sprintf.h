#ifndef S21_SPRINTF_
#define S21_SPRINTF_

#include "s21_string.h"
#define BUFFER_SIZE 4048

typedef struct {
  /* Определяет выравнивание результата форматирования.
  Если значение равно true, результат будет выравнен по левому краю, в
  противном случае - по правому краю. Соответствует флагу '-'.*/
  bool minus;
  /* Определяет, следует ли всегда отображать знак для чисел со знаком.
  Если значение равно true, знак числа будет отображаться даже для положительных
  чисел. Соответствует флагу '+'*/
  bool plus;
  /* Определяет, следует ли вставлять пробел перед положительными числами и
  перед нулем. Если значение равно true, пробел будет вставлен. Соответствует
  флагу ' ' (пробел).*/
  bool space;
  /* Определяет, следует ли использовать альтернативную форму представления для
  некоторых спецификаторов. Например, для 'x' и 'X' это означает добавление
  префикса "0x" или "0X" перед числом. Соответствует флагу '#'.*/
  bool hash;
  /* Определяет, следует ли дополнять результат форматирования нулями слева,
  чтобы получить указанную ширину. Соответствует флагу '0'.*/
  bool zero;
  /* Определяет минимальную ширину результата форматирования.
  Если результат меньше указанной ширины, он будет дополнен пробелами или нулями
  (в зависимости от флага zero).*/
  int width;
  /* Определяет количество знаков после десятичной точки для чисел с плавающей
   точкой или максимальное количество символов для вывода строк. Для целых чисел
   определяет минимальное количество цифр для вывода.*/
  int precision;
  /*  Определяет модификатор длины для спецификатора формата. Может быть 'h',
   'l' или 'L', что указывает на короткий, длинный или длинный двойной тип
   соответственно.*/
  char lenght;
  /* Cодержит спецификатор формата, который определяет, как должен быть
     отформатирован аргумент. Может быть одним из следующих символов: 'c', 'd',
    'i', 'f', 's', 'u', '%', 'g', 'G', 'e', 'E', 'x', 'X', 'o', 'n', 'p'.*/
  char specifier;
  bool pointer;
} specifier;

int s21_sprintf(char *str, const char *format, ...);
void format_char(char **buf_ptr, int ch, specifier *spec);
bool parse_format_specifier(const char **format, specifier *spec,
                            va_list *args);
void format_int(char **buf_ptr, long long value, specifier *spec);
void format_string(char **buf_ptr, const char *str, specifier *spec);

void format_float(char **buf_ptr, long double value, specifier *spec);
void reverse_string(char *str, int length);
void float_to_string(char *buffer, long double value, int precision,
                     specifier *spec);

void format_uint(char **buf_ptr, unsigned long int value, specifier *spec);

void format_sci_notation(char **buf_ptr, long double value, specifier *spec);

void format_g(char **buf_ptr, long double value, specifier *spec);

void format_hex(char **buf_ptr, unsigned long long value, specifier *spec);

void format_uint_octal(char **buf_ptr, unsigned long long value,
                       specifier *spec);

void format_pointer(char **buf_ptr, const void *ptr, specifier *spec);

long long apply_lenght_modifier_sing(long long value, char lenght);
unsigned long long apply_lenght_modifier_unsigned(unsigned long long value,
                                                  char lenght);
long double apply_float_lenght_modifier(long double value, char lenght);
void add_padding(char **buf_ptr, int padding);

#endif  // S21_SPRINTF_