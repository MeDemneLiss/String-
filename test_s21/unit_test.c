#include <check.h>
#include <errno.h>
#include <string.h>

#include "../s21_string.h"

START_TEST(test_s21_strerror) {
  const char *result = s21_strerror(ENOENT);
  ck_assert_str_eq(result, strerror(ENOENT));
  ck_assert_str_eq(s21_strerror(999), strerror(999));
}
END_TEST

START_TEST(test_s21_strlen) {
  char str[10] = "abc";
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(test_s21_strchr) {
  ck_assert_str_eq(s21_strchr("abc53m", 'c'), strchr("abc53m", 'c'));
  ck_assert_pstr_eq(s21_strchr("abc53m", 'k'), strchr("abc53m", 'k'));
}
END_TEST

START_TEST(test_s21_strpbrk) {
  ck_assert_str_eq(s21_strpbrk("abc53m", "3nla"), strpbrk("abc53m", "3nla"));
  ck_assert_pstr_eq(s21_strpbrk("abc53m", "k"), strpbrk("abc53m", "k"));
}
END_TEST

START_TEST(test_s21_strrchr) {
  ck_assert_str_eq(s21_strrchr("HelloMyhomie", 'h'),
                   strrchr("HelloMyhomie", 'h'));
  ck_assert_pstr_eq(s21_strrchr("abc53m", 'k'), strrchr("abc53m", 'k'));
}
END_TEST

START_TEST(test_s21_strstr) {
  ck_assert_str_eq(s21_strstr("abc3sf3", "c3s"), strstr("abc3sf3", "c3s"));
  ck_assert_pstr_eq(s21_strstr("abc3lsf3", "c3s"), strstr("abc3lsf3", "c3s"));
  ck_assert_str_eq(s21_strstr("abc3sf3", ""), strstr("abc3sf3", ""));
}
END_TEST

START_TEST(test_s21_strtok) {
  char str1[100] = "aaaa2bbb2bbc2";
  char str2[100] = "2b";
  char str3[100] = "";
  char str4[100] = "";
  ck_assert_str_eq(s21_strtok(str1, str2), strtok(str1, str2));
  ck_assert_pstr_eq(s21_strtok("", "f"), strtok("", "f"));
  ck_assert_pstr_eq(s21_strtok(str3, str4), strtok(str3, str4));
}
END_TEST

START_TEST(test_s21_insert) {
  char str1[100] = "HelloFriend";
  char str2[100] = "My";
  char *res = s21_insert(str1, str2, 5);
  ck_assert_str_eq(res, "HelloMyFriend");
  free(res);
}
END_TEST

START_TEST(test_s21_trim) {
  char str[100] = "123abc456xyz789";
  char str2[100] = "123456789";
  char *res = s21_trim(str, str2);
  ck_assert_str_eq(res, "abc456xyz");
  free(res);
}
END_TEST

START_TEST(test_s21_strncmp) {
  ck_assert_int_eq(s21_strncmp("98772354", "1234325", 5),
                   strncmp("98772354", "1234325", 5));
  ck_assert_int_eq(s21_strncmp("28772354", "9234325", 5),
                   strncmp("28772354", "9234325", 5));
  ck_assert_int_eq(s21_strncmp("123456789", "123456789", 5),
                   strncmp("123456789", "123456789", 5));
}
END_TEST

START_TEST(test_s21_strncpy) {
  char str1[100] = "";
  char str2[100] = "asdfghj1";
  char str3[100] = "";
  ck_assert_str_eq(s21_strncpy(str1, str2, 3), strncpy(str1, str2, 3));
  ck_assert_str_eq(s21_strncpy(str2, str3, 3), strncpy(str2, str3, 3));
}
END_TEST

START_TEST(test_s21_strcspn) {
  char str1[100] = "abvc1sdfdshk1";
  char str2[100] = "123456dsjg";
  char str3[100] = "";
  ck_assert_int_eq(s21_strcspn(str1, str2), strcspn(str1, str2));
  ck_assert_int_eq(s21_strcspn(str3, str2), strcspn(str3, str2));
  ck_assert_int_eq(s21_strcspn(str1, str3), strcspn(str1, str3));
}
END_TEST

START_TEST(test_s21_memcmp) {
  char str1[100] = "98764214";
  char str2[100] = "1736452";
  char str3[1000] = "1736452";
  int res1 = s21_memcmp(str1, str2, 7);
  int res2 = memcmp(str1, str2, 7);
  int res3 = s21_memcmp(str3, str2, 4);
  int res4 = memcmp(str3, str2, 4);
  int res5 = s21_memcmp(str2, str1, 4);
  int res6 = memcmp(str2, str1, 4);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(res3, res4);
  ck_assert_int_eq(res5, res6);
}
END_TEST

START_TEST(test_s21_memcpy) {
  char str1[100] = "abcdddd";
  char str2[100] = "123456";
  char str3[100] = "";
  char *res = s21_memcpy(str1, str2, 3);
  char *res2 = memcpy(str1, str2, 3);
  char *res3 = s21_memcpy(str2, str3, 3);
  char *res4 = memcpy(str2, str3, 3);
  ck_assert_str_eq(res, res2);
  ck_assert_str_eq(res3, res4);
}
END_TEST

START_TEST(test_s21_memset) {
  char str1[100] = "abcdddd";
  char str2[100] = "123456";
  char *res = s21_memset(str1, '0', 3);
  char *res2 = memset(str1, '0', 3);
  char *res3 = s21_memset(str2, 'k', 7);
  char *res4 = memset(str2, 'k', 7);
  ck_assert_str_eq(res, res2);
  ck_assert_str_eq(res3, res4);
}
END_TEST

START_TEST(test_s21_memchr) {
  ck_assert_str_eq(s21_memchr("abc53m", 'c', 5), memchr("abc53m", 'c', 5));
  ck_assert_pstr_eq(s21_memchr("abc53m", 'k', 6), memchr("abc53m", 'k', 6));
}
END_TEST

START_TEST(test_s21_strncat) {
  char str1[100] = "abcdddd";
  char str2[100] = "123456";
  char *res = s21_strncat(str1, str2, 4);
  char *res2 = strncat(str1, str2, 4);
  char *res3 = s21_strncat(str1, str2, 7);
  char *res4 = strncat(str1, str2, 7);
  ck_assert_str_eq(res, res2);
  ck_assert_str_eq(res3, res4);
}
END_TEST

START_TEST(test_s21_to_upper) {
  char str[100] = "abc11DWEYF";
  char str2[100] = "ABC11DWEYF";
  char str3[100] = "";
  char *rez = s21_to_upper(str);
  ck_assert_str_eq(rez, str2);
  free(rez);
  char *rez2 = s21_to_upper(str3);
  ck_assert_str_eq(rez2, "");
  free(rez2);
  ck_assert_pstr_eq(s21_to_upper(NULL), NULL);
}
END_TEST

START_TEST(test_s21_to_lower) {
  char str[100] = "abc11DWEYF";
  char str2[100] = "abc11dweyf";
  char str3[100] = "";
  char *rez = s21_to_lower(str);
  ck_assert_str_eq(rez, str2);
  free(rez);
  char *rez2 = s21_to_lower(str3);
  ck_assert_str_eq(rez2, "");
  free(rez2);
  ck_assert_pstr_eq(s21_to_lower(NULL), NULL);
}
END_TEST

START_TEST(test_sprintf_specif_c) {
  for (int i = 0; i < 255; i++) {
    char my[300];
    char standart[300];
    s21_sprintf(my, "%c", i);
    sprintf(standart, "%c", i);
    ck_assert_str_eq(my, standart);
  }
}
END_TEST

START_TEST(test_sprintf_specif_d) {
  for (int i = -214748; i < 2147483; i += 10) {
    char my[300];
    char standart[300];
    s21_sprintf(my, "%*d", -12, i);
    sprintf(standart, "%*d", -12, i);
    ck_assert_str_eq(my, standart);
  }
}
END_TEST

START_TEST(test_sprintf_specif_i) {
  for (int i = -21474; i < 21476; i++) {
    char my[300];
    char standart[300];
    s21_sprintf(my, "%8.10i %.i %.i %+.0i %i %45i %+5i %9i %-10i", i, i, i, i,
                i, i, i, i, i);
    sprintf(standart, "%8.10i %.i %.i %+.0i %i %45i %+5i %9i %-10i", i, i, i, i,
            i, i, i, i, i);
    ck_assert_str_eq(my, standart);
  }
}
END_TEST

START_TEST(test_sprintf_specif_g) {
  char my[900];
  char standart[900];
  s21_sprintf(my, "%+08.10g %.6g %g %45g %+5g %#9g %-10g", 0.234, 234.342,
              234234.234234, 546.234, 3242.3, 3243545453.435345, 435.5);
  sprintf(standart, "%+08.10g %.6g %g %45g %+5g %#9g %-10g", 0.234, 234.342,
          234234.234234, 546.234, 3242.3, 3243545453.435345, 435.5);
  ck_assert_str_eq(my, standart);
}
END_TEST

START_TEST(test_sprintf_specif_E) {
  for (double i = -2; i < 2; i += 0.0001) {
    char my[300];
    char standart[300];
    s21_sprintf(my, "%E %#8E", i, i);
    sprintf(standart, "%E %#8E", i, i);
    ck_assert_str_eq(my, standart);
  }
}
END_TEST

START_TEST(test_sprintf_specif_f) {
  for (double i = 0; i < 2.123883; i += 0.00001) {
    char my[300];
    char standart[300];
    s21_sprintf(my, "% f", i);
    sprintf(standart, "% f", i);
    ck_assert_str_eq(my, standart);
  }
}
END_TEST

START_TEST(test_sprintf_specif_o) {
  for (int i = 0; i < 20932; i++) {
    char my[300];
    char standart[300];
    s21_sprintf(my, "%o %#o", i, 45458);
    sprintf(standart, "%o %#o", i, 45458);
    ck_assert_str_eq(my, standart);
  }
}
END_TEST

START_TEST(test_sprintf_specif_s) {
  char my[300];
  char standart[300];
  char *j = NULL;
  s21_sprintf(my, "%34s %s %s %s", "frimgkff,bg,A{,A<cs,vsx, 8q  323 r 4 gs }",
              "", "f5", j);
  sprintf(standart, "%34s %s %s %s",
          "frimgkff,bg,A{,A<cs,vsx, 8q  323 r 4 gs }", "", "f5", j);
  ck_assert_str_eq(my, standart);
}
END_TEST

START_TEST(test_sprintf_specif_u) {
  for (int i = -1000; i < 20932; i++) {
    char my[300];
    char standart[300];
    s21_sprintf(my, "%u", i);
    sprintf(standart, "%u", i);
    ck_assert_str_eq(my, standart);
  }
}
END_TEST

START_TEST(test_sprintf_specif_x) {
  for (unsigned int i = 0; i < 20932; i++) {
    char my[300];
    char standart[300];
    s21_sprintf(my, "%x %#-x", i, i);
    sprintf(standart, "%x %#-x", i, i);
    ck_assert_str_eq(my, standart);
  }
}
END_TEST

START_TEST(test_sprintf_specif_X) {
  for (int i = 0; i < 20932; i++) {
    char my[300];
    char standart[300];
    s21_sprintf(my, "%*.*X   %#-11X", 4, 9, i, i);
    sprintf(standart, "%*.*X   %#-11X", 4, 9, i, i);
    ck_assert_str_eq(my, standart);
  }
}
END_TEST

START_TEST(test_sprintf_specif_p) {
  for (int i = 0; i < 20932; i++) {
    char my[300];
    char standart[300];
    void *d = &i;
    s21_sprintf(my, "%p", d);
    sprintf(standart, "%p", d);
    ck_assert_str_eq(my, standart);
  }
}
END_TEST

START_TEST(test_sprintf_specif_e) {
  for (double i = -2; i < 2; i += 0.0001) {
    char my[300];
    char standart[300];
    s21_sprintf(my, "%e", i);
    sprintf(standart, "%e", i);
    ck_assert_str_eq(my, standart);
  }
}
END_TEST

START_TEST(test_sprintf_specif_n_persent) {
  char my[300];
  char standart[300];
  int n1 = 0;
  int n2 = 0;
  s21_sprintf(my, "dsf%%sdf %n", &n1);
  sprintf(standart, "dsf%%sdf %n", &n2);
  ck_assert_str_eq(my, standart);
  ck_assert_int_eq(n1, n2);
}
END_TEST

START_TEST(s21_sprintf_hard_test1) {
  char str1[200];
  char str2[200];
  char *str3 = "%0s test %0.s test1 %10s TEXT %3.4s TEXT1 %.s";
  char *val = "hello, world!";
  char *val2 = "text";
  char *val3 = "testus";
  char *val4 = "testikus";
  char *val5 = "testatus";
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3, val4, val5),
                   s21_sprintf(str2, str3, val, val2, val3, val4, val5));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(s21_sprintf_hard_test2) {
  char str1[100];
  char str2[100];
  char *str3 = "%lu text %lu text1 %hu TEXT %hu";
  long unsigned int val = 1305713857137507;
  long unsigned val2 = 72457698246;
  unsigned short val3 = 23800;
  unsigned short val4 = 111;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3, val4),
                   s21_sprintf(str2, str3, val, val2, val3, val4));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(s21_sprintf_hard_test3) {
  char str1[200] = "";
  char str2[200] = "";
  char *str3 = "%#G %#.0g %020G %020.5g %.2f %.0f %.2e %.0e";
  float f1 = 5.00f;
  float f2 = 5.0051f;
  float f3 = 7.9991f;
  int res1 = sprintf(str1, str3, f1, f1, f1, f2, f3, f3, f3, f3);
  int res2 = s21_sprintf(str2, str3, f1, f1, f1, f2, f3, f3, f3, f3);
  ck_assert_int_eq(res1, res2);
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(s21_sprintf_hard_test4) {
  char str1[200];
  char str2[200];
  char *str3 = "%7.o text %2.5o tetstasf %.13o TEST %.o";
  int val = 246;
  int val2 = 711;
  int val3 = 463;
  int val4 = 55;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3, val4),
                   s21_sprintf(str2, str3, val, val2, val3, val4));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(s21_sprintf_hard_test5) {
  char str1[200];
  char str2[200];
  char *str3 = "%15.43x text %-.3x tetstasf %3x TESTTEXT %-11.2x";
  int val = 246;
  int val2 = 711;
  int val3 = 463;
  int val4 = 55;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3, val4),
                   s21_sprintf(str2, str3, val, val2, val3, val4));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(s21_sprintf_hard_test6) {
  char str1[400] = "";
  char str2[400] = "";
  char *str3 = "{%+-#Lf}{%.Lf}{%.Lf}{%-#.f}{%+#.Lf}";
  long double num = 111.214255;
  ck_assert_int_eq(sprintf(str1, str3, num, num, num, num, num),
                   s21_sprintf(str2, str3, num, num, num, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(s21_sprintf_hard_test7) {
  char str1[100];
  char str2[100];
  char *str3 = "%1.1c abc %-5.7c abc %11.3lc abc %-11.3lc abc %0lc";
  int a = -11;
  unsigned long int b = 12;
  unsigned long int c = 12;
  ck_assert_int_eq(sprintf(str1, str3, a, a, b, c, c),
                   s21_sprintf(str2, str3, a, a, b, c, c));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(s21_sscanf_test_1) {
  char *str = "a hello world z";
  char *format = "%c hello %nworld %c ";
  int n1, n2;
  char a1, a2, b1, b2;
  ck_assert_int_eq(sscanf(str, format, &a1, &n1, &b1),
                   s21_sscanf(str, format, &a2, &n2, &b2));
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(n1, n2);
  ck_assert_int_eq(b1, b2);
}
END_TEST

START_TEST(s21_sscanf_test_2) {
  char *str = " 12 19  -123  0.0  1234 ";
  char *format = "%d %*d %d %lf %d";
  int a1, a2, a3, b1, b2, b3;
  double fl1, fl2;
  ck_assert_int_eq(sscanf(str, format, &a1, &a2, &fl1, &a3),
                   s21_sscanf(str, format, &b1, &b2, &fl2, &b3));
  ck_assert_int_eq(a1, b1);
  ck_assert_int_eq(a2, b2);
  ck_assert_int_eq(a3, b3);
  ck_assert_float_eq(fl1, fl2);
}
END_TEST

START_TEST(s21_sscanf_test_3) {
  char *str = "hi hell no  helno world z";
  char *format = "%ci hell no  helno %nworld %c";
  int n1, n2;
  char a1, a2, b1, b2;
  ck_assert_int_eq(sscanf(str, format, &a1, &n1, &b1),
                   s21_sscanf(str, format, &a2, &n2, &b2));
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
  ck_assert_int_eq(n2, n2);
}
END_TEST

START_TEST(s21_sscanf_test_4) {
  char *str = "113 12.0  -1  3451 0.0 ";
  char *format = "%i %*lf %i %i %lf";
  int a1, a2, a3, b1, b2, b3;
  double fl1, fl2;
  ck_assert_int_eq(sscanf(str, format, &a1, &a2, &fl1, &a3),
                   s21_sscanf(str, format, &b1, &b2, &fl2, &b3));
  ck_assert_int_eq(a1, b1);
  ck_assert_int_eq(a2, b2);
  ck_assert_int_eq(a3, b3);
  ck_assert_float_eq(fl1, fl2);
}
END_TEST

START_TEST(s21_sscanf_test_5) {
  char *str = " 12.1111 -19.13412  123.1  0.0  -1234 ";
  char *format = "%lf %lf %lf %lf %lf";
  double fl1, fl2, a1, a2, a3, b1, b2, b3;
  ck_assert_int_eq(sscanf(str, format, &a1, &a2, &fl1, &a3),
                   s21_sscanf(str, format, &b1, &b2, &fl2, &b3));
  ck_assert_float_eq(a1, b1);
  ck_assert_float_eq(a2, b2);
  ck_assert_float_eq(a3, b3);
  ck_assert_float_eq(fl1, fl2);
}
END_TEST

START_TEST(s21_sscanf_test_6) {
  double my1, my2, my3, my4, standart1, standart2, standart3, standart4;
  char my5, standart5;
  int my6, standart6;
  char *format = "%e %E %c %n%g %G";
  char *str = "-15e7 356.657 f 23.967e-2 4367.34e4";
  ck_assert_int_eq(sscanf(str, format, &standart1, &standart2, &standart5,
                          &standart6, &standart3, &standart4),
                   s21_sscanf(str, format, &my1, &my2, &my5, &my6, &my3, &my4));
  ck_assert_double_eq_tol(my1, standart1, 0.000001);
  ck_assert_double_eq_tol(my2, standart2, 0.000001);
  ck_assert_double_eq_tol(my3, standart3, 0.000001);
  ck_assert_double_eq_tol(my4, standart4, 0.000001);
  ck_assert_int_eq(my5, standart5);
  ck_assert_int_eq(my6, standart6);
}
END_TEST

START_TEST(s21_sscanf_test_7) {
  int my1, my2, my3, my4, standart1, standart2, standart3, standart4;
  char *format = "%o %x %% %X %i";
  char *str = "-15 64A % 3C67 0x436";
  ck_assert_int_eq(
      sscanf(str, format, &standart1, &standart2, &standart3, &standart4),
      s21_sscanf(str, format, &my1, &my2, &my3, &my4));
  ck_assert_int_eq(my1, standart1);
  ck_assert_int_eq(my2, standart2);
  ck_assert_int_eq(my3, standart3);
  ck_assert_int_eq(my4, standart4);
}
END_TEST

START_TEST(s21_sscanf_test_8) {
  short my1, standart1;
  long my2, standart2;
  long double my3, standart3;
  float my4, standart4;
  int my5, standart5;
  char *format = "%hd %ld %% %Lf %n%f";
  char *str = "-15 644567 % 4545.687e2 -5454.332";
  ck_assert_int_eq(sscanf(str, format, &standart1, &standart2, &standart3,
                          &standart5, &standart4),
                   s21_sscanf(str, format, &my1, &my2, &my3, &my5, &my4));
  ck_assert_int_eq(my1, standart1);
  ck_assert_int_eq(my2, standart2);
  ck_assert_int_eq(my3, standart3);
  ck_assert_double_eq(my4, standart4);
  ck_assert_int_eq(my5, standart5);
}
END_TEST

START_TEST(s21_sscanf_test_9) {
  char my1[128] = {0}, my2[128] = {0}, standart1[128] = {0},
       standart2[128] = {0};
  char my3, standart3;
  char *format = "%s cyclik %6c%c %*c %*s";
  char *str = "oslik cyclik paykans";
  ck_assert_int_eq(sscanf(str, format, &standart1, &standart2, &standart3),
                   s21_sscanf(str, format, &my1, &my2, &my3));
  ck_assert_str_eq(my1, standart1);
  ck_assert_str_eq(my2, standart2);
  ck_assert_int_eq(my3, standart3);
}
END_TEST

START_TEST(s21_sscanf_test_10) {
  int my1, my2, my3, my5, my8, standart1, standart2, standart3, standart5,
      standart8;
  unsigned int my4, standart4;
  unsigned short my6, standart6;
  unsigned long my7, standart7;
  float my9, my10, standart9, standart10;
  char *format = "%*o %x %% %*X %*x %n%*d %3d %4i %u %hu %lu %4x %6e %4f";
  char *str =
      "-15 0x64d % 3C67 43f5 7457 345 0436 -456 35 982378 4da6 345e-2 3.571";
  ck_assert_int_eq(sscanf(str, format, &standart1, &standart5, &standart2,
                          &standart3, &standart4, &standart6, &standart7,
                          &standart8, &standart9, &standart10),
                   s21_sscanf(str, format, &my1, &my5, &my2, &my3, &my4, &my6,
                              &my7, &my8, &my9, &my10));
  ck_assert_int_eq(my1, standart1);
  ck_assert_int_eq(my2, standart2);
  ck_assert_int_eq(my3, standart3);
  ck_assert_int_eq(my4, standart4);
  ck_assert_int_eq(my5, standart5);
  ck_assert_int_eq(my6, standart6);
  ck_assert_int_eq(my7, standart7);
  ck_assert_int_eq(my8, standart8);
  ck_assert_float_eq(my9, standart9);
  ck_assert_float_eq(my10, standart10);
}
END_TEST

int main(void) {
  Suite *suite = suite_create("S21StringUtils");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_strerror);
  tcase_add_test(tc_core, test_s21_strlen);
  tcase_add_test(tc_core, test_s21_strchr);
  tcase_add_test(tc_core, test_s21_strpbrk);
  tcase_add_test(tc_core, test_s21_strrchr);
  tcase_add_test(tc_core, test_s21_strstr);
  tcase_add_test(tc_core, test_s21_strtok);
  tcase_add_test(tc_core, test_s21_insert);
  tcase_add_test(tc_core, test_s21_trim);
  tcase_add_test(tc_core, test_s21_strncmp);
  tcase_add_test(tc_core, test_s21_strncpy);
  tcase_add_test(tc_core, test_s21_strcspn);
  tcase_add_test(tc_core, test_s21_memcmp);
  tcase_add_test(tc_core, test_s21_memcpy);
  tcase_add_test(tc_core, test_s21_memset);
  tcase_add_test(tc_core, test_s21_memchr);
  tcase_add_test(tc_core, test_s21_strncat);
  tcase_add_test(tc_core, test_s21_to_upper);
  tcase_add_test(tc_core, test_s21_to_lower);
  tcase_add_test(tc_core, test_sprintf_specif_c);
  tcase_add_test(tc_core, test_sprintf_specif_d);
  tcase_add_test(tc_core, test_sprintf_specif_i);
  tcase_add_test(tc_core, test_sprintf_specif_g);
  tcase_add_test(tc_core, test_sprintf_specif_E);
  tcase_add_test(tc_core, test_sprintf_specif_f);
  tcase_add_test(tc_core, test_sprintf_specif_o);
  tcase_add_test(tc_core, test_sprintf_specif_s);
  tcase_add_test(tc_core, test_sprintf_specif_u);
  tcase_add_test(tc_core, test_sprintf_specif_x);
  tcase_add_test(tc_core, test_sprintf_specif_X);
  tcase_add_test(tc_core, test_sprintf_specif_p);
  tcase_add_test(tc_core, s21_sprintf_hard_test1);
  tcase_add_test(tc_core, s21_sprintf_hard_test2);
  tcase_add_test(tc_core, s21_sprintf_hard_test3);
  tcase_add_test(tc_core, s21_sprintf_hard_test4);
  tcase_add_test(tc_core, s21_sprintf_hard_test5);
  tcase_add_test(tc_core, s21_sprintf_hard_test6);
  tcase_add_test(tc_core, s21_sprintf_hard_test7);

  tcase_add_test(tc_core, s21_sscanf_test_1);
  tcase_add_test(tc_core, s21_sscanf_test_2);
  tcase_add_test(tc_core, s21_sscanf_test_3);
  tcase_add_test(tc_core, s21_sscanf_test_4);
  tcase_add_test(tc_core, s21_sscanf_test_5);
  tcase_add_test(tc_core, s21_sscanf_test_6);
  tcase_add_test(tc_core, s21_sscanf_test_7);
  tcase_add_test(tc_core, s21_sscanf_test_8);
  tcase_add_test(tc_core, s21_sscanf_test_9);
  tcase_add_test(tc_core, s21_sscanf_test_10);

  tcase_add_test(tc_core, test_sprintf_specif_n_persent);
  tcase_add_test(tc_core, test_sprintf_specif_e);
  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_VERBOSE);

  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (failures == 0) ? 0 : 1;
}
