#ifndef S21_STRING_H_
#define S21_STRING_H_
#define _GNU_SOURCE

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <regex.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "s21_sprintf.h"
#include "s21_sscanf.h"

#define BUFFER 2048
#define s21_NULL (void *)0
typedef long unsigned s21_size_t;

/* 1. Ищет первое вхождение символа "c" в области памяти str, размером n байтов.
Если символ найден, функция возвращает указатель на его позицию в str; в
противном случае возвращается s21_NULL.
*/
void *s21_memchr(const void *str, int c, s21_size_t n);
/* 2. Сравнивает две области памяти, на которые указывают str1 и str2, на
протяжении n байтов.
*/
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
/* 3. Копирует n символов из области памяти, на которую указывает src, в область
памяти, на которую указывает dest
 */
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
/* 4. Копирует символ c  в первые n символов строки, на которую указывает
 аргумент str.
*/
void *s21_memset(void *str, int c, s21_size_t n);
/* 5. Добавляет строку, на которую указывает src, в конец строки, на которую
указывает dest, копируя до n символов.
*/
char *s21_strncat(char *dest, const char *src, s21_size_t n);
/* 6. Выполняет поиск первого вхождения символа c в строке str и возвращает
   указатель на первое вхождение или s21_NULL, если символ не найден.
 */
char *s21_strchr(const char *str, int c);
/* 7. Cравнивает не более первых n байтов двух строк str1 и str2:*/
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
/* 8. Копирует до n символов из строки, на которую указывает src, в dest.*/
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
/* 9. Вычисляет длину начального сегмента строки str1, который состоит из
  символов, отсутствующих в строке str2*/
s21_size_t s21_strcspn(const char *str1, const char *str2);
/* 10. Выводит сообщения об ошибках*/
char *s21_strerror(int errnum);
/* 11. Вычисляет длину строки str, не включая завершающий нулевой символ.*/
s21_size_t s21_strlen(const char *str);
/* 12. Функция проходит по каждому символу в строке str1 и проверяет,
 содержится ли этот символ в строке str2. Если символ находится, то функция
 возвращает указатель на этот символ в str1. Если же функция не находит
 соответствующих символов, то она возвращает нулевой указатель s21_NULL.*/
char *s21_strpbrk(const char *str1, const char *str2);
/* 13. Выполняет поиск последнего вхождения символа c (беззнаковый тип) в строке
 str*/
char *s21_strrchr(const char *str, int c);
/* 14. Ищет первое вхождение подстроки needle в строке haystack.*/
char *s21_strstr(const char *haystack, const char *needle);
/* 15. Разбивает строку str на ряд токенов, разделенных delim. */
char *s21_strtok(char *str, const char *delim);
/* 16. Доп функция для strtok. Вычисляет длину начального сегмента str1, который
 * полностью состоит из символов str2.*/
s21_size_t s21_strspn(const char *str1, const char *str2);
/* 17. Доп функция для sprintf Копирует строку, на которую указывает src, в dest
 */
char *s21_strcpy(char *dest, const char *src);
/* 18. Доп функция для sprintf Обрабатывает выход за область памяти, что
означает, что она корректно копирует данные, даже если источник и назначение
разного размера.
*/
void *s21_memmove(void *dest, const void *src, s21_size_t n);
/* 19. Возвращает копию строки (str), преобразованной в верхний регистр.
В случае какой-либо ошибки следует вернуть значение NULL*/
void *s21_to_upper(const char *str);
/* 20. Возвращает копию строки (str), преобразованной в нижний регистр.
В случае какой-либо ошибки следует вернуть значение NULL*/
void *s21_to_lower(const char *str);
/* 21. Возвращает новую строку, в которой указанная строка (str)
вставлена в указанную позицию (start_index) в данной строке (src).
В случае какой-либо ошибки следует вернуть значение NULL*/
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
/* 22. Возвращает новую строку, в которой удаляются все начальные и
конечные вхождения набора заданных символов (trim_chars) из данной строки (src).
В случае какой-либо ошибки следует вернуть значение NULL*/
void *s21_trim(const char *src, const char *trim_chars);

#endif  // S21_STRING_H_