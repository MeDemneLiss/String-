#include "s21_string.h"

char *get_number(char *str, int *rezult) {
  *rezult = 0;
  while ((*str >= '0') && (*str <= '9')) {
    *rezult *= 10;
    *rezult += *str - '0';
    str++;
  }
  str--;
  return str;
}

char *specifiers_processing(format_specifier f, va_list param, char specifier,
                            char *rezult_str, const char *str_run) {
  char specifier_str[1000] = "\0\0\0";
  switch (specifier) {
    case 'd':
    case 'i':
      int_to_str(specifier_str, length_int(f, param));
      break;
    case 'e':
      scientific_notation(specifier_str, length_double(param, f), 'e',
                          f.accuracy);
      break;
    case 'E':
      scientific_notation(specifier_str, length_double(param, f), 'E',
                          f.accuracy);
      break;
    case 'f':
      float_to_str(specifier_str, length_double(param, f), f.accuracy);
      break;
    case 'g':
      g_spesifer(specifier_str, length_double(param, f), 'e', f);
      break;
    case 'G':
      g_spesifer(specifier_str, length_double(param, f), 'E', f);
      break;
    case 'o':
      o_specifier(specifier_str, length_u_int(f, param));
      break;
    case 'c':
      specifier_str[0] = va_arg(param, unsigned int);
      specifier_str[1] = 0;
      break;
    case 's':
      s_specif(specifier_str, va_arg(param, char *), f.accuracy);
      break;
    case 'u':
      int_to_str(specifier_str, length_u_int(f, param));
      break;
    case 'p':
      p_specifer(specifier_str, va_arg(param, void *));
      break;
    case 'x':
      hex_to_str(specifier_str, length_u_int(f, param), 'a');
      break;
    case 'X':
      hex_to_str(specifier_str, length_u_int(f, param), 'A');
      break;
    case 'n':
      n_specifer(f, param, rezult_str, str_run);
      break;
    case '%':
      specifier_str[0] = '%';
      break;
  }
  flags_processing(specifier_str, f, specifier);
  rezult_str = copy_str(rezult_str, specifier_str);
  if ((specifier_str[0] == 0) && (specifier == 'c')) {
    rezult_str++;
  }
  return rezult_str;
}

void n_specifer(format_specifier f, va_list param, char *rezult_str,
                const char *str_run) {
  if (f.options & FLAG_h) {
    *(va_arg(param, short *)) = rezult_str - str_run;
  } else if (f.options & FLAG_l) {
    *(va_arg(param, long int *)) = rezult_str - str_run;
  } else {
    *(va_arg(param, int *)) = rezult_str - str_run;
  }
}

void p_specifer(char *str, void *point) {
  str[0] = '0';
  str[1] = 'x';
  hex_to_str(str + 2, (unsigned long long)point, 'a');
}

void s_specif(char *str, char *new_str, int accuracy) {
  if (new_str != s21_NULL) {
    copy_str(str, new_str);
    if (accuracy < (int)s21_strlen(new_str) && (accuracy > -1)) {
      str[accuracy] = 0;
    }
  } else {
    str[0] = '(';
    str[1] = 'n';
    str[2] = 'u';
    str[3] = 'l';
    str[4] = 'l';
    str[5] = ')';
    str[6] = 0;
  }
}

void accuracy_processing_int(char *str, int accuracy, char s) {
  if (accuracy == -1) {
    accuracy = 1;
  }
  if ((!accuracy) && (s21_strlen(str) == 1) && (str[0] == '0')) {
    *str = '\0';
  }
  int len = (int)s21_strlen(str);
  int sign = 0;
  if ((str[0] == '-') || (str[0] == '+')) {
    sign = 1;
  }
  if (s == 'p') {
    sign = 2;
  }
  if ((accuracy - (len - sign)) > 0) {
    len = len - sign;
    move_left(str + sign, accuracy - len);
    for (int i = sign; i < (accuracy - len) + sign; i++) {
      str[i] = '0';
    }
  }
}

void g_spesifer(char *str, long double n, char ch, format_specifier f) {
  int minus = 0;
  if (n < 0) {
    minus = 1;
    n *= -1;
  }
  if (f.accuracy == -1) {
    f.accuracy = 6;
  }
  int length = 0;
  while (n < pow(10.0, f.accuracy)) {
    n *= 10.0;
    length++;
  }
  if ((long long)(n * 10.0) % 10 > 0) {
    n -= 0.1;
  }
  n = round(n) / pow(10.0, length);
  length = 1;
  for (long long k = (long long)n; k >= 10; k = k / 10) {
    length++;
  }
  if ((length > f.accuracy) && !((length == 1) && (f.accuracy == 0))) {
    if (f.accuracy < 2) {
      f.accuracy = 0;
    } else {
      f.accuracy = f.accuracy - 1;
    }
    scientific_notation(str, n, ch, f.accuracy);
    if (!(f.options & FLAG_LATTICE)) {
      int i = s21_strlen(str) - 5;
      for (; str[i] == '0'; i--) {
      }
      if (str[i] == '.') {
        i--;
      }
      str[i + 1] = str[s21_strlen(str) - 4];
      str[i + 2] = str[s21_strlen(str) - 3];
      str[i + 3] = str[s21_strlen(str) - 2];
      str[i + 4] = str[s21_strlen(str) - 1];
      str[i + 5] = 0;
    }

  } else {
    if (f.accuracy) {
      f.accuracy = f.accuracy - length;
    }
    float_to_str(str, n, f.accuracy);
    if (f.accuracy && !(f.options & FLAG_LATTICE)) {
      int i = s21_strlen(str) - 1;
      for (; str[i] == '0'; i--) {
        str[i] = 0;
      }
      if (str[i] == '.') {
        str[i] = 0;
      }
    }
  }
  if (minus) {
    move_left(str, 1);
    str[0] = '-';
  }
}

void move_left(char *str, int n) {
  int len = s21_strlen(str);
  for (int i = len; i >= 0; i--) {
    str[i + n] = str[i];
  }
}

void flags_processing(char *str, format_specifier f, char s) {
  if (f.accuracy != -1) {
    if ((s == 'd') || (s == 'i') || (s == 'o') || (s == 'u') || (s == 'x') ||
        (s == 'X') || (s == 'p')) {
      accuracy_processing_int(str, f.accuracy, s);
    }
  }
  if ((s != 'c') && (s != 'x') && (s != 'X') && (s != 'o') && (s != 's') &&
      (s != 'u')) {
    if (!s21_plus(str, f)) {
      s21_space(str, f);
    }
  }
  s21_lattice(str, f, s);
  if (f.width > 0) {
    str_width(str, f);
  }
}

void scientific_notation(char *str, long double number, char ch, int accuracy) {
  int count = 0;
  int sign = 1;
  if (number < 0) {
    sign = -1;
    number *= sign;
  }
  for (; number < 1.0; count--) {
    number = number * 10.0;
  }
  for (; number > 10.0; count++) {
    number = number * 0.1;
  }
  number *= sign;
  float_to_str(str, number, accuracy);
  if ((str[0] == '1') && (str[1] == '0')) {
    for (s21_size_t i = 1; i < s21_strlen(str); i++) {
      str[i] = str[i + 1];
    }
    count++;
  }
  if ((str[1] == '1') && (str[2] == '0')) {
    for (s21_size_t i = 2; i < s21_strlen(str); i++) {
      str[i] = str[i + 1];
    }
    count--;
  }
  int j = s21_strlen(str);
  *(str + j) = ch;
  if (count < 0) {
    *(str + j + 1) = '-';
    count *= -1;
  } else {
    *(str + j + 1) = '+';
  }
  if (count < 10 && count > -10) {
    *(str + j + 2) = '0';
    j++;
  }
  int_to_str(str + j + 2, count);
}

char *copy_str(char *str, char *new_str) {
  for (; *new_str != 0; new_str++, str++) {
    *str = *new_str;
  }
  *str = *new_str;
  return str--;
}

long double length_double(va_list param, format_specifier f) {
  long double number;
  if (f.options & FLAG_L) {
    number = va_arg(param, long double);
  } else {
    number = (long double)va_arg(param, double);
  }
  return number;
}

long length_int(format_specifier f, va_list param) {
  long number;
  if (f.options & FLAG_h) {
    number = (long)va_arg(param, int);
  } else if ((f.options & FLAG_l)) {
    number = va_arg(param, long);
  } else {
    number = (long)va_arg(param, int);
  }
  return number;
}

long long unsigned int length_u_int(format_specifier f, va_list param) {
  long long unsigned int number;
  if (f.options & FLAG_h) {
    number = (long long unsigned int)va_arg(param, unsigned int);
  } else if ((f.options & FLAG_l)) {
    number = va_arg(param, unsigned long long);
  } else {
    number = (unsigned long long)va_arg(param, unsigned int);
  }
  return number;
}

void hex_to_str(char *str, long long unsigned int number, char ch) {
  char str_conversely[99];
  int i = 0;
  if (number == 0) {
    str_conversely[0] = '0';
    i++;
  }
  for (; number > 0; i++) {
    int digit = number % 16;
    if (digit < 10) {
      str_conversely[i] = '0' + digit;
    } else {
      str_conversely[i] = ch + (digit - 10);
    }
    number = number / 16;
  }
  str_conversely[i] = 0;
  for (int j = i - 1; j >= 0; j--, str++) {
    *str = str_conversely[j];
  }
  *str = 0;
}

void float_to_str(char *str, long double number, int precision) {
  if (precision == -1) {
    precision = 6;
  }
  double accuracy_dec = pow(10, precision);
  if (!accuracy_dec) {
    accuracy_dec = 1;
  }
  number = roundl(number * accuracy_dec) / accuracy_dec;
  int_to_str(str, (long long)number);
  if (precision > 0) {
    if (number < 0) {
      number *= -1.0;
    }
    int point = s21_strlen(str);
    str[point] = '.';
    str[point + 1] = 0;
    while (((int)s21_strlen(str) - 1 - point < precision)) {
      number = roundl((number - (long long)number) * accuracy_dec) /
               accuracy_dec * 10.0;
      int new_ch = s21_strlen(str);
      long long int n = ((long long)floorl(number) % 10);
      str[new_ch] = '0' + n;
      str[new_ch + 1] = 0;
      number = number - n;
      accuracy_dec /= 10.0;
    }
  }
}

void int_to_str(char *str, long number) {
  int i = 0;
  char str2[90] = {0};
  long number2 = number;
  if (number < 0) {
    number = number * -1;
  }

  do {
    str2[i] = '0' + (number % 10);
    number /= 10;
    i++;
  } while (number > 0);
  if (number2 < 0) {
    str2[i] = '-';
  } else {
    i -= 1;
  }

  int length = i;
  for (int j = 0; j <= length; j++) {
    str[j] = str2[i];
    i--;
  }
  str[length + 1] = '\0';
}

void o_specifier(char *str, long long unsigned int number) {
  char str2[100];
  int i = 0;
  if (number == 0) {
    str2[0] = '0';
    i = 1;
  }
  while (number != 0) {
    str2[i] = '0' + (number % 8);
    number /= 8;
    i++;
  }
  i--;
  int length = i;
  for (int j = 0; j <= length; j++) {
    str[j] = str2[i];
    i--;
  }
  str[length + 1] = '\0';
}

void str_width(char *str, format_specifier f) {
  int count = s21_strlen(str);
  int n = f.width;
  if (n > count) {
    char ch = ' ';
    if (f.options & FLAG_MINUS) {
      for (int i = count; i < n; i++) {
        str[i] = ch;
      }
      str[n] = 0;
    } else {
      if (f.options & FLAG_0) {
        ch = '0';
      }
      int sign = 0;
      if (((str[0] == '-') || (str[0] == '+')) && (ch == '0')) {
        sign = 1;
      }
      count -= sign;
      for (int i = count; i >= sign; i--) {
        str[i + n - count - sign] = str[i];
      }
      for (int i = sign; i < n - count; i++) {
        str[i] = ch;
      }
    }
  }
}

int s21_plus(char *str, format_specifier f) {
  int rezult = 0;
  if (f.options & FLAG_PLUS) {
    rezult = 1;
    if (str[0] != '-') {
      int count = s21_strlen(str);
      for (int i = count; i >= 0; i--) {
        str[i + 1] = str[i];
      }
      str[0] = '+';
    }
  }
  return rezult;
}

void s21_space(char *str, format_specifier f) {
  if (f.options & FLAG_SPACE) {
    char c = ' ';
    int count = s21_strlen(str);
    if (str[0] != '-') {
      for (int i = count; i >= 0; i--) {
        str[i + 1] = str[i];
      }
      str[0] = c;
    }
  }
}

void s21_lattice(char *str, format_specifier f, char ch) {
  if ((f.options & FLAG_LATTICE) && (ch == 'o') &&
      ((s21_strlen(str) != 1) || (str[0] != '0'))) {
    char c = '0';
    int count = s21_strlen(str);

    for (int i = count; i >= 0; i--) {
      str[i + 1] = str[i];
    }
    str[0] = c;

  } else if (f.options & FLAG_LATTICE && (ch == 'x') &&
             ((s21_strlen(str) != 1) || (str[0] != '0'))) {
    int count = s21_strlen(str);

    for (int i = count; i >= 0; i--) {
      str[i + 2] = str[i];
    }
    str[0] = '0';
    str[1] = 'x';
  } else if (f.options & FLAG_LATTICE && (ch == 'X') &&
             ((s21_strlen(str) != 1) || (str[0] != '0'))) {
    int count = s21_strlen(str);

    for (int i = count; i >= 0; i--) {
      str[i + 2] = str[i];
    }
    str[0] = '0';
    str[1] = 'X';
  }

  if (f.options & FLAG_LATTICE && ((ch == 'f') || (ch == 'g') || (ch == 'G'))) {
    int pointer = 0;
    int len = s21_strlen(str);
    for (int i = 0; i < len; i++) {
      if (str[i] == '.') {
        pointer = 1;
      }
    }
    if (pointer != 1) {
      str[len] = '.';
    }
  }
  if (f.options & FLAG_LATTICE && ((ch == 'e') || (ch == 'E'))) {
    int pointer = 0;
    int len = s21_strlen(str);
    int i = 0;
    for (; (str[i] != 'e') && (str[i] != 'E'); i++) {
      if (str[i] == '.') {
        pointer = 1;
      }
    }
    if (!pointer) {
      for (int j = len; j >= i; j--) {
        str[j + 1] = str[j];
      }
      str[i] = '.';
    }
  }
}