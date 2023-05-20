#include <check.h>
#include <string.h>

#include "s21_string.h"

/* -----===== PART-1 =====----- */
/* -----===== START =====----- */

// MEMCHR TEST

START_TEST(test_s21_memchr_fn) {
  const char *s = "hello world";
  char *result = s21_memchr(s, 'w', strlen(s));
  ck_assert_ptr_eq(result, s + 6);
}
END_TEST

START_TEST(test_s21_memchr_fn_null) {
  const char *s = "hello world";
  char *result = s21_memchr(s, 'z', strlen(s));
  ck_assert_ptr_eq(result, NULL);
}
END_TEST

START_TEST(test_s21_memchr_fn_compare) {
  const char *s = "hello world";
  char *result_s21 = s21_memchr(s, 'o', strlen(s));
  char *result_str = memchr(s, 'o', strlen(s));
  ck_assert_ptr_eq(result_s21, result_str);
}
END_TEST

// MEMCMP TEST

START_TEST(test_empty_strings) {
  char str1[] = "";
  char str2[] = "";
  ck_assert_int_eq(s21_memcmp(str1, str2, 0), memcmp(str1, str2, 0));
}
END_TEST

START_TEST(test_different_lengths) {
  char str1[] = "Hello, world!";
  char str2[] = "Hello";
  ck_assert_int_eq(s21_memcmp(str1, str2, strlen(str2)),
                   memcmp(str1, str2, strlen(str2)));
}
END_TEST

START_TEST(test_same_strings) {
  char str1[] = "Hello, world!";
  char str2[] = "Hello, world!";
  ck_assert_int_eq(s21_memcmp(str1, str2, strlen(str1)),
                   memcmp(str1, str2, strlen(str1)));
}
END_TEST

START_TEST(test_different_strings_negative) {
  char str1[] = "Hello, world!";
  char str2[] = "Hola, mundo!";
  ck_assert(s21_memcmp(str1, str2, strlen(str1)) *
                memcmp(str1, str2, strlen(str1)) >=
            0);
}
END_TEST

START_TEST(test_different_strings_positive) {
  char str1[] = "Hola, mundo!";
  char str2[] = "Hello, world!";
  ck_assert(s21_memcmp(str1, str2, strlen(str1)) *
                memcmp(str1, str2, strlen(str1)) >=
            0);
}
END_TEST

// MEMCPY TEST

START_TEST(test_s21_memcpy_basic) {
  char src[6] = "Hello";
  char dest[6];
  char dest_orig[6];

  s21_memcpy(dest, src, sizeof(src));
  memcpy(dest_orig, src, sizeof(src));

  ck_assert_str_eq(dest, dest_orig);
}
END_TEST

START_TEST(test_s21_memcpy_non_overlap) {
  char src[] = "Hello, world!";
  char dest[14];
  char dest_orig[14];

  s21_memcpy(dest, src, sizeof(src));
  memcpy(dest_orig, src, sizeof(src));

  ck_assert_str_eq(dest, dest_orig);
}
END_TEST

START_TEST(test_s21_memcpy_zero_size) {
  char src[6] = "Hello";
  char dest[6] = "World";
  char dest_orig[6] = "World";

  s21_memcpy(dest, src, 0);
  memcpy(dest_orig, src, 0);

  ck_assert_str_eq(src, "Hello");
  ck_assert_str_eq(dest, dest_orig);
}
END_TEST

START_TEST(test_s21_memcpy_large_size) {
  char src[1024];
  char dest[1024];
  char dest_orig[1024];

  for (int i = 0; i < 1024; i++) {
    src[i] = i % 256;
  }

  s21_memcpy(dest, src, sizeof(src));
  memcpy(dest_orig, src, sizeof(src));

  ck_assert_mem_eq(dest, dest_orig, sizeof(src));
}
END_TEST

// TEST MEMSET

START_TEST(test_s21_memset_normal_case) {
  char str1[10] = "test";
  char str2[10] = "test";

  s21_memset(str1, 'a', 5);
  memset(str2, 'a', 5);

  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_s21_memset_zero_length) {
  char str1[10] = "test";
  char str2[10] = "test";

  ck_assert_str_eq(str1, str2);
  s21_memset(str1, 'a', 0);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_s21_memset_full_length) {
  char str1[10] = "test";
  char str2[10] = "test";

  s21_memset(str1, 'a', sizeof(str1));
  memset(str2, 'a', sizeof(str2));

  ck_assert_mem_eq(str1, str2, sizeof(str1));
}
END_TEST

START_TEST(test_s21_memset_null_pointer) {
  void *str1 = s21_NULL;
  void *str2 = s21_NULL;

  ck_assert_ptr_eq(s21_memset(str1, 'a', 5), str2);
}
END_TEST

// TEST STRNCAT

START_TEST(test_s21_strncat_normal_case) {
  char dest1[20] = "Hello";
  char dest2[20] = "Hello";
  const char *src = " world!";

  s21_strncat(dest1, src, 7);
  strncat(dest2, src, 7);

  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_s21_strncat_less_than_n) {
  char dest1[25] = "Hello";
  char dest2[25] = "Hello";
  const char *src = " world!";

  s21_strncat(dest1, src, 20);
  strncat(dest2, src, 20);

  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_s21_strncat_zero_length) {
  char dest1[20] = "Hello";
  char dest2[20] = "Hello";
  const char *src = " world!";

  s21_strncat(dest1, src, 0);
  strncat(dest2, src, 0);

  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_s21_strncat_null_dest) {
  char *dest1 = s21_NULL;
  char *dest2 = s21_NULL;
  const char *src = " world!";

  ck_assert_ptr_eq(s21_strncat(dest1, src, 5), dest2);
}
END_TEST

// TEST STRCHR

START_TEST(test_s21_strchr_found) {
  const char *str = "Hello, world!";
  char c = 'w';

  char *result1 = s21_strchr(str, c);
  char *result2 = strchr(str, c);

  ck_assert_ptr_eq(result1, str + 7);
  ck_assert_ptr_eq(result2, str + 7);
}
END_TEST

START_TEST(test_s21_strchr_not_found) {
  const char *str = "Hello, world!";
  char c = 'x';

  char *result1 = s21_strchr(str, c);
  char *result2 = strchr(str, c);

  ck_assert_ptr_eq(result1, result2);
}
END_TEST

START_TEST(test_s21_strchr_null_termination) {
  const char *str = "Hello, world!";
  char c = '\0';

  char *result1 = s21_strchr(str, c);
  char *result2 = strchr(str, c);

  ck_assert_ptr_eq(result1, result2);
}
END_TEST

START_TEST(test_s21_strchr_null_string) {
  const char *str = s21_NULL;
  char c = 'H';

  char *result1 = s21_strchr(str, c);

  ck_assert_ptr_eq(result1, s21_NULL);
}
END_TEST

// TEST STRNCMP

START_TEST(test_s21_strncmp_equal_strings) {
  const char *str1 = "Hello, world!";
  const char *str2 = "Hello, world!";

  int result1 = s21_strncmp(str1, str2, 8);
  int result2 = strncmp(str1, str2, 8);

  ck_assert_int_eq(result1, result2);
}
END_TEST

START_TEST(test_s21_strncmp_unequal_strings) {
  const char *str1 = "Hello, world!";
  const char *str2 = "Hello, Universe!";

  int result1 = s21_strncmp(str1, str2, 8);
  int result2 = strncmp(str1, str2, 8);

  ck_assert_int_eq(result1 > 0, result2 > 0);
  ck_assert_int_eq(result1 < 0, result2 < 0);
}
END_TEST

START_TEST(test_s21_strncmp_empty_strings) {
  const char *str1 = "";
  const char *str2 = "";

  int result1 = s21_strncmp(str1, str2, 5);
  int result2 = strncmp(str1, str2, 5);
  ck_assert_int_eq(result1, result2);
}
END_TEST

START_TEST(test_s21_strncmp_zero_n) {
  const char *str1 = "Hello, world!";
  const char *str2 = "Hello, Universe!";
  int result1 = s21_strncmp(str1, str2, 0);
  int result2 = strncmp(str1, str2, 0);

  ck_assert_int_eq(result1, result2);
}
END_TEST

// TEST STRNCPY

START_TEST(test_s21_strncpy_non_empty_string) {
  const char *src = "Hello, world!";
  char dest1[20];
  char dest2[20];

  char *result1 = s21_strncpy(dest1, src, 8);
  char *result2 = strncpy(dest2, src, 8);

  ck_assert_ptr_eq(result1, dest1);
  ck_assert_ptr_eq(result2, dest2);
  ck_assert_mem_eq(dest1, dest2, 8);
}
END_TEST

START_TEST(test_s21_strncpy_full_copy) {
  const char *src = "Hello, world!";
  char dest1[20];
  char dest2[20];

  char *result1 = s21_strncpy(dest1, src, 13);
  char *result2 = strncpy(dest2, src, 13);

  ck_assert_ptr_eq(result1, dest1);
  ck_assert_ptr_eq(result2, dest2);
  ck_assert_mem_eq(dest1, dest2, 13);
}
END_TEST

START_TEST(test_s21_strncpy_empty_string) {
  const char *src = "";
  char dest1[20];
  char dest2[20];
  char *result1 = s21_strncpy(dest1, src, 10);
  char *result2 = strncpy(dest2, src, 10);

  ck_assert_ptr_eq(result1, dest1);
  ck_assert_ptr_eq(result2, dest2);
  ck_assert_mem_eq(dest1, dest2, 10);
}
END_TEST

// TEST STRCSPN

START_TEST(test_s21_strcspn_no_matching_chars) {
  const char *str1 = "abcdef";
  const char *str2 = "ghijkl";
  s21_size_t result1 = s21_strcspn(str1, str2);
  size_t result2 = strcspn(str1, str2);
  ck_assert_uint_eq(result1, result2);
}
END_TEST

START_TEST(test_s21_strcspn_some_matching_chars) {
  const char *str1 = "abcdefghijk";
  const char *str2 = "ijklmnop";
  s21_size_t result1 = s21_strcspn(str1, str2);
  size_t result2 = strcspn(str1, str2);
  ck_assert_uint_eq(result1, result2);
}
END_TEST

// TEST STRERROR
START_TEST(test_strerror) {
  int i;
  for (i = 0; i < 160; i++) {
    ck_assert_str_eq(s21_strerror(i), strerror(i));
  }
}

// TEST STRLEN

START_TEST(test_s21_strlen_empty_string) {
  const char *str = "";
  s21_size_t result1 = s21_strlen(str);
  s21_size_t result2 = strlen(str);
  ck_assert_int_eq(result1, result2);
}
END_TEST

START_TEST(test_s21_strlen_nonempty_string) {
  const char *str = "Hello, world!";
  s21_size_t result1 = s21_strlen(str);
  s21_size_t result2 = strlen(str);
  ck_assert_int_eq(result1, result2);
}
END_TEST

// TEST STRPBRK

START_TEST(test_s21_strpbrk_found) {
  const char *str1 = "Hello, World!";
  const char *str2 = "World";
  char *expected = strpbrk(str1, str2);
  char *result = s21_strpbrk(str1, str2);
  ck_assert_ptr_eq(result, expected);
}
END_TEST

START_TEST(test_s21_strpbrk_not_found) {
  const char *str1 = "Hello, World!";
  const char *str2 = "xyz";
  char *expected = strpbrk(str1, str2);
  char *result = s21_strpbrk(str1, str2);
  ck_assert_ptr_eq(result, expected);
}
END_TEST

START_TEST(test_s21_strpbrk_null_str1) {
  const char *str1 = s21_NULL;
  const char *str2 = "World";
  char *result = s21_strpbrk(str1, str2);
  ck_assert_ptr_eq(result, s21_NULL);
}
END_TEST

// TEST STRRCHR

START_TEST(test_s21_strrchr_found) {
  const char *str = "Hello, World! This is a test.";
  int c = 't';
  char *expected = strrchr(str, c);
  char *result = s21_strrchr(str, c);
  ck_assert_ptr_eq(result, expected);
}
END_TEST

START_TEST(test_s21_strrchr_not_found) {
  const char *str = "Hello, World! This is a test.";
  int c = 'z';
  char *expected = strrchr(str, c);
  char *result = s21_strrchr(str, c);
  ck_assert_ptr_eq(result, expected);
}
END_TEST

START_TEST(test_s21_strrchr_null_str) {
  const char *str = s21_NULL;
  int c = 't';
  char *result = s21_strrchr(str, c);
  ck_assert_ptr_eq(result, s21_NULL);
}
END_TEST

// TEST STRSTR

START_TEST(test_s21_strstr_empty_needle) {
  const char *haystack = "Hello, World!";
  const char *needle = "";
  ck_assert_ptr_eq(strstr(haystack, needle), s21_strstr(haystack, needle));
}
END_TEST

START_TEST(test_s21_strstr_full_match) {
  const char *haystack = "Hello, World!";
  const char *needle = "Hello, World!";
  ck_assert_ptr_eq(strstr(haystack, needle), s21_strstr(haystack, needle));
}
END_TEST

START_TEST(test_s21_strstr_partial_match) {
  const char *haystack = "Hello, World!";
  const char *needle = "World";
  ck_assert_ptr_eq(strstr(haystack, needle), s21_strstr(haystack, needle));
}
END_TEST

// TEST STRTOK

START_TEST(test_s21_strtok_basic) {
  char input[] = "Hello, world! This is an example.";
  char input_orig[] = "Hello, world! This is an example.";
  char delim[] = " ,!";

  char *token_s21 = s21_strtok(input, delim);
  char *token_orig = strtok(input_orig, delim);

  while (token_s21 != s21_NULL && token_orig != NULL) {
    ck_assert_msg(strcmp(token_s21, token_orig) == 0,
                  "Token mismatch: s21_strtok got '%s', strtok got '%s'",
                  token_s21, token_orig);
    token_s21 = s21_strtok(s21_NULL, delim);
    token_orig = strtok(NULL, delim);
  }

  ck_assert(token_s21 == s21_NULL && token_orig == NULL);
}
END_TEST

// TEST STRSPN

START_TEST(test_s21_strspn_matching) {
  const char *str1 = "Hello, World!";
  const char *str2 = "Hel";
  s21_size_t expected = strspn(str1, str2);
  size_t result = s21_strspn(str1, str2);
  ck_assert_uint_eq(result, expected);
}
END_TEST

START_TEST(test_s21_strspn_no_matching) {
  const char *str1 = "Hello, World!";
  const char *str2 = "XYZ";
  s21_size_t expected = strspn(str1, str2);
  s21_size_t result = s21_strspn(str1, str2);
  ck_assert_uint_eq(result, expected);
}
END_TEST

START_TEST(test_s21_strspn_null_str1) {
  const char *str1 = s21_NULL;
  const char *str2 = "Hel";
  s21_size_t result = s21_strspn(str1, str2);
  ck_assert_uint_eq(result, 0);
}
END_TEST

// TEST STRCPY

START_TEST(test_s21_strcpy_non_empty_string) {
  const char *src = "Hello, world!";
  char dest1[20];
  char dest2[20];

  char *result1 = s21_strcpy(dest1, src);
  char *result2 = strcpy(dest2, src);

  ck_assert_ptr_eq(result1, dest1);
  ck_assert_ptr_eq(result2, dest2);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_s21_strcpy_empty_string) {
  const char *src = "";
  char dest1[20];
  char dest2[20];

  char *result1 = s21_strcpy(dest1, src);
  char *result2 = strcpy(dest2, src);

  ck_assert_ptr_eq(result1, dest1);
  ck_assert_ptr_eq(result2, dest2);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// TEST MEMMOVE

START_TEST(test_s21_memmove_non_overlap) {
  unsigned char src[10] = "abcdefghij";
  unsigned char dest[10] = "abcde";
  unsigned char dest_orig[10] = "abcde";
  unsigned char expected[10] = "abcdeabcde";

  s21_memmove(dest + 5, src, 5);
  memmove(dest_orig + 5, src, 5);
  ck_assert_mem_eq(dest, expected, 10);
  ck_assert_mem_eq(dest_orig, expected, 10);
}
END_TEST

START_TEST(test_s21_memmove_overlap_forward) {
  char data[] = "Hello, world!";
  char data_orig[] = "Hello, world!";

  s21_memmove(data + 1, data, 11);
  memmove(data_orig + 1, data_orig, 11);
  ck_assert_str_eq(data, data_orig);
}
END_TEST

START_TEST(test_s21_memmove_overlap_backward) {
  char data[] = "Hello, world!";
  char data_orig[] = "Hello, world!";

  s21_memmove(data, data + 1, 11);
  memmove(data_orig, data_orig + 1, 11);
  ck_assert_str_eq(data, data_orig);
}
END_TEST

START_TEST(test_s21_memmove_zero_size) {
  char data[] = "Hello, world!";
  char data_orig[] = "Hello, world!";
  char *result;
  char *result_orig;

  result = s21_memmove(data, data, 0);
  result_orig = memmove(data_orig, data_orig, 0);

  ck_assert_ptr_eq(result, data);
  ck_assert_ptr_eq(result_orig, data_orig);
  ck_assert_str_eq(data, data_orig);
}
END_TEST

/* -----===== PART-2 =====----- */
/* -----===== START =====----- */

// TEST %C
START_TEST(test_base) {
  char data_orig[BUFFER_SIZE] = {'\0'};
  char data[BUFFER_SIZE] = {'\0'};
  char c = 'h';
  char w = 'w';
  s21_sprintf(data, "hello %c %c  ", w, c);
  sprintf(data_orig, "hello %c %c  ", w, c);
  ck_assert_str_eq(data, data_orig);

  s21_sprintf(data, "hello %-c %c  ", w, c);
  sprintf(data_orig, "hello %-c %c  ", w, c);
  ck_assert_str_eq(data, data_orig);

  s21_sprintf(data, "hello %10c %c  ", w, c);
  sprintf(data_orig, "hello %10c %c  ", w, c);
  ck_assert_str_eq(data, data_orig);

  s21_sprintf(data, "hello %10c %-c  ", w, c);
  sprintf(data_orig, "hello %10c %-c  ", w, c);
  ck_assert_str_eq(data, data_orig);

  s21_sprintf(data, "hello %10c %-c  ", w, c);
  sprintf(data_orig, "hello %10c %-c  ", w, c);
  ck_assert_str_eq(data, data_orig);

  setlocale(LC_ALL, "");
  wint_t ch = L'あ';
  wint_t ru = L'ф';
  s21_sprintf(data, "%lc", ch);
  sprintf(data_orig, "%lc", ch);
  ck_assert_str_eq(data, data_orig);

  s21_sprintf(data, "%lc", ru);
  sprintf(data_orig, "%lc", ru);
  ck_assert_str_eq(data, data_orig);
  setlocale(LC_ALL, "C");
}
END_TEST

// TEST D AND I

START_TEST(test_int_d_variations) {
  int num = 4233;
  long int num_long = 12345678910111213;
  char data_orig[BUFFER_SIZE] = {'\0'};
  char data[BUFFER_SIZE] = {'\0'};

  sprintf(data_orig, "Number: %ld", LONG_MAX);
  s21_sprintf(data, "Number: %ld", LONG_MAX);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %d", INT_MAX);
  s21_sprintf(data, "Number: %d", INT_MAX);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %d", num);
  s21_sprintf(data, "Number: %d", num);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %10d", num);
  s21_sprintf(data, "Number: %10d", num);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %0d", num);
  s21_sprintf(data, "Number: %0d", num);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %ld", num_long);
  s21_sprintf(data, "Number: %ld", num_long);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %+ld", num_long);
  s21_sprintf(data, "Number: %+ld", num_long);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %03ld", num_long);
  s21_sprintf(data, "Number: %03ld", num_long);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %+d", num);
  s21_sprintf(data, "Number: %+d", num);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: % d", num);
  s21_sprintf(data, "Number: % d", num);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %08d", num);
  s21_sprintf(data, "Number: %08d", num);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %-+8d", num);
  s21_sprintf(data, "Number: %-+8d", num);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %08d, same number: %08i", num, num);
  s21_sprintf(data, "Number: %08d, same number: %08i", num, num);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %+08d, same number: %+08i", num, num);
  s21_sprintf(data, "Number: %+08d, same number: %+08i", num, num);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: % 08d, same number: % 08i", num, num);
  s21_sprintf(data, "Number: % 08d, same number: % 08i", num, num);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %- 8d, same number: %- 8i", num, num);
  s21_sprintf(data, "Number: %- 8d, same number: %- 8i", num, num);
  ck_assert_str_eq(data, data_orig);
}
END_TEST

// TEST %S

START_TEST(test_string_variation) {
  char data_orig[BUFFER_SIZE] = {'\0'};
  char data[BUFFER_SIZE] = {'\0'};
  char *str = "Hello, World!";
  wchar_t *wide_str = L"Привет, мир!";

  sprintf(data_orig, "Number: %s", str);
  s21_sprintf(data, "Number: %s", str);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "The string is '%.5s'", str);
  s21_sprintf(data, "The string is '%.5s'", str);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "The string is '%20s'", str);
  s21_sprintf(data, "The string is '%20s'", str);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "The percent symbol is %% %%");
  s21_sprintf(data, "The percent symbol is %% %%");
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "String: %ls", wide_str);
  s21_sprintf(data, "String: %ls", wide_str);
  ck_assert_str_eq(data, data_orig);
}
END_TEST

// TEST %f

START_TEST(test_float) {
  long double ld_value = 12321131.512312355533333333556L;
  char data_orig[BUFFER_SIZE] = {'\0'};
  char data[BUFFER_SIZE] = {'\0'};
  double q = 123.4563333;

  sprintf(data_orig, "%#f", 123.11);
  s21_sprintf(data, "%#f", 123.11);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%f", -123.11);
  s21_sprintf(data, "%f", -123.11);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%.8f", 1111123.11233444);
  s21_sprintf(data, "%.8f", 1111123.11233444);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%f", 12311.11233444);
  s21_sprintf(data, "%f", 12311.11233444);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%20f", 11.112);
  s21_sprintf(data, "%20f", 11.112);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%.2f", q);
  s21_sprintf(data, "%.2f", q);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%10.2f", q);
  s21_sprintf(data, "%10.2f", q);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%+10.2f", q);
  s21_sprintf(data, "%+10.2f", q);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%+f", q);
  s21_sprintf(data, "%+f", q);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%-10.2f", q);
  s21_sprintf(data, "%-10.2f", q);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%#5f", q);
  s21_sprintf(data, "%#5f", q);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%-+8.3f", q);
  s21_sprintf(data, "%-+8.3f", q);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%.*f", 2, q);
  s21_sprintf(data, "%.*f", 2, q);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "% f", -12311.11233444);
  s21_sprintf(data, "% f", -12311.11233444);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "% f", 12311.11233444);
  s21_sprintf(data, "% f", 12311.11233444);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%lf", 123.11);
  s21_sprintf(data, "%lf", 123.11);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%.15lf", 123.11);
  s21_sprintf(data, "%.15lf", 123.11);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%Lf", ld_value);
  s21_sprintf(data, "%Lf", ld_value);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "%.15Lf", ld_value);
  s21_sprintf(data, "%.15Lf", ld_value);
  ck_assert_str_eq(data, data_orig);
}
END_TEST

// TEST %u

START_TEST(test_uint_basic) {
  char data_orig[BUFFER_SIZE] = {'\0'};
  char data[BUFFER_SIZE] = {'\0'};
  unsigned num = 42;
  unsigned long num_long = 1234455;
  unsigned short num_short = 123;

  sprintf(data_orig, "Number: %u", num);
  s21_sprintf(data, "Number: %u", num);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %lu", num_long);
  s21_sprintf(data, "Number: %lu", num_long);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %hu", num_short);
  s21_sprintf(data, "Number: %hu", num_short);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Number: %03lu", num_long);
  s21_sprintf(data, "Number: %03lu", num_long);
  ck_assert_str_eq(data, data_orig);
}
END_TEST

START_TEST(test_uint_large) {
  char data_orig[BUFFER_SIZE] = {'\0'};
  char data[BUFFER_SIZE] = {'\0'};
  unsigned int num = 4294967295;  // UINT_MAX

  sprintf(data_orig, "Large number: %u", num);
  s21_sprintf(data, "Large number: %u", num);
  ck_assert_str_eq(data, data_orig);

  sprintf(data_orig, "Large number: %lu", ULONG_MAX);
  s21_sprintf(data, "Large number: %lu", ULLONG_MAX);
}
END_TEST

START_TEST(test_uint_width) {
  char data_orig[BUFFER_SIZE] = {'\0'};
  char data[BUFFER_SIZE] = {'\0'};
  unsigned int num = 42;

  sprintf(data_orig, "Padded number: %10u", num);
  s21_sprintf(data, "Padded number: %10u", num);
  ck_assert_str_eq(data, data_orig);
  sprintf(data_orig, "Padded number: %-10u", num);
  s21_sprintf(data, "Padded number: %-10u", num);
  ck_assert_str_eq(data, data_orig);
}
END_TEST

START_TEST(test_uint_zero_padding) {
  char data_orig[BUFFER_SIZE] = {'\0'};
  char data[BUFFER_SIZE] = {'\0'};
  unsigned int num = 42;

  sprintf(data_orig, "Zero-padded number: %010u", num);
  s21_sprintf(data, "Zero-padded number: %010u", num);
  ck_assert_str_eq(data, data_orig);
}

// TEST %e %E
END_TEST

START_TEST(test_e_and_E) {
  char data_orig[BUFFER_SIZE] = {'\0'};
  char data[BUFFER_SIZE] = {'\0'};
  float value = 12345.6789;
  float value_2 = -12345.6789;

  sprintf(data_orig, "%e", value);
  s21_sprintf(data, "%e", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%#e", value);
  s21_sprintf(data, "%#e", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%E", value);
  s21_sprintf(data, "%E", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "% e", value);
  s21_sprintf(data, "% e", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "% E", value);
  s21_sprintf(data, "% E", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%+e", value);
  s21_sprintf(data, "%+e", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%+E", value);
  s21_sprintf(data, "%+E", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%0e", value);
  s21_sprintf(data, "%0e", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%0E", value);
  s21_sprintf(data, "%0E", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%.3e", value);
  s21_sprintf(data, "%.3e", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%10e", value);
  s21_sprintf(data, "%10e", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%10E", value);
  s21_sprintf(data, "%10E", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "% 10e", value);
  s21_sprintf(data, "% 10e", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%.*e", 5, value);
  s21_sprintf(data, "%.*e", 5, value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%.*E", 5, value);
  s21_sprintf(data, "%.*E", 5, value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%.*e", -20, value);
  s21_sprintf(data, "%.*e", -20, value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%.*E", -20, value);
  s21_sprintf(data, "%.*E", -20, value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%+.*e", -20, value);
  s21_sprintf(data, "%+.*e", -20, value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%-.*e", -20, value_2);
  s21_sprintf(data, "%-.*e", -20, value_2);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%-10e", value_2);
  s21_sprintf(data, "%-10e", value_2);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%Le", LDBL_MAX);
  s21_sprintf(data, "%Le", LDBL_MAX);
  ck_assert_str_eq(data_orig, data);
}
END_TEST

// TEST %g %G

START_TEST(test_g_and_G_formatting) {
  char data_orig[BUFFER_SIZE] = {'\0'};
  char data[BUFFER_SIZE] = {'\0'};

  sprintf(data_orig, "%g", 0.0123);
  s21_sprintf(data, "%g", 0.0123);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%0g", 0.0123);
  s21_sprintf(data, "%0g", 0.0123);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%g", 1234531.2120300);
  s21_sprintf(data, "%g", 1234531.21203000);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%G", 1234531.2120300);
  s21_sprintf(data, "%G", 1234531.21203000);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%#g", 1234531.2120);
  s21_sprintf(data, "%#g", 1234531.2120);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%.2g", 123456.022);
  s21_sprintf(data, "%.2g", 123456.022);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%#g", 12345678.121110);
  s21_sprintf(data, "%#g", 12345678.121110);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%#g", 123456.1211101);
  s21_sprintf(data, "%#g", 123456.1211101);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%.2g", 123456.0);
  s21_sprintf(data, "%.2g", 123456.0);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%10g", 12345.1);
  s21_sprintf(data, "%10g", 12345.1);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%10g", 1.12365789);
  s21_sprintf(data, "%10g", 1.12365789);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%30g", 123456.123);
  s21_sprintf(data, "%30g", 123456.123);
  ck_assert_str_eq(data_orig, data);
}
END_TEST

// TEST %x %X
START_TEST(test_hex_formatting) {
  char data_orig[BUFFER_SIZE] = {'\0'};
  char data[BUFFER_SIZE] = {'\0'};
  unsigned value = 0x123abc;
  unsigned long value_long = 0x123abc;

  sprintf(data_orig, "%-20x", value);
  s21_sprintf(data, "%-20x", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%20x", value);
  s21_sprintf(data, "%20x", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%x", value);
  s21_sprintf(data, "%x", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%lx", value_long);
  s21_sprintf(data, "%lx", value_long);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%lX", value_long);
  s21_sprintf(data, "%lX", value_long);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%X", value);
  s21_sprintf(data, "%X", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%#x", value);
  s21_sprintf(data, "%#x", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%#X", value);
  s21_sprintf(data, "%#X", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%030x", value);
  s21_sprintf(data, "%030x", value);
  ck_assert_str_eq(data_orig, data);
}
END_TEST

// TEST %O
START_TEST(test_o) {
  char data_orig[BUFFER_SIZE] = {'\0'};
  char data[BUFFER_SIZE] = {'\0'};
  unsigned int value = 1234567;

  sprintf(data_orig, "%o", value);
  s21_sprintf(data, "%o", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%-10o", value);
  s21_sprintf(data, "%-10o", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%10o", value);
  s21_sprintf(data, "%10o", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%010o", value);
  s21_sprintf(data, "%010o", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%#o", value);
  s21_sprintf(data, "%#o", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%#010o", value);
  s21_sprintf(data, "%#010o", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%#10o", value);
  s21_sprintf(data, "%#10o", value);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%-#10o", value);
  s21_sprintf(data, "%-#10o", value);
  ck_assert_str_eq(data_orig, data);
}
END_TEST

// TEST %p
START_TEST(test_p) {
  char data_orig[BUFFER_SIZE] = {'\0'};
  char data[BUFFER_SIZE] = {'\0'};

  int var = 10;
  void *ptr = &var;
  sprintf(data_orig, "%p", ptr);
  s21_sprintf(data, "%p", ptr);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%15p", ptr);
  s21_sprintf(data, "%15p", ptr);
  ck_assert_str_eq(data_orig, data);

  sprintf(data_orig, "%-15p", ptr);
  s21_sprintf(data, "%-15p", ptr);
  ck_assert_str_eq(data_orig, data);
}
END_TEST

// TEST %n

START_TEST(test_n) {
  char data_orig[BUFFER_SIZE] = {'\0'};
  char data[BUFFER_SIZE] = {'\0'};

  int n1 = 0, n2 = 0;

  sprintf(data_orig, "Hello, %s! This is number %dn%n", "world", 42, &n1);
  s21_sprintf(data, "Hello, %s! This is number %dn%n", "world", 42, &n2);

  ck_assert_str_eq(data_orig, data);
  ck_assert_int_eq(n1, n2);

  sprintf(data_orig, "%10s%nhello", "world", &n1);
  s21_sprintf(data, "%10s%nhello", "world", &n2);

  ck_assert_str_eq(data_orig, data);
  ck_assert_int_eq(n1, n2);

  sprintf(data_orig, "%-10s%nhello", "world", &n1);
  s21_sprintf(data, "%-10s%nhello", "world", &n2);

  ck_assert_str_eq(data_orig, data);
  ck_assert_int_eq(n1, n2);

  sprintf(data_orig, "%.5s%nhello", "world", &n1);
  s21_sprintf(data, "%.5s%nhello", "world", &n2);

  ck_assert_str_eq(data_orig, data);
  ck_assert_int_eq(n1, n2);
}
END_TEST

/* -----===== PART-3 =====----- */
/* -----===== START =====----- */

START_TEST(test_empty_str) {
  char *str = "";
  char a;
  char b;
  int result1, result2;
  result1 = s21_sscanf(str, "%c", &a);
  result2 = sscanf(str, "%c", &b);
  ck_assert_int_eq(result1, result2);
}
END_TEST

START_TEST(test_c_v1) {
  char *str = " ";
  char a, b;
  char c, d;
  int result1, result2;
  result1 = s21_sscanf(str, "%c%c", &a, &b);
  result2 = sscanf(str, "%c%c", &c, &d);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, c);
}
END_TEST

START_TEST(test_c_v5) {
  char *str = "i c";
  char a, b;
  char c, d;
  int result1, result2;
  result1 = s21_sscanf(str, "%c%c", &a, &b);
  result2 = sscanf(str, "%c%c", &c, &d);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, c);
}
END_TEST

START_TEST(test_c_v2) {
  char *str = "c";
  char a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%c", &a);
  result2 = sscanf(str, "%c", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_c_v3) {
  char *str = "ic";
  char a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*c%c", &a);
  result2 = sscanf(str, "%*c%c", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_c_v4) {
  char *str = "ic";
  wchar_t a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%lc", &a);
  result2 = sscanf(str, "%lc", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_d_v1) {
  char *str = "";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%d", &a);
  result2 = sscanf(str, "%d", &b);
  ck_assert_int_eq(result1, result2);
}
END_TEST

START_TEST(test_d_v2) {
  char *str = "21";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%d", &a);
  result2 = sscanf(str, "%d", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_d_v4) {
  char *str = "21";
  short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%hd", &a);
  result2 = sscanf(str, "%hd", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_d_v5) {
  char *str = "2112121223";
  short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%4hd", &a);
  result2 = sscanf(str, "%4hd", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_d_v6) {
  char *str = "21";
  long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%ld", &a);
  result2 = sscanf(str, "%ld", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_d_v7) {
  char *str = "21212121";
  long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%4ld", &a);
  result2 = sscanf(str, "%4ld", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_d_v8) {
  char *str = "-21";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%d", &a);
  result2 = sscanf(str, "%d", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_d_v3) {
  char *str = "-21212121";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%4d", &a);
  result2 = sscanf(str, "%4d", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_d_v11) {
  char *str = "- 21";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%d", &a);
  result2 = sscanf(str, "%d", &b);
  ck_assert_int_eq(result1, result2);
}
END_TEST

START_TEST(test_d_v9) {
  char *str = "21 42";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*d %d", &a);
  result2 = sscanf(str, "%*d %d", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_d_v10) {
  char *str = "21 42";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*d%d", &a);
  result2 = sscanf(str, "%*d%d", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_d_c_combo_v1) {
  char *str = "21a";
  int a, b;
  char c, d;
  int result1, result2;
  result1 = s21_sscanf(str, "%d%c", &a, &c);
  result2 = sscanf(str, "%d%c", &b, &d);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(c, d);
}
END_TEST

START_TEST(test_d_c_combo_v2) {
  char *str = "21 a";
  int a, b;
  char c, d;
  int result1, result2;
  result1 = s21_sscanf(str, "%d%c", &a, &c);
  result2 = sscanf(str, "%d%c", &b, &d);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(c, d);
}
END_TEST

START_TEST(test_d_c_combo_v3) {
  char *str = "21 1";
  int a, b;
  char c, d;
  int result1, result2;
  result1 = s21_sscanf(str, "%d%c", &a, &c);
  result2 = sscanf(str, "%d%c", &b, &d);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(c, d);
}
END_TEST

START_TEST(test_i_v1) {
  char *str = "21";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%i", &a);
  result2 = sscanf(str, "%i", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_i_v3) {
  char *str = "21";
  short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%hi", &a);
  result2 = sscanf(str, "%hi", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_i_v4) {
  char *str = "2112121223";
  short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%4hi", &a);
  result2 = sscanf(str, "%4hi", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_i_v5) {
  char *str = "21";
  long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%li", &a);
  result2 = sscanf(str, "%li", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_i_v6) {
  char *str = "21212121";
  long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%4li", &a);
  result2 = sscanf(str, "%4li", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_i_v7) {
  char *str = "-21";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%i", &a);
  result2 = sscanf(str, "%i", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_i_v2) {
  char *str = "-21212121";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%4i", &a);
  result2 = sscanf(str, "%4i", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_i_v12) {
  char *str = "- 21";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%i", &a);
  result2 = sscanf(str, "%i", &b);
  ck_assert_int_eq(result1, result2);
}
END_TEST

START_TEST(test_i_v8) {
  char *str = "21 42";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*i %i", &a);
  result2 = sscanf(str, "%*i %i", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_i_v9) {
  char *str = "21 42";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*i%i", &a);
  result2 = sscanf(str, "%*i%i", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_i_v10) {
  char *str = "2f1 42";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*if%i", &a);
  result2 = sscanf(str, "%*if%i", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_i_v13) {
  char *str = "0xf12ad21";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%i", &a);
  result2 = sscanf(str, "%i", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_i_v16) {
  char *str = "-0xf12ad21";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%i", &a);
  result2 = sscanf(str, "%i", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, -0xf12ad21);
}
END_TEST

START_TEST(test_i_v17) {
  char *str = "-0xff";
  short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%hi", &a);
  result2 = sscanf(str, "%hi", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, -0xff);
}
END_TEST

START_TEST(test_i_v18) {
  char *str = "-0xf12ad2";
  long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%4li", &a);
  result2 = sscanf(str, "%4li", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_i_v19) {
  char *str = "-0xf12ad2 123c";
  int a, b, c, d;
  char e, f;
  int result1, result2;
  result1 = s21_sscanf(str, "%i%d%c", &a, &c, &e);
  result2 = sscanf(str, "%i%d%c", &b, &d, &f);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(c, d);
  ck_assert_int_eq(e, f);
  ck_assert_int_eq(a, -0xf12ad2);
  ck_assert_int_eq(c, 123);
  ck_assert_int_eq(e, 99);
}
END_TEST

START_TEST(test_i_v20) {
  char *str = "-0xf12ad2 123c";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*i%i", &a);
  result2 = sscanf(str, "%*i%i", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 123);
}
END_TEST

START_TEST(test_i_v21) {
  char *str = "-0123452 123c";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*i%i", &a);
  result2 = sscanf(str, "%*i%i", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 123);
}
END_TEST

START_TEST(test_i_v22) {
  char *str = "-01283452 123c";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*i%i", &a);
  result2 = sscanf(str, "%*i%i", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 83452);
}
END_TEST

START_TEST(test_i_v14) {
  char *str = "0xff";
  short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%hi", &a);
  result2 = sscanf(str, "%hi", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_i_v15) {
  char *str = "0xfffff";
  long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%li", &a);
  result2 = sscanf(str, "%li", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_i_v23) {
  char *str = "-0123452";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%i", &a);
  result2 = sscanf(str, "%i", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, -0123452);
}
END_TEST

START_TEST(test_i_v24) {
  char *str = "-0123452";
  long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%li", &a);
  result2 = sscanf(str, "%li", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, -0123452);
}
END_TEST

START_TEST(test_i_v25) {
  char *str = "-012345";
  short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%hi", &a);
  result2 = sscanf(str, "%hi", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, -012345);
}
END_TEST

START_TEST(test_i_v26) {
  char *str = "-012345";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%4i", &a);
  result2 = sscanf(str, "%4i", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, -012);
}
END_TEST

START_TEST(test_o_v1) {
  char *str = "01234567";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%o", &a);
  result2 = sscanf(str, "%o", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 01234567);
}
END_TEST

START_TEST(test_o_v3) {
  char *str = "0123456";
  unsigned short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%ho", &a);
  result2 = sscanf(str, "%ho", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 0123456);
}
END_TEST

START_TEST(test_o_v5) {
  char *str = "01234567 ";
  unsigned long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%lo", &a);
  result2 = sscanf(str, "%lo", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 01234567);
}
END_TEST

START_TEST(test_o_v2) {
  char *str = "-0123";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%o", &a);
  result2 = sscanf(str, "%o", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_o_v4) {
  char *str = "-0123";
  unsigned short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%ho", &a);
  result2 = sscanf(str, "%ho", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_o_v6) {
  char *str = "-0123";
  unsigned long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%lo", &a);
  result2 = sscanf(str, "%lo", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_combo_v1) {
  char *str =
      "Здесь присутствуют десятичные (42, 31), восьмеричные (052, 031), "
      "шестнадцатеричные (0x2A, 0X2a, 0x1F, 0X1f) ";
  int a, a2, e, e2;
  short int b, b2, f, f2;
  long int g, g2;
  unsigned int c, c2, d, d2;
  int result1, result2;
  result1 = s21_sscanf(str,
                       "Здесь присутствуют десятичные (%d, %2hd), восьмеричные "
                       "(%o, %3o), шестнадцатеричные (%i, %hi, %li, %*4i) ",
                       &a, &b, &c, &d, &e, &f, &g);
  result2 = sscanf(str,
                   "Здесь присутствуют десятичные (%d, %2hd), восьмеричные "
                   "(%o, %3o), шестнадцатеричные (%i, %hi, %li, %*4i) ",
                   &a2, &b2, &c2, &d2, &e2, &f2, &g2);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, a2);
  ck_assert_int_eq(b, b2);
  ck_assert_int_eq(c, c2);
  ck_assert_int_eq(d, d2);
  ck_assert_int_eq(e, e2);
  ck_assert_int_eq(f, f2);
  ck_assert_int_eq(g, g2);
}
END_TEST

START_TEST(test_o_v7) {
  char *str = "0123 04567";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*o%o", &a);
  result2 = sscanf(str, "%*o%o", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 04567);
}
END_TEST

START_TEST(test_x_v1) {
  char *str = "0x123 0x4567";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*x%x", &a);
  result2 = sscanf(str, "%*x%x", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 0x4567);
}
END_TEST

START_TEST(test_x_v5) {
  char *str = "0X123 0X4567";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*x%x", &a);
  result2 = sscanf(str, "%*x%x", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 0X4567);
}
END_TEST

START_TEST(test_x_v6) {
  char *str = "0X123 0X4567";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*x %x", &a);
  result2 = sscanf(str, "%*x %x", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 0X4567);
}
END_TEST

START_TEST(test_x_v7) {
  char *str = "0x1234567";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%x", &a);
  result2 = sscanf(str, "%x", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 0x1234567);
}
END_TEST

START_TEST(test_x_v2) {
  char *str = "0X1234567";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%X", &a);
  result2 = sscanf(str, "%X", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 0x1234567);
}
END_TEST

START_TEST(test_x_v3) {
  char *str = "0X1234567";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%x", &a);
  result2 = sscanf(str, "%x", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 0X1234567);
}
END_TEST

START_TEST(test_x_v4) {
  char *str = "0x1234567";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%X", &a);
  result2 = sscanf(str, "%X", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 0x1234567);
}
END_TEST

START_TEST(test_x_v8) {
  char *str = "0x1234";
  unsigned short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%hx", &a);
  result2 = sscanf(str, "%hx", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 0x1234);
}
END_TEST

START_TEST(test_x_v9) {
  char *str = "0x1234567";
  unsigned long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%lx", &a);
  result2 = sscanf(str, "%lx", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 0x1234567);
}
END_TEST

START_TEST(test_x_v10) {
  char *str = "-0x123";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%x", &a);
  result2 = sscanf(str, "%x", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_x_v11) {
  char *str = "-0x123";
  unsigned short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%hx", &a);
  result2 = sscanf(str, "%hx", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_x_v12) {
  char *str = "-0x123";
  unsigned long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%lx", &a);
  result2 = sscanf(str, "%lx", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_x_v13) {
  char *str = "-123";
  unsigned long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%lx", &a);
  result2 = sscanf(str, "%lx", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_u_v1) {
  char *str = "123";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%u", &a);
  result2 = sscanf(str, "%u", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 123);
}
END_TEST

START_TEST(test_u_v7) {
  char *str = "123a762";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%u", &a);
  result2 = sscanf(str, "%u", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 123);
}
END_TEST

START_TEST(test_u_v2) {
  char *str = "123 456";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*u%u", &a);
  result2 = sscanf(str, "%*u%u", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 456);
}
END_TEST

START_TEST(test_u_v3) {
  char *str = "123 456";
  unsigned int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*u %u", &a);
  result2 = sscanf(str, "%*u %u", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 456);
}
END_TEST

START_TEST(test_u_v4) {
  char *str = "123";
  unsigned short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%hu", &a);
  result2 = sscanf(str, "%hu", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 123);
}
END_TEST

START_TEST(test_u_v5) {
  char *str = "-123";
  unsigned short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%hu", &a);
  result2 = sscanf(str, "%hu", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_u_v6) {
  char *str = "12345";
  unsigned short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%4hu", &a);
  result2 = sscanf(str, "%4hu", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 1234);
}
END_TEST

START_TEST(test_u_v8) {
  char *str = "123";
  unsigned short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*u%hu", &a);
  result2 = sscanf(str, "%*u%hu", &b);
  ck_assert_int_eq(result1, result2);
}
END_TEST

START_TEST(test_u_v9) {
  char *str = "123 333";
  unsigned short int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*u%hu", &a);
  result2 = sscanf(str, "%*u%hu", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 333);
}
END_TEST

START_TEST(test_u_v10) {
  char *str = "123";
  unsigned long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%lu", &a);
  result2 = sscanf(str, "%lu", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 123);
}
END_TEST

START_TEST(test_u_v11) {
  char *str = "-123";
  unsigned long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%lu", &a);
  result2 = sscanf(str, "%lu", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_u_v12) {
  char *str = "12345";
  unsigned long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%4lu", &a);
  result2 = sscanf(str, "%4lu", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 1234);
}
END_TEST

START_TEST(test_u_v13) {
  char *str = "123";
  unsigned long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*u%lu", &a);
  result2 = sscanf(str, "%*u%lu", &b);
  ck_assert_int_eq(result1, result2);
}
END_TEST

START_TEST(test_u_v14) {
  char *str = "123 333";
  unsigned long int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*u%lu", &a);
  result2 = sscanf(str, "%*u%lu", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(a, 333);
}
END_TEST

START_TEST(test_s_v1) {
  char *str = "123 string 333";
  char a[20], b[20];
  int result1, result2;
  result1 = s21_sscanf(str, "%*d%19s%*d", a);
  result2 = sscanf(str, "%*d%19s%*d", b);
  ck_assert_int_eq(result1, result2);
  ck_assert_str_eq(a, b);
  ck_assert_str_eq(a, "string");
}
END_TEST

START_TEST(test_s_v2) {
  char *str = "123string333";
  char a[20], b[20];
  int result1, result2;
  result1 = s21_sscanf(str, "%*d%19s%*d", a);
  result2 = sscanf(str, "%*d%19s%*d", b);
  ck_assert_int_eq(result1, result2);
  ck_assert_str_eq(a, b);
  ck_assert_str_eq(a, "string333");
}
END_TEST

START_TEST(test_s_v3) {
  char *str = "123string";
  char a[20], b[20];
  int result1, result2;
  result1 = s21_sscanf(str, "%*d%19s%*d", a);
  result2 = sscanf(str, "%*d%19s%*d", b);
  ck_assert_int_eq(result1, result2);
  ck_assert_str_eq(a, b);
  ck_assert_str_eq(a, "string");
}
END_TEST

START_TEST(test_s_v8) {
  char *str = "123string 333";
  char a[20], b[20];
  int result1, result2;
  result1 = s21_sscanf(str, "%*1d%3s", a);
  result2 = sscanf(str, "%*1d%3s", b);
  ck_assert_int_eq(result1, result2);
  ck_assert_str_eq(a, b);
  ck_assert_str_eq(a, "23s");
}
END_TEST

START_TEST(test_s_v4) {
  char *str = "ignored string ignored";
  char a[20], b[20];
  int result1, result2;
  result1 = s21_sscanf(str, "%*s%19s%*s", a);
  result2 = sscanf(str, "%*s%19s%*s", b);
  ck_assert_int_eq(result1, result2);
  ck_assert_str_eq(a, b);
  ck_assert_str_eq(a, "string");
}
END_TEST

START_TEST(test_s_v5) {
  wchar_t wstr[100];
  swprintf(wstr, 100, L"Hello, world!");
  char str[20000];
  sprintf(str, "My message: %ls", wstr);
  wchar_t wstr1[100];
  wchar_t wstr2[100];
  int result1, result2;
  result1 = s21_sscanf(str, "My message: %99ls", wstr1);
  result2 = sscanf(str, "My message: %99ls", wstr2);
  ck_assert_int_eq(result1, result2);
  ck_assert_mem_eq(wstr1, wstr2, sizeof(wchar_t));
}
END_TEST

START_TEST(test_s_v7) {
  char *str = "";
  char a[20], b[20];
  int result1, result2;
  result1 = s21_sscanf(str, "%19s", a);
  result2 = sscanf(str, "%19s", b);
  ck_assert_int_eq(result1, result2);
}
END_TEST

START_TEST(test_n_v1) {
  char *str = "ignoredstrng ignored";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*s%n%*s", &a);
  result2 = sscanf(str, "%*s%n%*s", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST
START_TEST(test_n_v4) {
  char *str = "ignoredstrng ignored";
  int a, b;
  char str1[100];
  char str2[100];
  int result1, result2;
  result1 = s21_sscanf(str, "%99s%n", str1, &a);
  result2 = sscanf(str, "%99s%n", str2, &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_n_v2) {
  char *str = "ignoredstrng ignored";
  int a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*s %n%*s", &a);
  result2 = sscanf(str, "%*s %n%*s", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_n_v3) {
  char *str = "ignoredstrng igno red";
  int a, b;
  char s1[20], s2[20], s3[20], s4[20], s5[20], s6[20];
  int result1, result2;
  result1 = s21_sscanf(str, "%19s%19s%19s%n", s1, s3, s5, &a);
  result2 = sscanf(str, "%19s%19s%19s%n", s2, s4, s6, &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_str_eq(s1, s2);
  ck_assert_str_eq(s3, s4);
  ck_assert_str_eq(s5, s6);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_s_v6) {
  wchar_t wstr[100];
  swprintf(wstr, 100, L"Hello, world!");
  char str[20000];
  sprintf(str, "My message: %ls", wstr);
  wchar_t wstr1[100];
  wchar_t wstr2[100];
  int result1, result2;
  result1 = s21_sscanf(str, "My message: %3ls", wstr1);
  result2 = sscanf(str, "My message: %3ls", wstr2);
  ck_assert_int_eq(result1, result2);
  ck_assert_mem_eq(wstr1, wstr2, sizeof(wchar_t));
}
END_TEST

START_TEST(test_p_v1) {
  char *str = "0x7fff5b809abc";
  void *a, *b;
  int result1, result2;
  result1 = s21_sscanf(str, "%p", &a);
  result2 = sscanf(str, "%p", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_ptr_eq(a, b);
}
END_TEST

START_TEST(test_p_v2) {
  char *str = "0x7fff5b809abc 0x7ffd76d52a8c";
  void *a, *b, *c, *d;
  int result1, result2;
  result1 = s21_sscanf(str, "%p%p", &a, &b);
  result2 = sscanf(str, "%p%p", &c, &d);
  ck_assert_int_eq(result1, result2);
  ck_assert_ptr_eq(a, c);
  ck_assert_ptr_eq(b, d);
}
END_TEST

START_TEST(test_p_v3) {
  char *str = "0x7fff5b809abc 0x7ffd76d52a8c";
  void *a, *b, *c, *d;
  int result1, result2;
  result1 = s21_sscanf(str, "%p %p", &a, &b);
  result2 = sscanf(str, "%p %p", &c, &d);
  ck_assert_int_eq(result1, result2);
  ck_assert_ptr_eq(a, c);
  ck_assert_ptr_eq(b, d);
}
END_TEST

START_TEST(test_p_v4) {
  char *str = "0x7fff5b809abc 0x7ffd76d52a8c";
  void *a, *b;
  int result1, result2;
  result1 = s21_sscanf(str, "%*p %p", &a);
  result2 = sscanf(str, "%*p %p", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_ptr_eq(a, b);
}
END_TEST

START_TEST(test_f_v1) {
  char *str = "123.45";
  float a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%f", &a);
  result2 = sscanf(str, "%f", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_double_eq(a, b);
}
END_TEST

START_TEST(test_f_v2) {
  char *str = "-123.45";
  float a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%f", &a);
  result2 = sscanf(str, "%f", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_double_eq(a, b);
}
END_TEST

START_TEST(test_f_v3) {
  char *str = "-123.45e-2";
  float a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%f", &a);
  result2 = sscanf(str, "%f", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_double_eq(a, b);
}
END_TEST

START_TEST(test_f_v4) {
  char *str = "-123.45e2";
  float a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%f", &a);
  result2 = sscanf(str, "%f", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_double_eq(a, b);
}
END_TEST

START_TEST(test_f_v5) {
  char *str = "-123.45e+2";
  float a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%f", &a);
  result2 = sscanf(str, "%f", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_double_eq(a, b);
}
END_TEST

START_TEST(test_f_v6) {
  char *str = "-123.45e+2";
  long double a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%Lf", &a);
  result2 = sscanf(str, "%Lf", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_double_eq(a, b);
}
END_TEST

START_TEST(test_f_v7) {
  char *str = "-1234567891234.123345678838485726";
  long double a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%Lf", &a);
  result2 = sscanf(str, "%Lf", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_double_eq(a, b);
}
END_TEST

START_TEST(test_f_v8) {
  char *str = "-1234567891234.123345678838485726";
  long double a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%10Lf", &a);
  result2 = sscanf(str, "%10Lf", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_double_eq(a, b);
}
END_TEST

START_TEST(test_f_v9) {
  char *str = "-1234567891234.123345678838485726";
  long double a, b;
  int result1, result2;
  result1 = s21_sscanf(str, "%Lg", &a);
  result2 = sscanf(str, "%Lg", &b);
  ck_assert_int_eq(result1, result2);
  ck_assert_double_eq(a, b);
}
END_TEST

START_TEST(test_f_v10) {
  char *str = " -123.45 -67.89";
  long double a, b, c, d;
  int result1, result2;
  result1 = s21_sscanf(str, "%Lg%Lf", &a, &c);
  result2 = sscanf(str, "%Lg%Lf", &b, &d);
  ck_assert_int_eq(result1, result2);
  ck_assert_double_eq(a, b);
  ck_assert_double_eq(c, d);
}
END_TEST

START_TEST(test_f_v11) {
  char *str = " -123.45 -67.89";
  double a, b, c, d;
  int result1, result2;
  result1 = s21_sscanf(str, "%le%lf", &a, &c);
  result2 = sscanf(str, "%le%lf", &b, &d);
  ck_assert_int_eq(result1, result2);
  ck_assert_double_eq(a, b);
  ck_assert_double_eq(c, d);
}
END_TEST

START_TEST(test_combo_v2) {
  char *data = "42 3.14 Hello";
  int a, b;
  float c, d;
  char e[20], f[20];
  int result1, result2;
  result1 = s21_sscanf(data, "%d %f %s", &a, &c, e);
  result2 = s21_sscanf(data, "%d %f %s", &b, &d, f);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a, b);
  ck_assert_double_eq(c, d);
  ck_assert_str_eq(e, f);
}
END_TEST

START_TEST(test_combo_v3) {
  char *data = "0xFF 0xABC 0x123";
  int a1, b1, c1;
  int a2, b2, c2;
  int result1, result2;
  result1 = s21_sscanf(data, "%x %x %x", &a1, &b1, &c1);
  result2 = s21_sscanf(data, "%x %x %x", &a2, &b2, &c2);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
  ck_assert_int_eq(c1, c2);
}
END_TEST

START_TEST(test_combo_v4) {
  char *data = "3.14 2.718 1.414";
  float a1, b1, c1;
  float a2, b2, c2;
  int result1, result2;
  result1 = s21_sscanf(data, "%f %e %g", &a1, &b1, &c1);
  result2 = s21_sscanf(data, "%f %e %g", &a2, &b2, &c2);
  ck_assert_int_eq(result1, result2);
  ck_assert_double_eq(a1, a2);
  ck_assert_double_eq(b1, b2);
  ck_assert_double_eq(c1, c2);
}
END_TEST

START_TEST(test_combo_v5) {
  char *data = "A B C";
  char a1, b1[20], c1;
  char a2, b2[20], c2;
  int result1, result2;
  result1 = s21_sscanf(data, "%c %s %c", &a1, b1, &c1);
  result2 = s21_sscanf(data, "%c %s %c", &a2, b2, &c2);
  ck_assert_int_eq(result1, result2);
  ck_assert_int_eq(a1, a2);
  ck_assert_str_eq(b1, b2);
  ck_assert_int_eq(c1, c2);
}
END_TEST

/* -----===== PART-5 =====----- */

// TEST TO_UPPER

START_TEST(test_s21_to_upper_null) {
  const char *str1 = NULL;
  char *result = s21_to_upper(str1);
  ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_s21_to_upper_v1) {
  const char *str1 = "big";
  char *result = s21_to_upper(str1);
  ck_assert_str_eq(result, "BIG");
  free(result);
}
END_TEST

START_TEST(test_s21_to_upper_v2) {
  const char *str1 = "bIG one!";
  char *result = s21_to_upper(str1);
  ck_assert_str_eq(result, "BIG ONE!");
  free(result);
}
END_TEST

START_TEST(test_s21_to_upper_v3) {
  const char *str1 = "BIg";
  char *result = s21_to_upper(str1);
  ck_assert_str_eq(result, "BIG");
  free(result);
}
END_TEST

// TEST TO_LOWER

START_TEST(test_s21_to_lower_null) {
  const char *str1 = NULL;
  char *result = s21_to_lower(str1);
  ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_s21_to_lower_v1) {
  const char *str1 = "small";
  char *result = s21_to_lower(str1);
  ck_assert_str_eq(result, "small");
  free(result);
}
END_TEST

START_TEST(test_s21_to_lower_v2) {
  const char *str1 = "SMaLL one!";
  char *result = s21_to_lower(str1);
  ck_assert_str_eq(result, "small one!");
  free(result);
}
END_TEST

START_TEST(test_s21_to_lower_v3) {
  const char *str1 = "BIg";
  char *result = s21_to_lower(str1);
  ck_assert_str_eq(result, "big");
  free(result);
}
END_TEST

START_TEST(test_s21_insert_null_null) {
  const char *str1 = NULL;
  const char *str2 = NULL;
  char *result = s21_insert(str1, str2, 0);
  ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_s21_insert_v1) {
  const char *str1 = "insert><here";
  const char *str2 = "right";
  char *result = s21_insert(str1, str2, 7);
  ck_assert_str_eq(result, "insert>right<here");
  free(result);
}
END_TEST

START_TEST(test_s21_insert_v2_out_of_range) {
  const char *str1 = "insert><here";
  const char *str2 = "right";
  char *result = s21_insert(str1, str2, 20);
  ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_s21_insert_v3_out_of_range) {
  const char *str1 = "insert><here";
  const char *str2 = "right";
  char *result = s21_insert(str1, str2, -1);
  ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_s21_trim_v1) {
  const char *str1 = "not_okey";
  const char *str2 = "not";
  char *result = s21_trim(str1, str2);
  ck_assert_str_eq(result, "_okey");
  free(result);
}
END_TEST

START_TEST(test_s21_trim_v2) {
  const char *str1 = "not_okey";
  const char *str2 = "otn";
  char *result = s21_trim(str1, str2);
  ck_assert_str_eq(result, "_okey");
  free(result);
}
END_TEST

START_TEST(test_s21_trim_v3) {
  const char *str1 = "where is this";
  const char *str2 = "this";
  char *result = s21_trim(str1, str2);
  ck_assert_str_eq(result, "where is ");
  free(result);
}
END_TEST

START_TEST(test_s21_trim_v4) {
  const char *str1 = "where is this";
  const char *str2 = "sthi";
  char *result = s21_trim(str1, str2);
  ck_assert_str_eq(result, "where is ");
  free(result);
}
END_TEST

START_TEST(test_s21_trim_v5) {
  const char *str1 = NULL;
  const char *str2 = "oops";
  char *result = s21_trim(str1, str2);
  ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_s21_trim_v6) {
  const char *str1 = "NULL";
  const char *str2 = NULL;
  char *result = s21_trim(str1, str2);
  ck_assert_ptr_null(result);
}
END_TEST

Suite *s21_string_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Core");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_memchr_fn);
  tcase_add_test(tc_core, test_s21_memchr_fn_null);
  tcase_add_test(tc_core, test_s21_memchr_fn_compare);

  tcase_add_test(tc_core, test_empty_strings);
  tcase_add_test(tc_core, test_different_lengths);
  tcase_add_test(tc_core, test_same_strings);
  tcase_add_test(tc_core, test_different_strings_negative);
  tcase_add_test(tc_core, test_different_strings_positive);

  tcase_add_test(tc_core, test_s21_memcpy_basic);
  tcase_add_test(tc_core, test_s21_memcpy_non_overlap);
  tcase_add_test(tc_core, test_s21_memcpy_zero_size);
  tcase_add_test(tc_core, test_s21_memcpy_large_size);

  tcase_add_test(tc_core, test_s21_memset_normal_case);
  tcase_add_test(tc_core, test_s21_memset_zero_length);
  tcase_add_test(tc_core, test_s21_memset_full_length);
  tcase_add_test(tc_core, test_s21_memset_null_pointer);

  tcase_add_test(tc_core, test_s21_strncat_normal_case);
  tcase_add_test(tc_core, test_s21_strncat_less_than_n);
  tcase_add_test(tc_core, test_s21_strncat_zero_length);
  tcase_add_test(tc_core, test_s21_strncat_null_dest);

  tcase_add_test(tc_core, test_s21_strchr_found);
  tcase_add_test(tc_core, test_s21_strchr_not_found);
  tcase_add_test(tc_core, test_s21_strchr_null_termination);
  tcase_add_test(tc_core, test_s21_strchr_null_string);

  tcase_add_test(tc_core, test_s21_strncmp_equal_strings);
  tcase_add_test(tc_core, test_s21_strncmp_unequal_strings);
  tcase_add_test(tc_core, test_s21_strncmp_empty_strings);
  tcase_add_test(tc_core, test_s21_strncmp_zero_n);

  tcase_add_test(tc_core, test_s21_strncpy_non_empty_string);
  tcase_add_test(tc_core, test_s21_strncpy_full_copy);
  tcase_add_test(tc_core, test_s21_strncpy_empty_string);

  tcase_add_test(tc_core, test_s21_strcspn_no_matching_chars);
  tcase_add_test(tc_core, test_s21_strcspn_some_matching_chars);

  tcase_add_test(tc_core, test_strerror);

  tcase_add_test(tc_core, test_s21_strlen_empty_string);
  tcase_add_test(tc_core, test_s21_strlen_nonempty_string);

  tcase_add_test(tc_core, test_s21_strpbrk_found);
  tcase_add_test(tc_core, test_s21_strpbrk_not_found);
  tcase_add_test(tc_core, test_s21_strpbrk_null_str1);

  tcase_add_test(tc_core, test_s21_strrchr_found);
  tcase_add_test(tc_core, test_s21_strrchr_not_found);
  tcase_add_test(tc_core, test_s21_strrchr_null_str);

  tcase_add_test(tc_core, test_s21_strstr_empty_needle);
  tcase_add_test(tc_core, test_s21_strstr_full_match);
  tcase_add_test(tc_core, test_s21_strstr_partial_match);

  tcase_add_test(tc_core, test_s21_strtok_basic);

  tcase_add_test(tc_core, test_s21_strspn_matching);
  tcase_add_test(tc_core, test_s21_strspn_no_matching);
  tcase_add_test(tc_core, test_s21_strspn_null_str1);

  tcase_add_test(tc_core, test_s21_memmove_non_overlap);
  tcase_add_test(tc_core, test_s21_memmove_overlap_forward);
  tcase_add_test(tc_core, test_s21_memmove_overlap_backward);
  tcase_add_test(tc_core, test_s21_memmove_zero_size);

  tcase_add_test(tc_core, test_s21_strcpy_non_empty_string);
  tcase_add_test(tc_core, test_s21_strcpy_empty_string);

  // SPRINTF
  tcase_add_test(tc_core, test_base);
  tcase_add_test(tc_core, test_int_d_variations);
  tcase_add_test(tc_core, test_string_variation);
  tcase_add_test(tc_core, test_float);
  tcase_add_test(tc_core, test_uint_basic);
  tcase_add_test(tc_core, test_uint_large);
  tcase_add_test(tc_core, test_uint_width);
  tcase_add_test(tc_core, test_uint_zero_padding);
  tcase_add_test(tc_core, test_e_and_E);
  tcase_add_test(tc_core, test_g_and_G_formatting);
  tcase_add_test(tc_core, test_hex_formatting);
  tcase_add_test(tc_core, test_o);
  tcase_add_test(tc_core, test_n);
  tcase_add_test(tc_core, test_p);

  // SSCANF

  tcase_add_test(tc_core, test_empty_str);

  tcase_add_test(tc_core, test_c_v1);
  tcase_add_test(tc_core, test_c_v2);
  tcase_add_test(tc_core, test_c_v3);
  tcase_add_test(tc_core, test_c_v4);
  tcase_add_test(tc_core, test_c_v5);

  tcase_add_test(tc_core, test_d_v1);
  tcase_add_test(tc_core, test_d_v2);
  tcase_add_test(tc_core, test_d_v3);
  tcase_add_test(tc_core, test_d_v4);
  tcase_add_test(tc_core, test_d_v5);
  tcase_add_test(tc_core, test_d_v6);
  tcase_add_test(tc_core, test_d_v7);
  tcase_add_test(tc_core, test_d_v8);
  tcase_add_test(tc_core, test_d_v9);
  tcase_add_test(tc_core, test_d_v10);
  tcase_add_test(tc_core, test_d_v11);

  tcase_add_test(tc_core, test_i_v1);
  tcase_add_test(tc_core, test_i_v2);
  tcase_add_test(tc_core, test_i_v3);
  tcase_add_test(tc_core, test_i_v4);
  tcase_add_test(tc_core, test_i_v5);
  tcase_add_test(tc_core, test_i_v6);
  tcase_add_test(tc_core, test_i_v7);
  tcase_add_test(tc_core, test_i_v8);
  tcase_add_test(tc_core, test_i_v9);
  tcase_add_test(tc_core, test_i_v10);
  tcase_add_test(tc_core, test_i_v12);
  tcase_add_test(tc_core, test_i_v13);
  tcase_add_test(tc_core, test_i_v14);
  tcase_add_test(tc_core, test_i_v15);
  tcase_add_test(tc_core, test_i_v16);
  tcase_add_test(tc_core, test_i_v17);
  tcase_add_test(tc_core, test_i_v18);
  tcase_add_test(tc_core, test_i_v19);
  tcase_add_test(tc_core, test_i_v20);
  tcase_add_test(tc_core, test_i_v21);
  tcase_add_test(tc_core, test_i_v22);
  tcase_add_test(tc_core, test_i_v23);
  tcase_add_test(tc_core, test_i_v24);
  tcase_add_test(tc_core, test_i_v25);
  tcase_add_test(tc_core, test_i_v26);

  tcase_add_test(tc_core, test_o_v1);
  tcase_add_test(tc_core, test_o_v2);
  tcase_add_test(tc_core, test_o_v3);
  tcase_add_test(tc_core, test_o_v4);
  tcase_add_test(tc_core, test_o_v5);
  tcase_add_test(tc_core, test_o_v6);
  tcase_add_test(tc_core, test_o_v7);

  tcase_add_test(tc_core, test_x_v1);
  tcase_add_test(tc_core, test_x_v2);
  tcase_add_test(tc_core, test_x_v3);
  tcase_add_test(tc_core, test_x_v4);
  tcase_add_test(tc_core, test_x_v5);
  tcase_add_test(tc_core, test_x_v6);
  tcase_add_test(tc_core, test_x_v7);
  tcase_add_test(tc_core, test_x_v8);
  tcase_add_test(tc_core, test_x_v9);
  tcase_add_test(tc_core, test_x_v10);
  tcase_add_test(tc_core, test_x_v11);
  tcase_add_test(tc_core, test_x_v12);
  tcase_add_test(tc_core, test_x_v13);

  tcase_add_test(tc_core, test_u_v1);
  tcase_add_test(tc_core, test_u_v2);
  tcase_add_test(tc_core, test_u_v3);
  tcase_add_test(tc_core, test_u_v4);
  tcase_add_test(tc_core, test_u_v5);
  tcase_add_test(tc_core, test_u_v6);
  tcase_add_test(tc_core, test_u_v7);
  tcase_add_test(tc_core, test_u_v8);
  tcase_add_test(tc_core, test_u_v9);
  tcase_add_test(tc_core, test_u_v10);
  tcase_add_test(tc_core, test_u_v11);
  tcase_add_test(tc_core, test_u_v12);
  tcase_add_test(tc_core, test_u_v13);
  tcase_add_test(tc_core, test_u_v14);

  tcase_add_test(tc_core, test_s_v1);
  tcase_add_test(tc_core, test_s_v2);
  tcase_add_test(tc_core, test_s_v3);
  tcase_add_test(tc_core, test_s_v4);
  tcase_add_test(tc_core, test_s_v5);
  tcase_add_test(tc_core, test_s_v6);
  tcase_add_test(tc_core, test_s_v7);
  tcase_add_test(tc_core, test_s_v8);

  tcase_add_test(tc_core, test_n_v1);
  tcase_add_test(tc_core, test_n_v2);
  tcase_add_test(tc_core, test_n_v3);
  tcase_add_test(tc_core, test_n_v4);

  tcase_add_test(tc_core, test_p_v1);
  tcase_add_test(tc_core, test_p_v2);
  tcase_add_test(tc_core, test_p_v3);
  tcase_add_test(tc_core, test_p_v4);

  tcase_add_test(tc_core, test_f_v1);
  tcase_add_test(tc_core, test_f_v2);
  tcase_add_test(tc_core, test_f_v3);
  tcase_add_test(tc_core, test_f_v4);
  tcase_add_test(tc_core, test_f_v5);
  tcase_add_test(tc_core, test_f_v6);
  tcase_add_test(tc_core, test_f_v7);
  tcase_add_test(tc_core, test_f_v8);
  tcase_add_test(tc_core, test_f_v9);
  tcase_add_test(tc_core, test_f_v10);
  tcase_add_test(tc_core, test_f_v11);

  tcase_add_test(tc_core, test_combo_v1);
  tcase_add_test(tc_core, test_combo_v2);
  tcase_add_test(tc_core, test_combo_v3);
  tcase_add_test(tc_core, test_combo_v4);
  tcase_add_test(tc_core, test_combo_v5);
  tcase_add_test(tc_core, test_d_c_combo_v1);
  tcase_add_test(tc_core, test_d_c_combo_v2);
  tcase_add_test(tc_core, test_d_c_combo_v3);

  tcase_add_test(tc_core, test_s21_to_upper_null);
  tcase_add_test(tc_core, test_s21_to_upper_v1);
  tcase_add_test(tc_core, test_s21_to_upper_v2);
  tcase_add_test(tc_core, test_s21_to_upper_v3);

  tcase_add_test(tc_core, test_s21_to_lower_null);
  tcase_add_test(tc_core, test_s21_to_lower_v1);
  tcase_add_test(tc_core, test_s21_to_lower_v2);
  tcase_add_test(tc_core, test_s21_to_lower_v3);

  tcase_add_test(tc_core, test_s21_insert_null_null);
  tcase_add_test(tc_core, test_s21_insert_v1);
  tcase_add_test(tc_core, test_s21_insert_v2_out_of_range);
  tcase_add_test(tc_core, test_s21_insert_v3_out_of_range);

  tcase_add_test(tc_core, test_s21_trim_v1);
  tcase_add_test(tc_core, test_s21_trim_v2);
  tcase_add_test(tc_core, test_s21_trim_v3);
  tcase_add_test(tc_core, test_s21_trim_v4);
  tcase_add_test(tc_core, test_s21_trim_v5);
  tcase_add_test(tc_core, test_s21_trim_v6);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = s21_string_suite();
  sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}