#include "s21_string.h"

#include "error.h"

int s21_sscanf(const char *str, const char *format, ...) {
  va_list argv;
  va_start(argv, format);

  sscanf_format_specifier f = {0, 0, -1, '\0', 0, 0};
  do {
    int chars_scanned_before = f.chars_scanned;
    if (*format == '%') {
      f.start = 1;
      format++;
    }
    if (f.start) {
      if (*format == '*') {
        f.asterisk = 1;
      } else if (*format >= '0' && *format <= '9') {
        f.width = f.width == -1 ? *format - '0' : f.width * 10 + *format - '0';
      } else if (*format == 'h' || *format == 'l' || *format == 'L') {
        f.modifier = *format;
      } else {
        if (*format == 'c') {
          handle_c(&argv, str, &f);
        } else if (*format == 's') {
          handle_s(&argv, str, &f);
        } else if (*format == 'd') {
          handle_d(&argv, str, &f, 1);
        } else if (*format == 'u') {
          handle_u(&argv, str, &f);
        } else if (*format == 'i') {
          handle_i(&argv, str, &f);
        } else if (*format == 'o') {
          handle_o(&argv, str, &f, 1, 0);
        } else if (*format == 'x' || *format == 'X') {
          handle_x(&argv, str, &f, 1, 0);
        } else if (*format == 'e' || *format == 'E' || *format == 'f' ||
                   *format == 'g' || *format == 'G') {
          handle_e_f_g(&argv, str, &f);
        } else if (*format == 'p') {
          handle_p(&argv, str, &f);
        } else if (*format == '%') {
          while (*str != *format) {
            f.chars_scanned++;
            str++;
          }
          f.chars_scanned++;
          str++;
          chars_scanned_before = f.chars_scanned;
        } else if (*format == 'n') {
          case_d_write_to_destination(&argv, &f, &f.chars_scanned, 1);
        }
        f.start = 0;
        f.asterisk = 0;
        f.width = -1;
        f.modifier = '\0';
      }
    } else {
      while (*str != *format) {
        f.chars_scanned++;
        str++;
      }
      f.chars_scanned++;
      str++;
      chars_scanned_before = f.chars_scanned;
    }
    str += f.chars_scanned - chars_scanned_before;
  } while (*format++);
  va_end(argv);
  return f.success;
}

int s21_sprintf(char *str, const char *format, ...) {
  char *now_position = str;
  char flags[] = "+- #0hlL";
  va_list param;
  va_start(param, format);
  while (*format) {
    format_specifier f = {0, -1, 0};
    if (*format == '%') {
      format++;
      int run = 1;
      while (run) {
        if ((*format == '+') || (*format == '-') || (*format == ' ') ||
            (*format == '#') || (*format == '0') || (*format == 'h') ||
            (*format == 'l') || (*format == 'L')) {
          int shift = (1 << (s21_strchr(flags, *format) - flags));
          f.options |= shift;
        } else if (*format == '*') {
          f.width = va_arg(param, int);
          if (f.width < 0) {
            f.width *= -1;
            f.options |= FLAG_MINUS;
          }
        } else if (((*format >= '0') && (*format <= '9')) ||
                   ((*format == '0') && (f.accuracy & FLAG_0))) {
          format = get_number((char *)format, &f.width);
        } else if (*format == '.') {
          format++;
          if (*format == '*') {
            f.accuracy = va_arg(param, unsigned int);
          } else if ((*format >= '0') && (*format <= '9')) {
            format = get_number((char *)format, &f.accuracy);
          } else {
            f.accuracy = 0;
            format--;
          }
        } else {
          now_position =
              specifiers_processing(f, param, *format, now_position, str);
          run = 0;
        }
        format++;
      }
    } else {
      *now_position = *format;
      now_position++;
      format++;
    }
  }
  *now_position = 0;
  va_end(param);
  return now_position - str;
}

s21_size_t s21_strlen(const char *str) {
  s21_size_t i = 0;
  for (; str[i]; i++) {
  }
  return i;
}

char *s21_strchr(const char *string, int symbol) {
  char *pointer = s21_NULL;
  int index = -1;
  do {
    index++;
    if (symbol == string[index]) {
      pointer = (char *)(string + index);
    }
  } while ((pointer == s21_NULL) && (string[index] != 0));
  return pointer;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  int rez = 0;
  char *char_in_str = s21_NULL;
  for (int i = 0; str1[i] != 0 && rez != 1; i++) {
    for (int ii = 0; str2[ii] != 0 && rez != 1; ii++) {
      if (str1[i] == str2[ii]) {
        char_in_str = (char *)(str1 + i);
        rez = 1;
      }
    }
  }
  return char_in_str;
}

char *s21_strrchr(const char *str, int c) {
  char *char_in_str = s21_NULL;
  for (int i = 0; str[i] != 0; i++) {
    if (str[i] == c) {
      char_in_str = (char *)(str + i);
    }
  }
  return char_in_str;
}

char *s21_strstr(const char *haystack, const char *needle) {
  char *char_in_str = s21_NULL;
  int lenA = s21_strlen(needle);
  int max = s21_strlen(haystack) - lenA;
  if (lenA == 0) {
    char_in_str = (char *)haystack;
  } else {
    for (int i = 0; i <= max; i++) {
      int ii = 0;
      for (; (haystack[i + ii] == needle[ii]) && needle[ii] != 0; ii++) {
      }
      if (ii == lenA) {
        char_in_str = (char *)(haystack + i);
        i = max;
      }
    }
  }
  return char_in_str;
}

char *s21_strtok(char *str, const char *sep) {
  static char *address;
  if (str != s21_NULL) {
    address = s21_NULL;
  } else {
    str = address;
  }
  char *rezult = s21_NULL;
  if (str != s21_NULL) {
    rezult = str;
    if (s21_strpbrk(str, sep) != s21_NULL) {
      do {
        rezult = str;
        address = s21_strpbrk(str, sep);
        *address = 0;
        address++;
        str = address;
      } while ((*rezult == 0) && s21_strpbrk(str, sep) != s21_NULL);
      if ((*rezult == 0) && (s21_strpbrk(str, sep) == s21_NULL)) {
        rezult = str;
        address = s21_NULL;
        if (*rezult == 0) {
          rezult = s21_NULL;
        }
      }
    } else if (*str != 0) {
      address = s21_NULL;
    } else {
      rezult = s21_NULL;
    }
  }
  return rezult;
}

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  int i = 0;
  char *rezult = s21_NULL;
  if ((src != s21_NULL)) {
    int str_length;
    if (str != s21_NULL) {
      str_length = s21_strlen(str);
    } else {
      str_length = 0;
    }
    int length = s21_strlen(src) + str_length;
    char *new_str = s21_NULL;
    new_str = (char *)malloc((length + 1) * sizeof(char));
    for (; (i < (int)start_index) && (i < length); i++) {
      new_str[i] = src[i];
    }
    if (str != s21_NULL) {
      for (int j = 0; j < str_length && (i < length); i++, j++) {
        new_str[i] = str[j];
      }
    }
    for (int j = start_index; (src[j] != 0) && (i < length); i++, j++) {
      new_str[i] = src[j];
    }
    if (i < length + 1) {
      new_str[i] = 0;
      rezult = new_str;
    }
  }
  return rezult;
}

void *s21_trim(const char *src, const char *trim_chars) {
  char *new_str = s21_NULL;
  if (src != NULL) {
    int src_length = s21_strlen(src);
    new_str = (char *)malloc((src_length) * sizeof(char));
    for (int i = 0; i <= src_length; i++) {
      new_str[i] = src[i];
    }
    if (trim_chars != s21_NULL) {
      int trim_length = s21_strlen(trim_chars);
      for (int i = 0; i < src_length; i++) {
        int is_true = 0;
        for (int ii = 0; (ii < trim_length) && (!is_true); ii++) {
          if (new_str[i] == trim_chars[ii]) {
            new_str[i] = 0;
            is_true = 1;
          }
        }
        if (!is_true) {
          i = src_length;
        }
      }
      for (int i = src_length - 1; i > 0; i--) {
        int is_true = 0;
        for (int ii = 0; (ii < trim_length) && (!is_true); ii++) {
          if (new_str[i] == trim_chars[ii]) {
            new_str[i] = 0;
            is_true = 1;
          }
        }
        if (!is_true) {
          i = 0;
        }
      }
      int num = 0;
      for (int i = 0; i < src_length; i++) {
        if (new_str[i] != 0) {
          new_str[num] = new_str[i];
          num++;
        }
      }
      new_str[num] = 0;
    }
  }
  return new_str;
}

char *s21_strerror(int errnum) {
  char *res;
  char buff[BUFF_SIZE];
  if (errnum <= ARRAY_SIZE) {
    res = errors_array[errnum];
  } else {
    sprintf(buff, "%s%d", unknown_error, errnum);
    res = buff;
  }
  return res;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int result = 0;
  for (s21_size_t i = 0; i < n; i++) {
    if (str1[i] > str2[i]) {
      result = str1[i] - str2[i];
      i = n;
    } else if (str1[i] < str2[i]) {
      result = str1[i] - str2[i];
      i = n;
    }
  }
  return result;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  int num = (int)n;
  for (int i = 0; i < num; i++) {
    dest[i] = src[i];
  }
  return dest;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t res = 0;
  int symbol_found = 0;
  for (int i = 0; str1[i] != '\0'; i++) {
    res = i;
    for (int j = 0; str2[j] != '\0'; j++) {
      if (str1[i] == str2[j]) {
        symbol_found = 1;
        break;
      }
    }
    if (symbol_found == 1) {
      break;
    }
  }
  if (symbol_found == 0) {
    res = s21_strlen(str1);
  }
  return res;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int result = 0;
  unsigned char *l = (unsigned char *)str1;
  unsigned char *r = (unsigned char *)str2;
  for (s21_size_t i = 0; i < n; i++) {
    if ((result = (*(l + i) - *(r + i))) != 0) {
      break;
    }
  }
  return result;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  for (s21_size_t i = 0; i < n; i++) {
    ((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
  }
  return dest;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  for (s21_size_t i = 0; i < n; i++) {
    ((unsigned char *)str)[i] = c;
  }
  return str;
}

void *s21_memchr(const void *str, int c, s21_size_t n) {
  const unsigned char *p = str;
  for (s21_size_t i = 0; i < n; i++) {
    if ((unsigned char)c == p[i]) {
      return (void *)(p + i);
    }
  }
  return s21_NULL;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  int count = 0;
  while (dest[count] != 0) {
    count++;
  }
  s21_size_t i = 0;
  for (; i < n && src[i] != '\0'; i++) {
    dest[count + i] = src[i];
  }
  dest[count + i] = '\0';
  return dest;
}

void *s21_to_upper(const char *str) {
  char *dest = s21_NULL;
  if (str != s21_NULL) {
    int count = s21_strlen(str);
    dest = (char *)malloc((count + 1) * sizeof(char));
    if (dest != NULL) {
      for (int i = 0; i < count; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
          dest[i] = str[i] - 32;
        } else {
          dest[i] = str[i];
        }
      }
      dest[count] = 0;
    }
  }
  return (void *)dest;
}

void *s21_to_lower(const char *str) {
  char *dest = s21_NULL;
  if (str != s21_NULL) {
    int count = s21_strlen(str);
    dest = (char *)malloc((count + 1) * sizeof(char));
    if (dest != NULL) {
      for (int i = 0; i < count; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
          dest[i] = str[i] + 32;
        } else {
          dest[i] = str[i];
        }
      }
      dest[count] = 0;
    }
  }
  return (void *)dest;
}
