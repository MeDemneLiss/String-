#include "s21_string.h"

void handle_c(va_list *argv, const char *str, sscanf_format_specifier *f) {
  if (f->width > 0) {
    handle_s(argv, str, f);
  } else {
    if (!f->asterisk) {
      f->success++;
      char *destination = va_arg(*argv, char *);
      *destination = *str;
    }
    str++;
    f->chars_scanned++;
  }
}

void handle_s(va_list *argv, const char *str, sscanf_format_specifier *f) {
  skip_useless_chars(&str, f);
  if (!f->asterisk) {
    char *destination = va_arg(*argv, char *);
    while (*str && *str != ' ' && *str != '\n' && f->width--) {
      f->chars_scanned++;
      *destination++ = *str++;
    }
    *destination = '\0';
    f->success++;
  } else {
    while (*str && *str != ' ' && *str != '\n' && f->width--) {
      str++;
      f->chars_scanned++;
    }
  }
}

void handle_i(va_list *argv, const char *str, sscanf_format_specifier *f) {
  skip_useless_chars(&str, f);
  int sign = 1;
  parse_sign(&str, f, &sign);
  if (*str == '0') {
    str++;
    f->chars_scanned++;
    if (*str == 'x') {
      str++;
      f->chars_scanned++;
      handle_x(argv, str, f, sign, 1);
    } else {
      handle_o(argv, str, f, sign, 1);
    }
  } else {
    handle_d(argv, str, f, sign);
  }
}

void handle_d(va_list *argv, const char *str, sscanf_format_specifier *f,
              int sign) {
  skip_useless_chars(&str, f);
  parse_sign(&str, f, &sign);
  int dest = (int)from_str_to_int(&str, 0, f);
  if (sign) {
    dest *= sign;
  }
  if (!f->asterisk) {
    f->success++;
    case_d_write_to_destination(argv, f, &dest, 1);
  }
}

void handle_o(va_list *argv, const char *str, sscanf_format_specifier *f,
              int sign, int if_signed) {
  skip_useless_chars(&str, f);
  parse_sign(&str, f, &sign);
  int dest = (int)from_str_to_int(&str, 1, f);
  if (sign) {
    dest *= sign;
  }
  if (!f->asterisk) {
    f->success++;
    if (if_signed) {
      case_d_write_to_destination(argv, f, &dest, 1);
    } else {
      case_d_write_to_destination(argv, f, &dest, 0);
    }
  }
}

void handle_x(va_list *argv, const char *str, sscanf_format_specifier *f,
              int sign, int if_signed) {
  skip_useless_chars(&str, f);
  parse_sign(&str, f, &sign);
  if (*str == '0') {
    str++;
    f->chars_scanned++;
    f->width--;
    if (*str == 'x') {
      str++;
      f->chars_scanned++;
      f->width--;
    }
  }
  int dest = (int)from_str_to_int(&str, 2, f);
  if (sign) {
    dest *= sign;
  }
  if (!f->asterisk) {
    f->success++;
    if (if_signed) {
      case_d_write_to_destination(argv, f, &dest, 1);
    } else {
      case_d_write_to_destination(argv, f, &dest, 0);
    }
  }
}

void handle_u(va_list *argv, const char *str, sscanf_format_specifier *f) {
  skip_useless_chars(&str, f);
  int sign = 1;
  parse_sign(&str, f, &sign);
  int dest = from_str_to_int(&str, 0, f);
  dest *= sign;
  if (!f->asterisk) {
    case_d_write_to_destination(argv, f, &dest, 0);
    f->success++;
  }
}

void handle_e_f_g(va_list *argv, const char *str, sscanf_format_specifier *f) {
  skip_useless_chars(&str, f);
  int sign = 1;
  parse_sign(&str, f, &sign);
  long double dest = from_str_to_int(&str, 3, f);
  int exponent = 0;
  int sign_exponent = 1;
  if (*str == 101) {
    str++;
    f->chars_scanned++;
    if (*str == '-' || *str == '+') {
      if (*str == '-') {
        sign_exponent = -1;
      }
      str++;
      f->chars_scanned++;
    }
    exponent = (int)from_str_to_int(&str, 0, f);
  }
  dest = sign_exponent > 0 ? dest * pow(10, exponent)
                           : dest / pow(10, exponent + 1);
  dest *= sign;
  if (!f->asterisk) {
    f->success++;
    case_f_write_to_destination(argv, f, &dest);
  }
}

long double from_str_to_int(const char **str, int dec_oct_hex_mant,
                            sscanf_format_specifier *f) {
  long double dest = 0;
  int base = 10;
  int len_mantissa = -1;
  if (dec_oct_hex_mant == 1) {
    base = 8;
  } else if (dec_oct_hex_mant == 2) {
    base = 16;
  }
  if (f->width > 0) {
    for (; f->width > 0 &&
           ((**str >= '0' && **str <= '9') ||
            (dec_oct_hex_mant == 2 && **str >= 'A' && **str <= 'F') ||
            (dec_oct_hex_mant == 2 && **str >= 'a' && **str <= 'f') ||
            (dec_oct_hex_mant == 3 && **str == 46));
         f->width--, f->chars_scanned++, (*str)++) {
      read_char_to_int(*str, dec_oct_hex_mant, &dest, base, &len_mantissa);
    }
  } else {
    while ((**str >= '0' && **str <= '9') ||
           (dec_oct_hex_mant == 2 && **str >= 'A' && **str <= 'F') ||
           (dec_oct_hex_mant == 2 && **str >= 'a' && **str <= 'f') ||
           (dec_oct_hex_mant == 3 && **str == 46)) {
      read_char_to_int(*str, dec_oct_hex_mant, &dest, base, &len_mantissa);
      f->chars_scanned++;
      (*str)++;
    }
  }
  if (dec_oct_hex_mant == 3) {
    dest /= pow(10, len_mantissa);
  }
  return dest;
}

void read_char_to_int(const char *str, int dec_oct_hex_mant, long double *dest,
                      int base, int *len_mantissa) {
  if (*len_mantissa > -1) {
    (*len_mantissa)++;
  }
  if (*str >= '0' && *str <= '9') {
    *dest = *dest * base + *str - '0';
  } else if (dec_oct_hex_mant == 2 && *str >= 'A' && *str <= 'F') {
    *dest = *dest * 16 + *str - 55;
  } else if (dec_oct_hex_mant == 2 && *str >= 'a' && *str <= 'f') {
    *dest = *dest * 16 + *str - 87;
  } else if (dec_oct_hex_mant == 3 && *str == 46) {
    *len_mantissa = 0;
  }
}

void handle_p(va_list *argv, const char *str, sscanf_format_specifier *f) {
  skip_useless_chars(&str, f);
  if (*str == '0') {
    str++;
    f->chars_scanned++;
    if (*str == 'x') {
      str++;
      f->chars_scanned++;
    }
  }
  long long dest = (long long)from_str_to_int(&str, 2, f);
  if (!f->asterisk) {
    f->success++;
    void **destination = va_arg(*argv, void **);
    *destination = (void *)(0x0 + dest);
  }
}

void case_d_write_to_destination(va_list *argv,
                                 const sscanf_format_specifier *f, int *res,
                                 int if_signed) {
  if (if_signed) {
    if (f->modifier == '\0') {
      int *destination = va_arg(*argv, int *);
      *destination = *res;
    } else if (f->modifier == 'h') {
      short *destination = va_arg(*argv, short *);
      *destination = (short)*res;
    } else if (f->modifier == 'l') {
      long *destination = va_arg(*argv, long *);
      *destination = (long)*res;
    }
  } else {
    if (f->modifier == '\0') {
      unsigned int *destination = va_arg(*argv, unsigned int *);
      *destination = (unsigned int)*res;
    } else if (f->modifier == 'h') {
      unsigned short *destination = va_arg(*argv, unsigned short *);
      *destination = (unsigned short)*res;
    } else if (f->modifier == 'l') {
      unsigned long *destination = va_arg(*argv, unsigned long *);
      *destination = (unsigned long)*res;
    }
  }
}

void case_f_write_to_destination(va_list *argv,
                                 const sscanf_format_specifier *f,
                                 long double *res) {
  if (f->modifier == '\0') {
    float *destination = va_arg(*argv, float *);
    *destination = (float)*res;
  } else if (f->modifier == 'L') {
    long double *destination = va_arg(*argv, long double *);
    *destination = *res;
  } else {
    double *destination = va_arg(*argv, double *);
    *destination = (double)*res;
  }
}

void skip_useless_chars(const char **str, sscanf_format_specifier *f) {
  while (**str == ' ' || **str == '\n' || **str == '\t' || **str == '\r' ||
         **str == '\x0B' || **str == '\f') {
    (*str)++;
    f->chars_scanned++;
  }
}

void parse_sign(const char **str, sscanf_format_specifier *f, int *sign) {
  if (**str == '-' || **str == '+') {
    if (*sign && **str == '-') {
      *sign = -1;
    }
    (*str)++;
    f->chars_scanned++;
    f->width--;
  }
}
