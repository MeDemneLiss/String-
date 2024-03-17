#ifndef S21_STRING
#define S21_STRING
#define s21_NULL (void *)0

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long s21_size_t;
s21_size_t s21_strlen(const char *str);
char *s21_strchr(const char *string, int symbol);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *sep);
char *s21_strerror(int errnum);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);
char *s21_strerror(int errnum);
s21_size_t s21_strcspn(const char *str1, const char *str2);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
void *s21_memchr(const void *str, int c, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
int s21_sscanf(const char *str, const char *format, ...);
int s21_sprintf(char *str, const char *format, ...);

typedef struct sscanf_format_specifier {
  int start;          // when we face new % sign
  short asterisk;     // * not to write the object read
  int width;          // means how many symbols are to be read
  char modifier;      // h, l, L
  int chars_scanned;  // all chars scanned for %n
  int success;        // number of successfully read objects
} sscanf_format_specifier;

void handle_c(va_list *argv, const char *str, sscanf_format_specifier *f);
void handle_s(va_list *argv, const char *str, sscanf_format_specifier *f);
void handle_d(va_list *argv, const char *str, sscanf_format_specifier *f,
              int sign);
void handle_i(va_list *argv, const char *str, sscanf_format_specifier *f);
void handle_o(va_list *argv, const char *str, sscanf_format_specifier *f,
              int sign, int if_signed);
void handle_x(va_list *argv, const char *str, sscanf_format_specifier *f,
              int sign, int if_signed);
void handle_u(va_list *argv, const char *str, sscanf_format_specifier *f);
void handle_e_f_g(va_list *argv, const char *str, sscanf_format_specifier *f);
void handle_p(va_list *argv, const char *str, sscanf_format_specifier *f);
long double from_str_to_int(const char **str, int dec_oct_hex_mant,
                            sscanf_format_specifier *f);
void read_char_to_int(const char *str, int dec_oct_hex_mant, long double *dest,
                      int base, int *len_mantissa);
void case_d_write_to_destination(va_list *argv,
                                 const sscanf_format_specifier *f, int *res,
                                 int if_signed);
void case_f_write_to_destination(va_list *argv,
                                 const sscanf_format_specifier *f,
                                 long double *res);
void skip_useless_chars(const char **str, sscanf_format_specifier *f);
void parse_sign(const char **str, sscanf_format_specifier *f, int *sign);

#define FLAG_PLUS 1     //+
#define FLAG_MINUS 2    //-
#define FLAG_SPACE 4    //' '
#define FLAG_LATTICE 8  // #
#define FLAG_0 16       // 0
#define FLAG_h 32       // h
#define FLAG_l 64       // l
#define FLAG_L 128      // L

typedef struct format_specifier {
  int width;     // 0 by default
  int accuracy;  // -1 by default
  int options;   // 0 by default
} format_specifier;

void scientific_notation(char *str, long double number, char ch, int accuracy);
void int_to_str(char *str, long number);
void o_specifier(char *str, long long unsigned int number);

char *get_number(char *str, int *rezult);
char *specifiers_processing(format_specifier f, va_list param, char specifier,
                            char *rezult_str, const char *str_run);
char *copy_str(char *str, char *new_str);
void hex_to_str(char *str, long long unsigned int number, char ch);
void float_to_str(char *str, long double number, int precision);
void str_width(char *str, format_specifier f);
void move_left(char *str, int n);
int s21_plus(char *str, format_specifier f);
void n_specifer(format_specifier f, va_list param, char *rezult_str,
                const char *str_run);
void s21_space(char *str, format_specifier f);
void p_specifer(char *str, void *ponter);
void s21_lattice(char *str, format_specifier f, char ch);
void flags_processing(char *str, format_specifier f, char specifier);
long long unsigned int length_u_int(format_specifier f, va_list param);
long length_int(format_specifier f, va_list param);
long double length_double(va_list param, format_specifier f);
void g_spesifer(char *str, long double n, char ch, format_specifier);
void accuracy_processing_int(char *str, int accuracy, char s);
void s_specif(char *str, char *new_str, int accuracy);

#endif