#include "s21_sprintf.h"

int s21_sprintf(char *buffer, const char *format, ...) {
#ifdef __APPLE__
  setlocale(LC_ALL, "");
#endif
  va_list args;
  va_start(args, format);
  char *buf_ptr = buffer;

  while (*format) {
    if (*format == '%') {
      format++;
      if (*format == '%') {
        *buf_ptr++ = *format++;
      } else {
        specifier spec = {0};
        if (parse_format_specifier(&format, &spec, &args)) {
          switch (spec.specifier) {
            case 'c':
              format_char(&buf_ptr, va_arg(args, int), &spec);
              break;
            case 'd':
            case 'i':
              format_int(&buf_ptr, va_arg(args, long long int), &spec);
              break;
            case 'u':
              if (spec.lenght == 'l') {
                format_uint(&buf_ptr, va_arg(args, unsigned long int), &spec);
              } else {
                format_uint(&buf_ptr, va_arg(args, unsigned int), &spec);
              }
              break;
            case 'f':
              if (spec.lenght == 'L') {
                format_float(&buf_ptr, va_arg(args, long double), &spec);
              } else {
                format_float(&buf_ptr, va_arg(args, double), &spec);
              }
              break;
            case 'g':
            case 'G':
              if (spec.lenght == 'L') {
                format_g(&buf_ptr, va_arg(args, long double), &spec);
              } else {
                format_g(&buf_ptr, va_arg(args, double), &spec);
              }
              break;
            case 'e':
            case 'E':
              if (spec.lenght == 'L') {
                format_sci_notation(&buf_ptr, va_arg(args, long double), &spec);
              } else {
                format_sci_notation(&buf_ptr, va_arg(args, double), &spec);
              }
              break;
            case 's':
              format_string(&buf_ptr, va_arg(args, const char *), &spec);
              break;
            case 'p':
              format_pointer(&buf_ptr, va_arg(args, const void *), &spec);
              break;
            case 'n':
              *(va_arg(args, int *)) = buf_ptr - buffer;
              break;
            case 'x':
            case 'X':
              format_hex(&buf_ptr, va_arg(args, unsigned int), &spec);
              break;
            case 'o':
              format_uint_octal(&buf_ptr, va_arg(args, unsigned int), &spec);
              break;
            default:
              break;
          }
        }
      }
    } else {
      *buf_ptr++ = *format++;
    }
  }

  *buf_ptr = '\0';
  va_end(args);
  return buf_ptr - buffer;
}

bool parse_format_specifier(const char **format, specifier *spec,
                            va_list *args) {
  bool flag_parsed = true;
  while (flag_parsed) {
    switch (**format) {
      case '-':
        spec->minus = true;
        ++*format;
        break;
      case '+':
        spec->plus = true;
        ++*format;
        break;
      case ' ':
        spec->space = true;
        ++*format;
        break;
      case '#':
        spec->hash = true;
        ++*format;
        break;
      case '0':
        spec->zero = true;
        ++*format;
        break;
      default:
        flag_parsed = false;
        break;
    }
  }
  if (**format == '*') {
    spec->width = va_arg(*args, int);
    ++*format;
  } else {
    for (spec->width = 0; **format >= '0' && **format <= '9'; ++*format) {
      spec->width = spec->width * 10 + (**format - '0');
    }
  }

  if (**format == '.') {
    ++*format;
    if (**format == '*') {
      spec->precision = va_arg(*args, int);
      ++*format;
    } else {
      for (spec->precision = 0; **format >= '0' && **format <= '9'; ++*format) {
        spec->precision = spec->precision * 10 + (**format - '0');
      }
    }
  }

  if (**format == 'h' || **format == 'l' || **format == 'L') {
    spec->lenght = **format;
    ++*format;
  }

  switch (**format) {
    case 'c':
    case 'd':
    case 'i':
    case 'f':
    case 's':
    case 'u':
    case '%':
    case 'g':
    case 'G':
    case 'e':
    case 'E':
    case 'x':
    case 'X':
    case 'o':
    case 'n':
    case 'p':
      spec->specifier = **format;
      ++*format;
      return true;
    default:
      return false;
  }
}
void format_char(char **buf_ptr, int ch, specifier *spec) {
  int width = spec->width > 0 ? spec->width : 1;
  int padding = width - 1;
  // Если флаг 'minus' не задан, добавляем пробелы перед символом
  if (!spec->minus) {
    add_padding(buf_ptr, padding);
  }
  // Записываем символ
  if (spec->lenght == 'l') {
    wint_t wide_char = (wint_t)ch;
    int mb_len = wcrtomb(*buf_ptr, wide_char, NULL);
    if (mb_len > 0) {
      *buf_ptr += mb_len;
    } else {
      // Обработка ошибок
      printf("Error converting wide character to multibyte\n");
      return;
    }
  } else {
    *(*buf_ptr)++ = (char)ch;
  }
  // Если флаг 'minus' задан, добавляем пробелы после символа
  if (spec->minus) {
    add_padding(buf_ptr, padding);
  }
}

void format_string(char **buf_ptr, const char *str, specifier *spec) {
  int len;
  // Если флаг 'l' задан, мы обрабатываем широкую строку
  if (spec->lenght == 'l') {
    const wchar_t *wide_str = (const wchar_t *)str;
    mbstate_t mbs;
    s21_memset(&mbs, 0,
               sizeof(mbstate_t));  // инициализируем состояние конвертации
    // Если указана точность, учитываем её при подсчёте длины строки
    if (spec->precision >= 1) {
      len = 0;
      const wchar_t *temp = wide_str;
      while (*temp != L'\0' && len < spec->precision) {
        len += wcrtomb(NULL, *temp, &mbs);  // подсчитываем количество байтов
        temp++;
      }
    } else {
      // Если точность не указана, подсчитываем полную длину строки
      len = wcsrtombs(NULL, &wide_str, 0,
                      &mbs);  // подсчитываем количество байтов
    }
    // Вычисляем количество пробелов для заполнения
    int padding = spec->width > len ? spec->width - len : 0;
    // Если флаг 'minus' не задан, добавляем пробелы перед строкой
    if (!spec->minus) {
      add_padding(buf_ptr, padding);
    }
    // Копировать символы строки в выходной буфер, учитывая точность
    wcsrtombs(*buf_ptr, &wide_str, len, &mbs);
    *buf_ptr += len;
    // Если флаг 'minus' задан, добавляем пробелы после строки
    if (spec->minus) {
      add_padding(buf_ptr, padding);
    }
  } else {
    const char *narrow_str = (const char *)str;
    // Если указана точность, учитываем её при подсчёте длины строки
    if (spec->precision >= 1) {
      len = 0;
      const char *temp = narrow_str;
      while (*temp != '\0' && len < spec->precision) {
        len++;
        temp++;
      }
    } else {
      // Если точность не указана, подсчитываем полную длину строки
      len = s21_strlen(narrow_str);
    }
    // Вычисляем количество пробелов для заполнения
    int padding = spec->width > len ? spec->width - len : 0;
    // Если флаг 'minus' не задан, добавляем пробелы перед строкой
    if (!spec->minus) {
      add_padding(buf_ptr, padding);
    }
    // Копировать символы строки в выходной буфер, учитывая точность
    for (int i = 0; i < len; i++) {
      *(*buf_ptr)++ = *narrow_str++;
    }
    // Если флаг 'minus' задан, добавляем пробелы после строки
    if (spec->minus) {
      add_padding(buf_ptr, padding);
    }
  }
}

void format_int(char **buf_ptr, long long int value, specifier *spec) {
  value = apply_lenght_modifier_sing(value, spec->lenght);
  char temp[BUFFER_SIZE];
  int len = 0;
  int sign = (value < 0) ? -1 : 1;

  if (value == 0) {
    temp[len++] = '0';
  } else {
    while (value != 0) {
      temp[len++] = (char)('0' + sign * (value % 10));
      value /= 10;
    }
  }
  // Если указана точность, добавить нули для дополнения до необходимой длины
  while (len < spec->precision) {
    temp[len++] = '0';
  }
  // Если установлен флаг '0' и есть минимальная ширина, добавить нули для
  // дополнения до необходимой длины
  if (spec->zero && !spec->minus) {
    while (len + (spec->plus || spec->space || sign < 0) < spec->width) {
      temp[len++] = '0';
    }
  }
  // Добавить знак перед числом, если необходимо
  if (sign < 0) {
    spec->plus = false;
    spec->space = false;
    temp[len++] = '-';
  } else if (spec->plus) {
    spec->space = false;
    temp[len++] = '+';
  } else if (spec->space && (spec->specifier != 'e' && spec->specifier != 'E' &&
                             spec->specifier != 'g')) {
    temp[len++] = ' ';
  }
  // Учесть выравнивание и ширину
  if (!spec->minus && !spec->zero &&
      (spec->specifier != 'e' && spec->specifier != 'E' &&
       spec->specifier != 'g')) {
    while (len < spec->width) {
      temp[len++] = ' ';
    }
  }
  // Записать результат в выходной буфер в обратном порядке
  for (int i = len - 1; i >= 0; --i) {
    *(*buf_ptr)++ = temp[i];
  }
  // Дополнить пробелами справа, если необходимо
  if (spec->minus) {
    while (len++ < spec->width) {
      *(*buf_ptr)++ = ' ';
    }
  }
}

void reverse_string(char *str, int length) {
  int start = 0;
  int end = length - 1;

  while (start < end) {
    char temp;
    temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    start++;
    end--;
  }
}

void float_to_string(char *buffer, long double value, int precision,
                     specifier *spec) {
  long long int_part = (long long)value;
  long double fractional_part = fabsl(value - (long double)int_part);
  int index = 0;

  if (value < 0) {
    buffer[index++] = '-';
    int_part = -int_part;
  } else if (spec->space == true) {
    buffer[index++] = ' ';
  }

  do {
    buffer[index++] = int_part % 10 + '0';
    int_part /= 10;
  } while (int_part);

  int start_position = 0;
  if (value < 0) {
    start_position = 1;
  } else if (spec->space) {
    start_position = 1;
  }
  reverse_string(buffer + start_position, index - start_position);
  if (precision > 0) {
    buffer[index++] = '.';
    long double multiplier = powl(10, precision);
    unsigned long long rounded_fractional_part =
        (unsigned long long)(fractional_part * multiplier + 0.5L);
    for (int i = 0; i < precision; ++i) {
      buffer[index + precision - 1 - i] = rounded_fractional_part % 10 + '0';
      rounded_fractional_part /= 10;
    }
    index += precision;
  }
  buffer[index] = '\0';
}

void format_float(char **buf_ptr, long double value, specifier *spec) {
  value = apply_float_lenght_modifier(value, spec->lenght);
  int precision = (spec->precision <= 0) ? 6 : spec->precision;
  char num_str[BUFFER_SIZE];
  float_to_string(num_str, value, precision, spec);
  int num_str_len = s21_strlen(num_str);

  if (spec->width > num_str_len + spec->plus) {
    int padding = spec->width - num_str_len - spec->plus;
    if (spec->minus) {
      if (spec->plus) {
        *(*buf_ptr)++ = '+';
      }
      s21_strcpy(*buf_ptr, num_str);
      s21_memset(*buf_ptr + num_str_len, ' ', padding);
      *buf_ptr += spec->width - spec->plus;
    } else {
      s21_memset(*buf_ptr, spec->zero ? '0' : ' ', padding);
      if (spec->plus) {
        *(*buf_ptr + (spec->zero ? 0 : padding)) = '+';
      }
      s21_strcpy(*buf_ptr + padding + spec->plus, num_str);
      *buf_ptr += spec->width;
    }
  } else {
    if (spec->plus) {
      *(*buf_ptr)++ = '+';
    }
    s21_strcpy(*buf_ptr, num_str);
    *buf_ptr += num_str_len;
  }
}

void format_uint(char **buf_ptr, unsigned long int value, specifier *spec) {
  value = apply_lenght_modifier_unsigned(value, spec->lenght);
  char temp[BUFFER_SIZE];
  int len = 0;

  if (value == 0) {
    temp[len++] = '0';
  } else {
    while (value != 0) {
      temp[len++] = (char)('0' + value % 10);
      value /= 10;
    }
  }
  // Если указана точность, добавить нули для дополнения до необходимой длины
  while (len < spec->precision) {
    temp[len++] = '0';
  }
  // Если установлен флаг '0' и есть минимальная ширина, добавить нули для
  // дополнения до необходимой длины
  if (spec->zero && !spec->minus) {
    while (len < spec->width) {
      temp[len++] = '0';
    }
  }
  // Учесть выравнивание и ширину
  if (!spec->minus && !spec->zero) {
    while (len < spec->width) {
      temp[len++] = ' ';
    }
  }
  // Записать результат в выходной буфер в обратном порядке
  for (int i = len - 1; i >= 0; --i) {
    *(*buf_ptr)++ = temp[i];
  }
  // Дополнить пробелами справа, если необходимо
  if (spec->minus) {
    while (len++ < spec->width) {
      *(*buf_ptr)++ = ' ';
    }
  }
}

void format_sci_notation(char **buf_ptr, long double value, specifier *spec) {
  value = apply_float_lenght_modifier(value, spec->lenght);
  int precision = (spec->precision <= 0) ? 6 : spec->precision;
  char num_str[BUFFER_SIZE];
  int exponent = 0;

  if (value != 0.0) {
    exponent = (int)floor(log10l(fabsl(value)));
    value *= powl(10, -exponent);
  }
  // Если установлен флаг '+', добавляем '+' перед числом для положительных
  // значений
  if (spec->plus && value >= 0.0) {
    *(*buf_ptr)++ = '+';
  }
  float_to_string(num_str, value, precision, spec);
  int num_str_len = s21_strlen(num_str);
  s21_strcpy(*buf_ptr, num_str);
  *buf_ptr += num_str_len;
  *(*buf_ptr)++ =
      (spec->specifier == 'e' || spec->specifier == 'g') ? 'e' : 'E';

  int sign = (exponent >= 0) ? '+' : '-';
  *(*buf_ptr)++ = sign;

  exponent = abs(exponent);
  char exp_str[BUFFER_SIZE];
  int exp_len = 0;

  do {
    exp_str[exp_len++] = exponent % 10 + '0';
    exponent /= 10;
  } while (exponent);

  while (exp_len < 2) {
    exp_str[exp_len++] = '0';
  }

  for (int i = exp_len - 1; i >= 0; --i) {
    *(*buf_ptr)++ = exp_str[i];
  }
}

void format_g(char **buf_ptr, long double value, specifier *spec) {
  long double f_value = apply_float_lenght_modifier(value, spec->lenght);
  int precision = (spec->precision <= 0) ? 6 : spec->precision;
  int exponent = 0;
  long double abs_value = fabsl(f_value);

  if (abs_value != 0.0) {
    exponent = (int)floor(log10l(abs_value));
  }
  char *start_ptr = *buf_ptr;
  // Выбираем между фиксированной точкой и научной записью
  if (exponent >= -4 && exponent < precision) {
    if (precision > 0) {
      spec->precision = precision - exponent - 1;
    } else {
      ++spec->precision;
    }
    format_float(buf_ptr, f_value, spec);
    // Если убрать эту часть функция ломаеться при 6 знаках в целой части (хз
    // почему);
    long long int_part = (long long)value;
    int int_part_size = 0;
    while (int_part != 0) {
      int_part /= 10;
      ++int_part_size;
    }
    if (int_part_size == precision) {
      while (*(*buf_ptr - 1) != '.') {
        (*buf_ptr)--;
      }
    }
    if (!spec->hash) {
      // Удаляем лишние нули после десятичной точки
      while (*(*buf_ptr - 1) == '0') {
        (*buf_ptr)--;
      }
      // Удаляем десятичную точку, если после нее нет значащих цифр и не указан
      // флаг #
      if (!spec->hash && *(*buf_ptr - 1) == '.') {
        (*buf_ptr)--;
      }
    }
  } else {
    spec->precision = precision - 1;
    format_sci_notation(buf_ptr, f_value, spec);
  }
  int written = *buf_ptr - start_ptr;
  int padding = spec->width - written;

  if (padding > 0) {
    s21_memmove(start_ptr + padding, start_ptr, written);
    s21_memset(start_ptr, ' ', padding);
    *buf_ptr += padding;
  }
}

void format_hex(char **buf_ptr, unsigned long long value, specifier *spec) {
  char num_buf[17];
  int len = 0;
  int padding;
  char padding_char = (spec->zero && !spec->minus) ? '0' : ' ';

  value = apply_lenght_modifier_unsigned(value, spec->lenght);

  // Преобразование числа в строку
  do {
    unsigned int digit = value % 16;
    num_buf[len++] = digit < 10
                         ? '0' + digit
                         : (spec->specifier == 'x' ? 'a' : 'A') + digit - 10;
    value /= 16;
  } while (value != 0);
  // Если указан флаг '#', добавить префикс "0x" или "0X" к числу
  if (spec->hash) {
    len += 2;  // Увеличиваем длину числа на 2 из-за добавления "0x" или "0X"
  }
  // Вычисление количества символов для заполнения
  padding = (spec->width > len) ? spec->width - len : 0;
  // Если флаг 'minus' не задан, добавляем символы заполнения перед числом
  if (!spec->minus) {
    for (int i = 0; i < padding; ++i) {
      *(*buf_ptr)++ = padding_char;
    }
  }
  // Записываем префикс "0x" или "0X" после символов заполнения, если указан
  // флаг '#'
  if (spec->hash && !spec->zero) {
    *(*buf_ptr)++ = '0';
    *(*buf_ptr)++ = spec->specifier;  // 'x' или 'X'
  }
  // Записываем число
  for (int i = len - 1 - (spec->hash ? 2 : 0); i >= 0; --i) {
    *(*buf_ptr)++ = num_buf[i];
  }
  // Если флаг 'minus' задан, добавляем символы заполнения после числа
  if (spec->minus) {
    for (int i = 0; i < padding; ++i) {
      *(*buf_ptr)++ = padding_char;
    }
  }
}

void format_uint_octal(char **buf_ptr, unsigned long long value,
                       specifier *spec) {
  char num_buf[23];  // Максимальное количество знаков в беззнаковом long long
                     // int в восьмеричной системе
  int len = 0;
  int padding;
  char padding_char = (spec->zero && !spec->minus) ? '0' : ' ';
  value = apply_lenght_modifier_unsigned(value, spec->lenght);
  // Преобразование числа в строку
  do {
    unsigned int digit = value % 8;
    num_buf[len++] = '0' + digit;
    value /= 8;
  } while (value != 0);
  // Если указан флаг '#', добавить префикс "0" к числу
  if (spec->hash && num_buf[0] != '0') {
    num_buf[len++] = '0';
  }
  // Вычисление количества символов для заполнения
  padding = (spec->width > len) ? spec->width - len : 0;
  // Если флаг 'minus' не задан, добавляем символы заполнения перед числом
  if (!spec->minus) {
    for (int i = 0; i < padding; ++i) {
      *(*buf_ptr)++ = padding_char;
    }
  }
  // Записываем число
  for (int i = len - 1; i >= 0; --i) {
    *(*buf_ptr)++ = num_buf[i];
  }
  // Если флаг 'minus' задан, добавляем символы заполнения после числа
  if (spec->minus) {
    for (int i = 0; i < padding; ++i) {
      *(*buf_ptr)++ = padding_char;
    }
  }
}

void format_pointer(char **buf_ptr, const void *ptr, specifier *spec) {
  // Cпецификатор 'x' для отображения указателя в шестнадцатеричном
  // формате
  spec->specifier = 'x';
  spec->hash = true;  // Добавляем префикс '0x'
  // Если ширина и точность не заданы, устанавливаем их по умолчанию
  if (spec->precision == 0) {
    spec->precision = 2 * sizeof(void *);  // Точность для отображения указателя
  }
  // Если флаг '0' задан, уменьшаем ширину на 2, чтобы учесть префикс "0x"
  if (spec->zero && !spec->minus) {
    spec->width -= 2;
  }
  // Преобразуем указатель в беззнаковое целое
  unsigned long long ptr_value = (unsigned long long)ptr;

  // Для форматирования указателя
  format_hex(buf_ptr, ptr_value, spec);
}

long long apply_lenght_modifier_sing(long long value, char lenght) {
  switch (lenght) {
    case 'h':
      return (short)value;
    case 'l':
      return (long)value;
    default:
      return value;
  }
}

unsigned long long apply_lenght_modifier_unsigned(unsigned long long value,
                                                  char lenght) {
  switch (lenght) {
    case 'h':
      return (unsigned short)value;
    case 'l':
      return (unsigned long)value;
    default:
      return value;
  }
}

long double apply_float_lenght_modifier(long double value, char lenght) {
  switch (lenght) {
    case 'l':
      return (double)value;
    case 'L':
      return (long double)value;
    default:
      return value;
  }
}

void add_padding(char **buf_ptr, int padding) {
  for (int i = 0; i < padding; i++) {
    *(*buf_ptr)++ = ' ';
  }
}