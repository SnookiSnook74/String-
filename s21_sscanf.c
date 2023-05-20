#include "s21_sscanf.h"

int s21_sscanf(char *str, char *format, ...) {
  char *copy;
  int status = 1;
  int result = 0;
  bool is_invisible_action = 0;
  asprintf(&copy, "%s", str);
  char *start_ptr = copy;
  va_list args;
  va_start(args, format);
  while (*format && status) {
    if (*format == '%') {
      format++;
      struct Options options = {.width = 10000,
                                .is_h = false,
                                .is_l = false,
                                .is_L = false,
                                .is_ignore_data = false};
      _set_options(&options, &format);
      switch (*format) {
        case '%':
        case 'c':
          if (*copy) result += _process_c(&copy, options, &args);
          break;
        case 'd':
          if (*copy) result += _process_d(&copy, options, &args);
          break;
        case 'i':
          if (*copy) result += _process_i(&copy, options, &args);
          break;
        case 'e':
        case 'E':
        case 'f':
        case 'g':
        case 'G':
          if (*copy) result += _process_f(&copy, options, &args);
          break;
        case 'o':
          if (*copy) result += _process_o(&copy, options, &args);
          break;
        case 's':
          if (*copy) result += _process_s(&copy, options, &args);
          break;
        case 'u':
          if (*copy) result += _process_u(&copy, options, &args);
          break;
        case 'x':
        case 'X':
          if (*copy) result += _process_x(&copy, options, &args);
          break;
        case 'p':
          if (*copy) result += _process_p(&copy, options, &args);
          break;
        case 'n':
          _process_n(start_ptr, copy, &args);
          is_invisible_action = true;
          break;
      }
    } else {
      if (copy[0] == *format) {
        copy = &copy[1];
      } else if (isspace(copy[0])) {
        copy = &copy[1];
      } else {
        status = 0;
      }
    }
    format++;
  }
  if (s21_strlen(copy) == 0 && is_invisible_action == false) {
    if (result == 0) result = -1;
  }
  free(start_ptr);
  va_end(args);
  return result;
}

int _process_p(char **str, struct Options options, va_list *args) {
  int counter = 0;
  while (isspace(**str)) (*str)++;
  if ((**str) == '0') {
    (*str)++;
  }
  if ((**str) == 'x' || (**str) == 'X') {
    (*str)++;
  }

  if (options.is_ignore_data == false) {
    unsigned long long result = 0;
    unsigned long long num = 0;
    void **ptr = va_arg(*args, void **);
    while (_is_hexal(**str) && options.width--) {
      if ((**str) >= '0' && (**str) <= '9') {
        num = (**str) - '0';
      } else if ((**str) >= 'a' && (**str) <= 'f') {
        num = ((**str) - 'a') + 10;
      } else if ((**str) >= 'A' && (**str) <= 'F') {
        num = ((**str) - 'A') + 10;
      }
      result = result * 16 + num;
      (*str)++;
    }
    *ptr = (void *)result;
    counter++;
  } else {
    while (_is_hexal(**str) && options.width--) {
      (*str)++;
    }
  }
  return counter;
}

int _process_f(char **str, struct Options options, va_list *args) {
  while (isspace(*str[0])) (*str)++;
  int count = 0;
  regex_t regex;
  regmatch_t match;
  int ret;
  ret =
      regcomp(&regex, "^[+-]?[0-9]+[.][0-9]+([eE][+-]?[0-9]+)?", REG_EXTENDED);
  if (ret) {
    fprintf(stderr, "Could not compile regex\n");
  } else {
    if (regexec(&regex, *str, 1, &match, 0) == 0) {
      int size = (int)(match.rm_eo - match.rm_so);
      if (size > options.width) size = options.width;
      char line[100] = {0};
      s21_strncpy(line, *str + match.rm_so, size);
      if (options.is_ignore_data == false) _parse_double(line, options, args);
      (*str) += size;
      count++;
    }
  }
  regfree(&regex);
  return count;
}

void _parse_double(char *str, struct Options options, va_list *args) {
  long double result = 0;
  int fractional = 0;
  int exponent = 0;
  int sign = 1;
  int exponent_sign = 1;
  if (*str == '-') {
    sign = -1;
    str++;
  }
  while (_is_decimal(*str)) {
    result = result * 10 + (*str) - '0';
    str++;
  }
  if ((*str) == '.') {
    str++;
    while (_is_decimal(*str)) {
      result = result * 10 + (*str) - '0';
      str++;
      fractional++;
    }
  }
  if ((*str) == 'e' || (*str) == 'E') {
    str++;
    if ((*str) == '-') {
      str++;
      exponent_sign = -1;
    } else if ((*str) == '+') {
      str++;
    }
    while (_is_decimal(*str)) {
      exponent = exponent * 10 + (*str) - '0';
      str++;
    }
  }
  exponent *= exponent_sign;
  result = result * pow(10, exponent);
  result = result / pow(10, fractional);
  if (options.is_L) {
    long double *num = va_arg(*args, long double *);
    *num = result * sign;
  } else if (options.is_l) {
    double *num = va_arg(*args, double *);
    *num = (double)(result * sign);
  } else {
    float *num = va_arg(*args, float *);
    *num = (float)result * sign;
  }
}

void _process_n(char *start, char *end, va_list *args) {
  int *num = va_arg(*args, int *);
  if (s21_strstr(start, end))
    *num = s21_strlen(start) - s21_strlen(end);
  else
    *num = s21_strlen(start);
}

int _process_s(char **str, struct Options options, va_list *args) {
  int count = 0;
  regex_t regex;
  regmatch_t match;
  int ret;
  // любые буквенные символы или цифры, а также символ подчеркивания
  ret = regcomp(&regex, "[[:alnum:]_]+", REG_EXTENDED);
  if (ret) {
    fprintf(stderr, "Could not compile regex\n");
  } else {
    if (regexec(&regex, *str, 1, &match, 0) == 0) {
      int size = (int)(match.rm_eo - match.rm_so);
      if (size > options.width) size = options.width;
      if (options.is_ignore_data == false) {
        if (options.is_l) {
          wchar_t *line = va_arg(*args, wchar_t *);
          swprintf(line, (size_t)size, L"%s", *str + match.rm_so);
        } else {
          char *line = va_arg(*args, char *);
          s21_strncpy(line, *str + match.rm_so, size);
          line[size] = '\0';
        }
        count++;
      }
      (*str) += match.rm_eo;
    }
  }
  regfree(&regex);
  return count;
}

int _process_u(char **str, struct Options options, va_list *args) {
  bool is_positive = true;
  int result = 0;
  while (isspace(*str[0])) (*str)++;
  if ((**str) == '-') {
    options.width--;
    is_positive = false;
    (*str)++;
  }
  if (options.is_ignore_data == false) {
    result = _parse_unsigned_decimal(str, options, args, is_positive);
  } else {
    while (_is_decimal(**str)) (*str)++;
  }
  return result;
}

int _process_x(char **str, struct Options options, va_list *args) {
  bool is_positive = true;
  int result = 0;
  while (isspace(*str[0])) (*str)++;
  if ((**str) == '-') {
    options.width--;
    is_positive = false;
    (*str)++;
  }
  if ((**str) == '0') {
    options.width--;
    (*str)++;
  }
  if ((**str) == 'x' || (**str) == 'X') {
    options.width--;
    (*str)++;
  }
  if (options.is_ignore_data == false) {
    result = _parse_unsigned_heximal(str, options, args, is_positive);
  } else {
    while (_is_hexal(**str)) (*str)++;
  }
  return result;
}

int _process_o(char **str, struct Options options, va_list *args) {
  bool is_positive = true;
  int result = 0;
  while (isspace(*str[0])) (*str)++;
  if ((**str) == '-') {
    options.width--;
    is_positive = false;
    (*str)++;
  }
  if (options.is_ignore_data == false) {
    result = _parse_unsigned_octal(str, options, args, is_positive);
  } else {
    while (_is_octal(**str)) (*str)++;
  }
  return result;
}

int _process_i(char **str, struct Options options, va_list *args) {
  int result = 0;
  while (isspace(*str[0])) (*str)++;
  bool is_positive = true;
  if ((**str) == '-') {
    options.width--;
    is_positive = false;
    (*str)++;
  }
  if (options.is_ignore_data == false) {
    if (**str == '0') {
      options.width--;
      (*str)++;
      if (**str == 'x' || **str == 'X') {
        options.width--;
        (*str)++;
        result = _parse_heximal(str, options, args, is_positive);
      } else {
        result = _parse_octal(str, options, args, is_positive);
      }
    } else {
      result += _parse_decimal(str, options, args, is_positive);
    }
  } else {
    if (**str == '0') {
      (*str)++;
      if (**str == 'x' || **str == 'X') {
        (*str)++;
        while (_is_hexal(**str)) (*str)++;
      } else {
        while (_is_octal(**str)) (*str)++;
      }
    } else {
      while (_is_decimal(**str)) (*str)++;
    }
  }
  return result;
}

bool _is_decimal(char c) {
  bool result = false;
  if (c >= '0' && c <= '9') {
    result = true;
  }
  return result;
}

bool _is_octal(char c) {
  bool result = false;
  if (c >= '0' && c <= '7') {
    result = true;
  }
  return result;
}

int _process_c(char **str, struct Options options, va_list *args) {
  int result = 0;
  if (options.is_ignore_data == false) {
    if (options.is_l) {
      wchar_t *c = va_arg(*args, wchar_t *);
      *c = **str;
      (*str)++;
    } else {
      char *c = va_arg(*args, char *);
      *c = **str;
      (*str)++;
    }
    result++;
  } else {
    (*str)++;
  }
  return result;
}

int _process_d(char **str, struct Options options, va_list *args) {
  int result = 0;
  bool is_positive = true;
  while (isspace(*str[0])) (*str)++;
  if ((**str) == '-') {
    is_positive = false;
    options.width--;
    (*str)++;
  }
  if (options.is_ignore_data == false) {
    result += _parse_decimal(str, options, args, is_positive);
  } else {
    while (*(*str) >= '0' && *(*str) <= '9' && options.width--)
      *str = &(*str)[1];
  }
  return result;
}

void _set_options(struct Options *options, char **format) {
  if (**format == '*') {
    options->is_ignore_data = true;
    (*format)++;
  }
  while (**format >= '0' && **format <= '9') {
    if (options->width == 10000) options->width = 0;
    options->width *= 10;
    options->width += **format - '0';
    (*format)++;
  }
  if (**format == 'h') {
    options->is_h = true;
    (*format)++;
  } else if (**format == 'l') {
    options->is_l = true;
    (*format)++;
  } else if (**format == 'L') {
    options->is_L = true;
    (*format)++;
  }
}

int _parse_decimal(char **str, struct Options options, va_list *args,
                   bool is_positive) {
  int count = 0;
  int sign = is_positive ? 1 : -1;
  if (options.is_h) {
    short int *num = va_arg(*args, short int *);
    count = _get_short_int(num, str, options);
    *num *= sign;
  } else if (options.is_l) {
    long int *num = va_arg(*args, long int *);
    count = _get_long_int(num, str, options);
    *num *= sign;
  } else {
    int *num = va_arg(*args, int *);
    count = _get_int(num, str, options);
    *num *= sign;
  }
  return count;
}

int _get_short_int(short int *num, char **str, struct Options options) {
  int result = 0;
  *num = 0;
  while ((**str) >= '0' && (**str) <= '9' && options.width--) {
    *num = *num * 10 + (short int)((**str) - '0');
    (*str)++;
    if (result == 0) result = 1;
  }
  return result;
}

int _get_long_int(long int *num, char **str, struct Options options) {
  long int result = 0;
  *num = 0;
  while ((**str) >= '0' && (**str) <= '9' && options.width--) {
    *num = *num * 10 + (long int)((**str) - '0');
    (*str)++;
    if (result == 0) result = 1;
  }
  return result;
}

int _get_int(int *num, char **str, struct Options options) {
  int result = 0;
  *num = 0;
  while ((**str) >= '0' && (**str) <= '9' && options.width--) {
    *num = *num * 10 + (long int)((**str) - '0');
    (*str)++;
    if (result == 0) result = 1;
  }
  return result;
}

int _parse_heximal(char **str, struct Options options, va_list *args,
                   bool is_positive) {
  int count = 0;
  int sign = is_positive ? 1 : -1;
  if (options.is_h) {
    short int *num = va_arg(*args, short int *);
    *num = _get_hex_short_int(str, options) * sign;
    count++;
  } else if (options.is_l) {
    long int *num = va_arg(*args, long int *);
    *num = _get_hex_long_int(str, options) * sign;
    count++;
  } else {
    int *num = va_arg(*args, int *);
    *num = _get_hex_int(str, options) * sign;
    count++;
  }
  return count;
}

int _parse_unsigned_heximal(char **str, struct Options options, va_list *args,
                            bool is_positive) {
  int count = 0;
  int sign = is_positive ? 1 : -1;
  if (options.is_h) {
    unsigned short int *num = va_arg(*args, unsigned short int *);
    *num = _get_unsigned_hex_short_int(str, options) * sign;
    count++;
  } else if (options.is_l) {
    unsigned long int *num = va_arg(*args, unsigned long int *);
    *num = _get_unsigned_hex_long_int(str, options) * sign;
    count++;
  } else {
    unsigned int *num = va_arg(*args, unsigned int *);
    *num = _get_unsigned_hex_int(str, options) * sign;
    count++;
  }
  return count;
}

unsigned short int _get_unsigned_hex_short_int(char **str,
                                               struct Options options) {
  unsigned short int result = 0;
  unsigned short int num = 0;
  while (_is_hexal(**str) && options.width--) {
    if ((**str) >= '0' && (**str) <= '9') {
      num = (**str) - '0';
    } else if ((**str) >= 'a' && (**str) <= 'f') {
      num = ((**str) - 'a') + 10;
    } else if ((**str) >= 'A' && (**str) <= 'F') {
      num = ((**str) - 'A') + 10;
    }
    result = result * 16 + num;
    (*str)++;
  }
  return result;
}

unsigned long int _get_unsigned_hex_long_int(char **str,
                                             struct Options options) {
  unsigned long int result = 0;
  unsigned long int num = 0;
  while (_is_hexal(**str) && options.width--) {
    if ((**str) >= '0' && (**str) <= '9') {
      num = (**str) - '0';
    } else if ((**str) >= 'a' && (**str) <= 'f') {
      num = ((**str) - 'a') + 10;
    } else if ((**str) >= 'A' && (**str) <= 'F') {
      num = ((**str) - 'A') + 10;
    }
    result = result * 16 + num;
    (*str)++;
  }
  return result;
}

unsigned int _get_unsigned_hex_int(char **str, struct Options options) {
  unsigned int result = 0;
  unsigned int num = 0;
  while (_is_hexal(**str) && options.width--) {
    if ((**str) >= '0' && (**str) <= '9') {
      num = (**str) - '0';
    } else if ((**str) >= 'a' && (**str) <= 'f') {
      num = ((**str) - 'a') + 10;
    } else if ((**str) >= 'A' && (**str) <= 'F') {
      num = ((**str) - 'A') + 10;
    }
    result = result * 16 + num;
    (*str)++;
  }
  return result;
}

int _get_hex_int(char **str, struct Options options) {
  int result = 0;
  int num = 0;
  while (_is_hexal(**str) && options.width--) {
    if ((**str) >= '0' && (**str) <= '9') {
      num = (**str) - '0';
    } else if ((**str) >= 'a' && (**str) <= 'f') {
      num = ((**str) - 'a') + 10;
    } else if ((**str) >= 'A' && (**str) <= 'F') {
      num = ((**str) - 'A') + 10;
    }
    result = result * 16 + num;
    (*str)++;
  }
  return result;
}

long int _get_hex_long_int(char **str, struct Options options) {
  long int result = 0;
  long int num = 0;
  while (_is_hexal(**str) && options.width--) {
    if ((**str) >= '0' && (**str) <= '9') {
      num = (**str) - '0';
    } else if ((**str) >= 'a' && (**str) <= 'f') {
      num = ((**str) - 'a') + 10;
    } else if ((**str) >= 'A' && (**str) <= 'F') {
      num = ((**str) - 'A') + 10;
    }
    result = result * 16 + num;
    (*str)++;
  }
  return result;
}

short int _get_hex_short_int(char **str, struct Options options) {
  short int result = 0;
  short int num = 0;
  while (_is_hexal(**str) && options.width--) {
    if ((**str) >= '0' && (**str) <= '9') {
      num = (**str) - '0';
    } else if ((**str) >= 'a' && (**str) <= 'f') {
      num = ((**str) - 'a') + 10;
    } else if ((**str) >= 'A' && (**str) <= 'F') {
      num = ((**str) - 'A') + 10;
    }
    result = result * 16 + num;
    (*str)++;
  }
  return result;
}

int _parse_octal(char **str, struct Options options, va_list *args,
                 bool is_positive) {
  int count = 0;
  int sign = is_positive ? 1 : -1;
  if (options.is_h) {
    short int *num = va_arg(*args, short int *);
    *num = _get_oct_short_int(str, options) * sign;
    count++;
  } else if (options.is_l) {
    long int *num = va_arg(*args, long int *);
    *num = _get_oct_long_int(str, options) * sign;
    count++;
  } else {
    int *num = va_arg(*args, int *);
    *num = _get_oct_int(str, options) * sign;
    count++;
  }
  return count;
}

int _parse_unsigned_octal(char **str, struct Options options, va_list *args,
                          bool is_positive) {
  int count = 0;
  int sign = is_positive ? 1 : -1;
  if (options.is_h) {
    unsigned short int *num = va_arg(*args, unsigned short int *);
    *num = _get_unsigned_oct_short_int(str, options) * sign;
    count++;
  } else if (options.is_l) {
    unsigned long int *num = va_arg(*args, unsigned long int *);
    *num = _get_unsigned_oct_long_int(str, options) * sign;
    count++;
  } else {
    unsigned int *num = va_arg(*args, unsigned int *);
    *num = _get_unsigned_oct_int(str, options) * sign;
    count++;
  }
  return count;
}

unsigned int _get_unsigned_oct_int(char **str, struct Options options) {
  unsigned int result = 0;
  while ((**str) >= '0' && (**str) <= '7' && options.width--) {
    result = result * 8 + (**str) - '0';
    (*str)++;
  }
  return result;
}

unsigned short int _get_unsigned_oct_short_int(char **str,
                                               struct Options options) {
  unsigned short int result = 0;
  while ((**str) >= '0' && (**str) <= '7' && options.width--) {
    result = result * 8 + (**str) - '0';
    (*str)++;
  }
  return result;
}

unsigned long int _get_unsigned_oct_long_int(char **str,
                                             struct Options options) {
  unsigned long int result = 0;
  while ((**str) >= '0' && (**str) <= '7' && options.width--) {
    result = result * 8 + (**str) - '0';
    (*str)++;
  }
  return result;
}

int _get_oct_int(char **str, struct Options options) {
  int result = 0;
  while ((**str) >= '0' && (**str) <= '7' && options.width--) {
    result = result * 8 + (**str) - '0';
    (*str)++;
  }
  return result;
}

short int _get_oct_short_int(char **str, struct Options options) {
  short int result = 0;
  while ((**str) >= '0' && (**str) <= '7' && options.width--) {
    result = result * 8 + (**str) - '0';
    (*str)++;
  }
  return result;
}

long int _get_oct_long_int(char **str, struct Options options) {
  long int result = 0;
  while ((**str) >= '0' && (**str) <= '7' && options.width--) {
    result = result * 8 + (**str) - '0';
    (*str)++;
  }
  return result;
}

bool _is_hexal(char c) {
  bool result = false;
  if (c >= '0' && c <= '9') {
    result = true;
  } else if (c >= 'A' && c <= 'F') {
    result = true;
  } else if (c >= 'a' && c <= 'f') {
    result = true;
  }
  return result;
}

unsigned int _parse_unsigned_decimal(char **str, struct Options options,
                                     va_list *args, bool is_positive) {
  int count = 0;
  int sign = is_positive ? 1 : -1;
  if (options.is_h) {
    unsigned short int *num = va_arg(*args, unsigned short int *);
    *num = _get_unsigned_short_int(str, options) * sign;
    count++;
  } else if (options.is_l) {
    unsigned long int *num = va_arg(*args, unsigned long int *);
    *num = _get_unsigned_long_int(str, options) * sign;
    count++;
  } else {
    unsigned int *num = va_arg(*args, unsigned int *);
    *num = _get_unsigned_int(str, options) * sign;
    count++;
  }
  return count;
}

unsigned short int _get_unsigned_short_int(char **str, struct Options options) {
  unsigned short int result = 0;

  while ((**str) >= '0' && (**str) <= '9' && options.width--) {
    result = result * 10 + (unsigned short int)((**str) - '0');
    (*str)++;
  }
  return result;
}

unsigned long int _get_unsigned_long_int(char **str, struct Options options) {
  unsigned long int result = 0;
  while ((**str) >= '0' && (**str) <= '9' && options.width--) {
    result = result * 10 + (unsigned long int)((**str) - '0');
    (*str)++;
  }
  return result;
}

unsigned int _get_unsigned_int(char **str, struct Options options) {
  unsigned int result = 0;
  while ((**str) >= '0' && (**str) <= '9' && options.width--) {
    result = result * 10 + (unsigned long int)((**str) - '0');
    (*str)++;
  }
  return result;
}